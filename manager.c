#include <string.h>
#include <stdbool.h>
#include "statement.h"
#include "common.h"
#include "wnck.h"
#include "kill.h"

extern Statement *config;
static GSList *applications;

/* ============================================================================
 * Suspend / Refresh / Resume / Restore
 *
 * Code for sending signals to processes.
 * ==========================================================================*/

typedef enum {
  STATE_UNHANDELED = 0,
  STATE_IS_SUSPENDING,
  STATE_SUSPENDED,
} ApplicationState;

typedef struct {
  WnckApplication *wnck_app;
  ApplicationState state;
  unsigned int timeout_id;
  unsigned int suspend_delay;
  unsigned int refresh_delay;
  unsigned int refresh_duration;
} Application;

static gboolean suspend_callback(Application*);
static gboolean refresh_callback(Application*);

#define G_TIMEOUT_ADD(T, CALLBACK, DATA) \
  (T >= 1000 ? g_timeout_add_seconds(T/1000, (GSourceFunc) CALLBACK, DATA) \
             : g_timeout_add(T, (GSourceFunc) CALLBACK, DATA))

static Application* get_application_for_wnck_application(WnckApplication *wnck_app) {
  for (GSList *l = applications; l; l = l->next)
    if (((Application*) l->data)->wnck_app == wnck_app)
      return l->data;
  return NULL;
}

static void kill_wnck_application(WnckApplication* wnck_app, int sig) {
  int pid = wnck_application_get_pid(wnck_app);
  if (G_UNLIKELY(!pid)) {
    log_signal("kill_wnck_application: PID is 0");
    return;
  }
  log_signal("Sending %s to %d \"%s\"\n", (sig == SIGCONT ? "SIGCONT" : "SIGSTOP"),
      pid, wnck_application_get_name(wnck_app));
  kill(pid, sig);
  kill_children(pid, sig);
}

static void application_cancel_timeout(Application* app) {
  debug("Removing timeout source %d\n", app->timeout_id);
  if (app->timeout_id)
    g_source_remove(app->timeout_id);
  app->timeout_id = 0;
  app->state = STATE_UNHANDELED;
}

/* Suspend a process and cancel outstanding suspend/refresh jobs */
void window_suspend(WnckWindow *win, int suspend_delay, int refresh_delay, int refresh_duration) {
  WnckApplication *wnck_app = wnck_window_get_application(win);
  if (G_UNLIKELY(!wnck_app)) {
    verbose("window_suspend[0]: could not get WnckApplication: %s\n", windump(win));
    return;
  }
  Application *app = get_application_for_wnck_application(wnck_app);
  if (G_UNLIKELY(!app)) {
    verbose("window_suspend[1]: could not get Application: %s\n", windump(win));
    return;
  }

  if (app->state != STATE_UNHANDELED) {
    if (app->suspend_delay == suspend_delay &&
        app->refresh_delay == refresh_delay &&
        app->refresh_duration == refresh_duration)
      return;
    else if (app->timeout_id)
      application_cancel_timeout(app);
  }

  app->suspend_delay = suspend_delay;
  app->refresh_delay = refresh_delay;
  app->refresh_duration = refresh_duration;
  app->state = STATE_IS_SUSPENDING;
  app->timeout_id = G_TIMEOUT_ADD(suspend_delay, suspend_callback, app);
}

/* Resume a process (only if it has previously been stopped by us) and cancel
 * outstanding suspend/refresh jobs */
void window_resume(WnckWindow *win) {
  WnckApplication *wnck_app = wnck_window_get_application(win);
  if (G_UNLIKELY(!wnck_app)) {
    verbose("window_resume[0]: could not get WnckApplication: %s\n", windump(win));
    return;
  }
  Application *app = get_application_for_wnck_application(wnck_app);
  if (G_UNLIKELY(!app)) {
    verbose("window_resume[1]: could not get Application: %s\n", windump(win));
    return;
  }

  if (app->timeout_id)
    application_cancel_timeout(app);

  kill_wnck_application(wnck_app, SIGCONT);
}

/* Restore the process states */
void restore_processes() {
  verbose("Restoring processes ...");
  for (GSList *l = applications; l; l = l->next) {
    Application *app = l->data;
    if (app->state == STATE_UNHANDELED)
      continue;
    if (app->timeout_id)
      application_cancel_timeout(app);
    if (verbosity)
      wnck_application_dump(app->wnck_app);
    kill_wnck_application(app->wnck_app, SIGCONT);
  }
}

static gboolean suspend_callback(Application *app) {
  debug("suspend_callback: %d\n", app->timeout_id);
  kill_wnck_application(app->wnck_app, SIGSTOP);
  app->state = STATE_SUSPENDED;
  if (app->refresh_delay)
    app->timeout_id = G_TIMEOUT_ADD(app->refresh_delay, refresh_callback, app);
  return false;
}

static gboolean refresh_callback(Application *app) {
  debug("refresh_callback: %d\n", app->timeout_id);
  kill_wnck_application(app->wnck_app, SIGCONT);
  app->timeout_id = G_TIMEOUT_ADD(app->refresh_duration, suspend_callback, app);
  return false;
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
    verbose(" `- "); // Tree-style if called from application_apply_rules()
  verbose("%s\n", windump(win));
  statement_get_matched(config, win, &statements);

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
  verbose("[ Application \"%s\" %d ]\n",
      wnck_application_get_name(app), wnck_application_get_pid(app));
  WnckWindow *win = NULL;
  Statement suspend_stmt = {
    .type = STATEMENT_SUSPEND,
    .klass.suspend = { 0 }
  };
  static PointerArray suspend_statements = { 0 };
  suspend_statements.size = 0;

  for (GList *l = wnck_application_get_windows(app); l; l = l->next) {
    win = l->data;
    if (! window_apply_rules(win, &suspend_statements)) {
      // One window didn't match the criteria for suspend, do a resume instead
      verbose("    --> No suspend for: %s\n", windump(win));
      suspend_stmt.type = STATEMENT_RESUME;
    }
  }

  if (suspend_stmt.type == STATEMENT_SUSPEND) {
    // Execute all statements except the suspend statement.
    for (unsigned int i = 0; i < suspend_statements.size; ++i) {
      Statement *statement = (Statement*) suspend_statements.data[i];
      // Choose highest suspend_delay
      suspend_stmt.klass.suspend.suspend_delay = MAX(
          suspend_stmt.klass.suspend.suspend_delay,
          statement->klass.suspend.suspend_delay);
      // Choose lowest refresh_delay (but ignore 0)
      if (! suspend_stmt.klass.suspend.refresh_delay)
        suspend_stmt.klass.suspend.refresh_delay = statement->klass.suspend.refresh_delay;
      else if (statement->klass.suspend.refresh_delay) {
        suspend_stmt.klass.suspend.refresh_delay = MIN(
            suspend_stmt.klass.suspend.refresh_delay,
            statement->klass.suspend.refresh_delay);
      }
      // Choost highest refresh_duration
      suspend_stmt.klass.suspend.refresh_duration = MAX(
          suspend_stmt.klass.suspend.refresh_duration,
          statement->klass.suspend.refresh_duration);
    }
  }

  verbose("    --> Executing on: %s\n", windump(win));
  if (verbosity)
    statement_dump(&suspend_stmt, 8, 0);
  statement_execute(&suspend_stmt, win);
}

/* Apply the configuration rules to all applications of the screen */
static void screen_apply_rules() {
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
    gpointer unused)
{
  window_hook = HOOK_STATE_CHANGED;
  window_apply_rules(window, NULL);
  /* If the window changed its `above` state we have to re-apply the configuration
   * to all windows on the screen, since the `above` state affects our logic
   * of calculating the stackposition. */
  if (changed_mask & WNCK_WINDOW_STATE_ABOVE) {
    window_hook = HOOK_WINDOW_STACKING_CHANGED;
    screen_apply_rules();
  }
}

static inline void window_connect_signals(WnckWindow *win) {
  g_signal_connect(win, "class-changed",     (GCallback) on_win_signal, GUINT_TO_POINTER(HOOK_CLASS_CHANGED));
  g_signal_connect(win, "name-changed",      (GCallback) on_win_signal, GUINT_TO_POINTER(HOOK_NAME_CHANGED));
  g_signal_connect(win, "role-changed",      (GCallback) on_win_signal, GUINT_TO_POINTER(HOOK_ROLE_CHANGED));
  g_signal_connect(win, "type-changed",      (GCallback) on_win_signal, GUINT_TO_POINTER(HOOK_TYPE_CHANGED));
  g_signal_connect(win, "icon-changed",      (GCallback) on_win_signal, GUINT_TO_POINTER(HOOK_ICON_CHANGED));
  g_signal_connect(win, "workspace-changed", (GCallback) on_win_signal, GUINT_TO_POINTER(HOOK_WORKSPACE_CHANGED));
  g_signal_connect(win, "state-changed",     (GCallback) on_window_state_changed, NULL);
  //g_signal_connect(win, "actions_changed", ...)
  //g_signal_connect(win, "geometry_changed", ...)
}

/* ============================================================================
 * Screen Signals
 *
 * Code for connecting screen events.
 * ==========================================================================*/

// === Applications ===
static void on_application_opened(WnckScreen* screen, WnckApplication* wnck_app, gpointer unused) {
  Application *app = calloc(1, sizeof(Application));
  app->wnck_app = wnck_app;
  applications = g_slist_prepend(applications, app);
  log_event("screen::on_application_opened(): added %d %s\n",
      wnck_application_get_pid(wnck_app), wnck_application_get_name(wnck_app));
}

static void on_application_closed(WnckScreen* screen, WnckApplication* wnck_app, gpointer unused) {
  Application *app = get_application_for_wnck_application(wnck_app);
  application_cancel_timeout(app);
  applications = g_slist_remove(applications, app);
  free(app);
  log_event("screen::on_application_closed(): removed %d %s\n",
      wnck_application_get_pid(wnck_app), wnck_application_get_name(wnck_app));
}

// === Windows ===
static void on_window_opened(WnckScreen* screen, WnckWindow* window, gpointer unused) {
  log_event("screen::on_window_opened(): %s\n", windump(window));
  window_connect_signals(window);
  window_hook = HOOK_OPENED;
  window_apply_rules(window, NULL);
  /* on_window_stacking_changed() will call screen_apply_rules() */
}

static void on_window_closed(WnckScreen* screen, WnckWindow* window, gpointer unused) {
  log_event("screen::on_window_closed(): %s\n", windump(window));
  window_hook = HOOK_CLOSED;
  window_apply_rules(window, NULL);
  /* on_window_stacking_changed() will call screen_apply_rules() */
}

// === Window Stacking ===
static void on_window_stacking_changed(WnckScreen* screen, gpointer unused) {
  log_event("screen::on_window_stacking_changed()\n");
  window_hook = HOOK_WINDOW_STACKING_CHANGED;
  screen_apply_rules();
}

// === Workspaces ===
static void on_active_workspace_changed(WnckScreen* screen, WnckWorkspace* prev, gpointer unused) {
  log_event("screen::on_active_workspace_changed()\n");
  window_hook = HOOK_ACTIVE_WORKSPACE_CHANGED;
  screen_apply_rules();
}

// === Misc ===
static void on_showing_desktop_changed(WnckScreen* screen, gpointer unused) {
  log_event("screen::on_showing_desktop_changed()\n");
  window_hook = HOOK_SHOWING_DESKTOP_CHANGED;
  screen_apply_rules();
}

int manager_init() {
  if (! (screen = wnck_screen_get_default()))
    return 0;

  // Applications
  g_signal_connect(screen, "application-opened", (GCallback) on_application_opened, NULL);
  g_signal_connect(screen, "application-closed", (GCallback) on_application_closed, NULL);

  // Windows
  g_signal_connect(screen, "window-opened", (GCallback) on_window_opened, NULL);
  g_signal_connect(screen, "window-closed", (GCallback) on_window_closed, NULL);
  g_signal_connect(screen, "window-stacking-changed", (GCallback) on_window_stacking_changed, NULL);
  g_signal_connect(screen, "active-window-changed", (GCallback) on_window_stacking_changed, NULL);

  // Workspaces
  g_signal_connect(screen, "active-workspace-changed", (GCallback) on_active_workspace_changed, NULL);

  // Misc
  g_signal_connect(screen, "showing-desktop-changed", (GCallback) on_showing_desktop_changed, NULL);

  return 1;
}
