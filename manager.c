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

#define G_TIMEOUT_ADD(T, ...) \
  (T >= 1000 ? g_timeout_add_seconds(T/1000, __VA_ARGS__) : g_timeout_add(T, __VA_ARGS__))

static Application* get_application_for_wnck_application(WnckApplication *wnck_app) {
  for (GSList *l = applications; l; l = l->next)
    if (((Application*) l->data)->wnck_app == wnck_app)
      return l->data;
  return NULL;
}

static void kill_wnck_application(WnckApplication* wnck_app, int sig) {
  int pid = wnck_application_get_pid(wnck_app);
  if (! pid) {
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
  if (!wnck_app) {
    verbose("window_suspend[0]: could not get WnckApplication: %s\n", windump(win));
    return;
  }
  Application *app = get_application_for_wnck_application(wnck_app);
  if (!app) {
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

  if (suspend_delay) {
    app->state = STATE_IS_SUSPENDING;
    app->timeout_id = G_TIMEOUT_ADD(suspend_delay, (GSourceFunc) suspend_callback, app);
  }
  else {
    app->timeout_id = 0;
    suspend_callback(app);
  }
}

/* Resume a process (only if it has previously been stopped by us) and cancel
 * outstanding suspend/refresh jobs */
void window_resume(WnckWindow *win) {
  WnckApplication *wnck_app = wnck_window_get_application(win);
  if (! wnck_app) {
    verbose("window_resume[0]: could not get WnckApplication: %s\n", windump(win));
    return;
  }
  Application *app = get_application_for_wnck_application(wnck_app);
  if (!app) {
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
    app->timeout_id = G_TIMEOUT_ADD(app->refresh_delay, (GSourceFunc) refresh_callback, app);
  else
    app->timeout_id = 0;
  return false;
}

static gboolean refresh_callback(Application *app) {
  debug("refresh_callback: %d\n", app->timeout_id);
  kill_wnck_application(app->wnck_app, SIGCONT);
  app->timeout_id = G_TIMEOUT_ADD(app->refresh_duration, (GSourceFunc) suspend_callback, app);
  return false;
}

/* ============================================================================
 * Window Matching
 *
 * Code for matching the windows of an application against the configured
 * conditions and executing the resulting statements.
 * ==========================================================================*/

/* Apply the configured rules to an application and its windows */
static void application_apply_rules(WnckApplication *app) {
  verbose("[ Application \"%s\" %d ]\n",
      wnck_application_get_name(app), wnck_application_get_pid(app));
  WnckWindow *win;
  Statement suspend_stmt = {
    .type = STATEMENT_SUSPEND,
    .klass.suspend = { 0 }
  };
  StatementList statements = { 0 };

  for (GList *l = wnck_application_get_windows(app); l; l = l->next) {
    win = l->data;
    verbose(" `- %s\n", windump(win));
    statements.size = 0;
    statement_get_matched(config, win, &statements);
    bool have_suspend = false;

    // Execute all statements except the suspend statement.
    for (unsigned int j = 0; j < statements.size; ++j) {
      if (statements.list[j]->type == STATEMENT_SUSPEND) {
        have_suspend = true;
        // Choose highest suspend_delay
        suspend_stmt.klass.suspend.suspend_delay = MAX(
            suspend_stmt.klass.suspend.suspend_delay,
            statements.list[j]->klass.suspend.suspend_delay);
        // Choose lowest refresh_delay (but ignore 0)
        if (! suspend_stmt.klass.suspend.refresh_delay)
          suspend_stmt.klass.suspend.refresh_delay = statements.list[j]->klass.suspend.refresh_delay;
        else if (statements.list[j]->klass.suspend.refresh_delay) {
          suspend_stmt.klass.suspend.refresh_delay = MIN(
              suspend_stmt.klass.suspend.refresh_delay,
              statements.list[j]->klass.suspend.refresh_delay);
        }
        // Choost highest refresh_duration
        suspend_stmt.klass.suspend.refresh_duration = MAX(
            suspend_stmt.klass.suspend.refresh_duration,
            statements.list[j]->klass.suspend.refresh_duration);
      }
      else {
        statement_execute(statements.list[j], win);
      }
    }

    if (! have_suspend) {
      // One window didn't match the criteria for suspend, do a resume instead
      verbose("    --> No suspend for: %s\n", windump(win));
      suspend_stmt.type = STATEMENT_RESUME;
    }
  }

  verbose("    --> Executing on: %s\n", windump(win));
  if (verbosity)
    statement_dump(&suspend_stmt, 8, 0);
  statement_execute(&suspend_stmt, win);
  free(statements.list);
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

static void on_win_signal(WnckWindow* window, gpointer signame) {
  log_event("window::%s(): %s\n", (char*) signame, windump(window));
  WnckApplication *wnck_app = wnck_window_get_application(window);
  if (wnck_app)
    application_apply_rules(wnck_app);
}

static void on_window_state_changed(WnckWindow* window,
    WnckWindowState changed_mask,
    WnckWindowState new_state,
    gpointer unused)
{
  // Window may have changed its `above` state, this affects our logic of
  // calculating the stackposition, so we have to apply the configuration to all
  // applications.
  screen_apply_rules();
}

static void window_connect_signals(WnckWindow *win) {
  g_signal_connect(win, "class-changed",     (GCallback) on_win_signal, "class-changed");
  g_signal_connect(win, "name-changed",      (GCallback) on_win_signal, "name-changed");
  g_signal_connect(win, "role-changed",      (GCallback) on_win_signal, "role-changed");
  g_signal_connect(win, "type-changed",      (GCallback) on_win_signal, "type-changed");
  g_signal_connect(win, "icon-changed",      (GCallback) on_win_signal, "icon-changed");
  g_signal_connect(win, "workspace-changed", (GCallback) on_win_signal, "workspace-changed");
  g_signal_connect(win, "state-changed",     (GCallback) on_window_state_changed, NULL);
  //g_signal_connect(win, "actions_changed", ...)
  //g_signal_connect(win, "geometry_changed", ...)
}

#if 0 /* LibWnck disconnects these signals automatically */
static void window_disconnect_signals(WnckWindow *win) {
  guint i;
  g_signal_parse_name("class-changed", wnck_window_get_type(), &i, NULL, FALSE);
  g_signal_handler_disconnect(win, i);
  g_signal_parse_name("name-changed",  wnck_window_get_type(), &i, NULL, FALSE);
  g_signal_handler_disconnect(win, i);
  g_signal_parse_name("role-changed",  wnck_window_get_type(), &i, NULL, FALSE);
  g_signal_handler_disconnect(win, i);
  g_signal_parse_name("type-changed",  wnck_window_get_type(), &i, NULL, FALSE);
  g_signal_handler_disconnect(win, i);
  g_signal_parse_name("icon-changed",  wnck_window_get_type(), &i, NULL, FALSE);
  g_signal_handler_disconnect(win, i);
  g_signal_parse_name("workspace-changed", wnck_window_get_type(), &i, NULL, FALSE);
  g_signal_handler_disconnect(win, i);
  g_signal_parse_name("state-changed", wnck_window_get_type(), &i, NULL, FALSE);
  g_signal_handler_disconnect(win, i);
}
#endif

/* ============================================================================
 * Screen Signals
 *
 * Code for connecting screen events.
 * ==========================================================================*/

// === Applications ===
static void on_application_opened(WnckScreen* screen, WnckApplication* wnck_app, gpointer unused) {
  Application *app = calloc(1, sizeof(Application));
  app->wnck_app = wnck_app;
  applications = g_slist_append(applications, app);
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
}

#if 0
static void on_window_closed(WnckScreen* screen, WnckWindow* window, gpointer unused) {
  log_event("screen::on_window_closed(): %s\n", windump(window));
  window_disconnect_signals(window);
  screen_apply_rules();
}
#endif

// === Window Stacking ===
static void on_window_stacking_changed(WnckScreen* screen, gpointer unused) {
  log_event("screen::on_window_stacking_changed()\n");
  screen_apply_rules();
}

#if 0
static void on_active_window_changed(WnckScreen* screen, WnckWindow* prev, gpointer unused) {
  log_event("screen::on_active_window_changed()\n");
  screen_apply_rules();
}
#endif

// === Workspaces ===
static void on_active_workspace_changed(WnckScreen* screen, WnckWorkspace* prev, gpointer unused) {
  log_event("screen::on_active_workspace_changed()\n");
  screen_apply_rules();
}

// === Misc ===
static void on_showing_desktop_changed(WnckScreen* screen, gpointer unused) {
  log_event("screen::on_showing_desktop_changed()\n");
  screen_apply_rules();
}

void manager_init() {
  screen = wnck_screen_get_default();

  // Applications
  g_signal_connect(screen, "application-opened", (GCallback) on_application_opened, NULL);
  g_signal_connect(screen, "application-closed", (GCallback) on_application_closed, NULL);

  // Windows
  g_signal_connect(screen, "window-opened", (GCallback) on_window_opened, NULL);
  //g_signal_connect(screen, "window-closed", (GCallback) on_window_closed, NULL);
  g_signal_connect(screen, "window-stacking-changed", (GCallback) on_window_stacking_changed, NULL);
  g_signal_connect(screen, "active-window-changed", (GCallback) on_window_stacking_changed, NULL);

  // Workspaces
  g_signal_connect(screen, "active-workspace-changed", (GCallback) on_active_workspace_changed, NULL);

  // Misc
  g_signal_connect(screen, "showing-desktop-changed", (GCallback) on_showing_desktop_changed, NULL);
}
