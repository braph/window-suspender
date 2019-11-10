#include <string.h>
#include <stdbool.h>
#include "statement.h"
#include "common.h"
#include "wnck.h"
#include "kill.h"

extern Statement *config;
static GSList *applications;
hook_type window_hook;

/* ============================================================================
 * Suspend / Refresh / Resume / Restore
 *
 * Code for sending signals to an application.
 * ==========================================================================*/

typedef enum {
  STATE_UNHANDELED,
  STATE_DO_SUSPEND,
  STATE_SUSPENDED,
  STATE_DO_REFRESH,
  STATE_DO_DESTROY,
} ApplicationState;

typedef struct {
  GSource source;
  WnckApplication *wnck_app;
  ApplicationState state;
  unsigned int suspend_delay;
  unsigned int refresh_delay;
  unsigned int refresh_duration;
} Application;

static void kill_wnck_application(WnckApplication*, int);
static gboolean application_handle_state(GSource*, GSourceFunc, gpointer);

#define NOW 0
#define NEVER -1
static inline void application_next_state(Application* app, ApplicationState state, unsigned int timeout) {
  app->state = state;
  if (timeout == NOW) {
    g_source_set_ready_time((GSource*) app, NEVER);
    application_handle_state((GSource*) app, NULL, NULL);
  }
  else if (timeout == NEVER) g_source_set_ready_time((GSource*) app, NEVER);
  else g_source_set_ready_time((GSource*) app, g_get_monotonic_time() + timeout * 1000);
}

static gboolean application_handle_state(GSource *source, GSourceFunc f, gpointer p) {
  Application *app = (Application*) source;
  switch (app->state) {
    case STATE_UNHANDELED:
      log_critical("Programming error: Timeout for STATE_UNHANDELED should be NEVER\n");
      break;
    case STATE_DO_SUSPEND:
      //printf("Suspending"); wnck_application_dump(app->wnck_app);
      kill_wnck_application(app->wnck_app, SIGSTOP);
      if (app->refresh_delay)
        application_next_state(app, STATE_DO_REFRESH, app->refresh_delay);
      else
        application_next_state(app, STATE_SUSPENDED, NOW);
    case STATE_SUSPENDED:
      break;
    case STATE_DO_REFRESH:
      //printf("Waking"); wnck_application_dump(app->wnck_app);
      kill_wnck_application(app->wnck_app, SIGCONT);
      application_next_state(app, STATE_DO_SUSPEND, app->refresh_duration);
      break;
    case STATE_DO_DESTROY:
      return G_SOURCE_REMOVE;
  }

  return G_SOURCE_CONTINUE;
}

static inline Application* application_new(WnckApplication *wnck_app) {
  static GSourceFuncs gsource_funcs = {
    NULL, NULL, application_handle_state, NULL, NULL, NULL
  };

  GSource *source = g_source_new(&gsource_funcs, sizeof(Application));
  Application *app = (Application*) source;
  app->wnck_app = wnck_app;
  g_source_attach(source, NULL);
  return app;
}

static Application* get_application_for_wnck_application(WnckApplication *wnck_app) {
  for (GSList *l = applications; l; l = l->next)
    if (((Application*) l->data)->wnck_app == wnck_app)
      return l->data;
  return NULL;
}

static void kill_wnck_application(WnckApplication* wnck_app, int sig) {
  int pid = wnck_application_get_pid(wnck_app);
  g_return_if_fail(pid);
  log_signal("Sending %s to %d \"%s\"\n", (sig == SIGCONT ? "SIGCONT" : "SIGSTOP"),
      pid, wnck_application_get_name(wnck_app));
  kill(pid, sig);
  kill_children(pid, sig);
}

/* Suspend a process and cancel outstanding suspend/refresh jobs */
void window_suspend(WnckWindow *win, int suspend_delay, int refresh_delay, int refresh_duration) {
  WnckApplication *wnck_app = wnck_window_get_application(win);
  g_return_if_fail(wnck_app);
  Application *app = get_application_for_wnck_application(wnck_app);
  g_return_if_fail(app);

  if (app->state != STATE_UNHANDELED) {
    if (app->suspend_delay == suspend_delay &&
        app->refresh_delay == refresh_delay &&
        app->refresh_duration == refresh_duration)
      return; // Suspend parameters did not change;
  }

  app->suspend_delay = suspend_delay;
  app->refresh_delay = refresh_delay;
  app->refresh_duration = refresh_duration;
  application_next_state(app, STATE_DO_SUSPEND, app->suspend_delay);
}

/* Resume a process (only if it has previously been stopped by us) and cancel
 * outstanding suspend/refresh jobs */
void window_resume(WnckWindow *win) {
  WnckApplication *wnck_app = wnck_window_get_application(win);
  g_return_if_fail(wnck_app);
  Application *app = get_application_for_wnck_application(wnck_app);
  g_return_if_fail(app);

  if (app->state != STATE_UNHANDELED) {
    kill_wnck_application(wnck_app, SIGCONT);
    application_next_state(app, STATE_UNHANDELED, NEVER);
  }
}

/* Restore the process states */
void restore_processes() {
  log_verbose("Restoring processes ...");
  for (GSList *l = applications; l; l = l->next) {
    Application *app = l->data;
    if (app->state != STATE_UNHANDELED) {
      if (verbosity)
        wnck_application_dump(app->wnck_app);
      kill_wnck_application(app->wnck_app, SIGCONT);
    }
  }
}

/* ============================================================================
 * Window Matching
 *
 * Code for matching the windows of an application against the configured
 * conditions and executing the resulting statements.
 * ==========================================================================*/

/* Apply the configured rules to a window.
 *
 * Executes all statements except the suspend statements. These will be stored
 * in the `suspend_statements` argument if it is not NULL.
 *
 * Returns TRUE if a suspend statement has been found, FALSE otherwise. */
static bool window_apply_rules(WnckWindow *win, PointerArray *suspend_statements) {
  bool have_suspend = false;
  static PointerArray statements = { 0 };
  statements.size = 0;

  if (suspend_statements)
    log_verbose(" `- "); // Tree-style if called from application_apply_rules()
  log_verbose("%s\n", windump(win));
  statement_get_matched(config, win, &statements);

  // Execute all statements except the suspend statement.
  for (unsigned int i = 0; i < statements.size; ++i) {
    Statement *statement = (Statement*) statements.data[i];
    if (statement->type == STATEMENT_SUSPEND) {
      have_suspend = true;
      if (suspend_statements)
        pointer_array_add(suspend_statements, statement);
    }
    else
      statement_execute(statement, win);
  }

  return have_suspend;
}

/* Apply the configured rules to an application and its windows */
static void application_apply_rules(WnckApplication *app) {
  log_verbose("[ Application \"%s\" %d ]\n",
      wnck_application_get_name(app), wnck_application_get_pid(app));
  WnckWindow *win = NULL;
  Statement statement = {
    .type = STATEMENT_SUSPEND,
    .klass.suspend = { 0 }
  };
  static PointerArray suspend_statements = { 0 };
  suspend_statements.size = 0;

  for (GList *l = wnck_application_get_windows(app); l; l = l->next) {
    win = l->data;
    if (! window_apply_rules(win, &suspend_statements)) {
      // One window didn't match the criteria for suspend, do a resume instead
      log_verbose("    --> No suspend for: %s\n", windump(win));
      statement.type = STATEMENT_RESUME;
    }
  }

  if (statement.type == STATEMENT_SUSPEND) {
    for (unsigned int i = 0; i < suspend_statements.size; ++i) {
      Statement *s = (Statement*) suspend_statements.data[i];
      // Choose highest suspend_delay
      if (s->klass.suspend.suspend_delay > statement.klass.suspend.suspend_delay)
        statement.klass.suspend.suspend_delay = s->klass.suspend.suspend_delay;
      // Choose lowest refresh_delay (but ignore 0)
      if (!statement.klass.suspend.refresh_delay ||
          (s->klass.suspend.refresh_delay &&
          s->klass.suspend.refresh_delay < statement.klass.suspend.refresh_delay))
        statement.klass.suspend.refresh_delay = s->klass.suspend.refresh_delay;
      // Choost highest refresh_duration
      if (s->klass.suspend.refresh_duration > statement.klass.suspend.refresh_duration)
        statement.klass.suspend.refresh_duration = s->klass.suspend.refresh_duration;
    }
  }

  log_verbose("    --> Executing on: %s\n", windump(win));
  if (verbosity)
    statement_dump(&statement, 8, 0);
  statement_execute(&statement, win);
}

/* Apply the configuration rules to all applications of the screen */
static void screen_apply_rules(gpointer hook) {
  window_hook = GPOINTER_TO_UINT(hook);
  log_event("screen::%s\n", hook_to_str(window_hook));
  for (GSList *l = applications; l; l = l->next)
    application_apply_rules(((Application*) l->data)->wnck_app);
}

/* ============================================================================
 * Window Signals
 *
 * Code for connecting window events.
 * ==========================================================================*/

static void on_win_signal(WnckWindow* window, gpointer hook) {
  window_hook = GPOINTER_TO_UINT(hook);
  log_event("window::%s(): %s\n", hook_to_str(window_hook), windump(window));
  window_apply_rules(window, NULL);
}

static void on_window_state_changed(WnckWindow* window,
    WnckWindowState changed_mask,
    WnckWindowState new_state,
    gpointer hook)
{
  // Forward signal
  on_win_signal(window, hook);

  /* If the window changed its `above` state we have to re-apply the configuration
   * to all windows on the screen, since the `above` state affects our logic
   * of calculating the stackposition. */
  if (changed_mask & WNCK_WINDOW_STATE_ABOVE)
    screen_apply_rules(GUINT_TO_POINTER(HOOK_WINDOW_STACKING_CHANGED));
}

static inline void window_connect_signals(WnckWindow *win) {
  g_signal_connect(win, "class-changed",     (GCallback) on_win_signal,
      GUINT_TO_POINTER(HOOK_CLASS_CHANGED));
  g_signal_connect(win, "name-changed",      (GCallback) on_win_signal,
      GUINT_TO_POINTER(HOOK_NAME_CHANGED));
  g_signal_connect(win, "role-changed",      (GCallback) on_win_signal,
      GUINT_TO_POINTER(HOOK_ROLE_CHANGED));
  g_signal_connect(win, "type-changed",      (GCallback) on_win_signal,
      GUINT_TO_POINTER(HOOK_TYPE_CHANGED));
  g_signal_connect(win, "icon-changed",      (GCallback) on_win_signal,
      GUINT_TO_POINTER(HOOK_ICON_CHANGED));
  g_signal_connect(win, "workspace-changed", (GCallback) on_win_signal,
      GUINT_TO_POINTER(HOOK_WORKSPACE_CHANGED));
  g_signal_connect(win, "state-changed",     (GCallback) on_window_state_changed,
      GUINT_TO_POINTER(HOOK_STATE_CHANGED));
  //g_signal_connect(win, "actions_changed", ...)
  //g_signal_connect(win, "geometry_changed", ...)
}

/* ============================================================================
 * Screen Signals
 *
 * Code for connecting screen events.
 * ==========================================================================*/

/* === Application opened / Application closed === */
static void on_application_event(WnckScreen* _, WnckApplication* wnck_app, gpointer opened) {
  Application *app;
  if (opened) {
    app = application_new(wnck_app);
    applications = g_slist_prepend(applications, app);
  }
  else /* closed */ {
    app = get_application_for_wnck_application(wnck_app);
    application_next_state(app, STATE_DO_DESTROY, NOW);
    applications = g_slist_remove(applications, app);
  }
  log_event("screen::on_application_event(): %s %d %s\n", (opened ? "added" : "removed"),
      wnck_application_get_pid(wnck_app), wnck_application_get_name(wnck_app));
}

/* === Window opened / Window closed === */
static void on_window_event(WnckScreen* _, WnckWindow* window, gpointer hook) {
  if (GPOINTER_TO_UINT(window_hook) == HOOK_OPENED)
    window_connect_signals(window);
  on_win_signal(window, hook);
  // Signal `window-stacking-changed` will call screen_apply_rules()
}

int manager_init() {
  if (! (screen = wnck_screen_get_default()))
    return 0;

  // Application
  g_signal_connect(screen, "application-opened",
      (GCallback) on_application_event, GUINT_TO_POINTER(1));
  g_signal_connect(screen, "application-closed",
      (GCallback) on_application_event, GUINT_TO_POINTER(0));

  // Window
  g_signal_connect(screen, "window-opened",
      (GCallback) on_window_event, GUINT_TO_POINTER(HOOK_OPENED));
  g_signal_connect(screen, "window-closed",
      (GCallback) on_window_event, GUINT_TO_POINTER(HOOK_CLOSED));

  // Screen
  g_signal_connect_swapped(screen, "window-stacking-changed",
      (GCallback) screen_apply_rules, GUINT_TO_POINTER(HOOK_WINDOW_STACKING_CHANGED));
  g_signal_connect_swapped(screen, "active-window-changed",
      (GCallback) screen_apply_rules, GUINT_TO_POINTER(HOOK_ACTIVE_WINDOW_CHANGED));
  g_signal_connect_swapped(screen, "active-workspace-changed",
      (GCallback) screen_apply_rules, GUINT_TO_POINTER(HOOK_ACTIVE_WORKSPACE_CHANGED));
  g_signal_connect_swapped(screen, "showing-desktop-changed",
      (GCallback) screen_apply_rules, GUINT_TO_POINTER(HOOK_SHOWING_DESKTOP_CHANGED));

  return 1;
}
