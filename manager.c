#include <string.h>
#include <stdbool.h>
#include "statement.h"
#include "common.h"
#include "wnck.h"

extern Statement *config;
static GSList *apps;

/* ============================================================================
 * Suspend / Refresh / Resume / Restore
 *
 * Code for sending signals to processes.
 * ==========================================================================*/

typedef struct {
  int pid;
  unsigned int event_id;
  unsigned int refresh_delay;
  unsigned int refresh_duration;
} SuspendJob;

static SuspendJob jobs[100]; // TODO
#define MAXJOBS (sizeof(jobs)/sizeof(*jobs))

static gboolean suspend_callback(SuspendJob *job);
static gboolean refresh_callback(SuspendJob *job);

#define G_TIMEOUT_ADD(T, ...) \
  (T >= 1000 ? g_timeout_add_seconds(T/1000, __VA_ARGS__) : g_timeout_add(T, __VA_ARGS__))

/* Suspend a process and cancel outstanding suspend/refresh jobs */
void suspend_process(int pid, int suspend_delay, int refresh_delay, int refresh_duration) {
  if (!pid) {
    debug("suspend_process: ignoring PID 0");
    return;
  }

  SuspendJob *job, *jobfree = NULL, *jobsame = NULL;

  for (unsigned int i = 0; i < MAXJOBS; ++i) {
    if (jobs[i].pid == pid)
      jobsame = &jobs[i];
    else if (!jobs[i].pid)
      jobfree = &jobs[i];
  }

  job = (jobsame ? jobsame : jobfree);

  if (job->pid && job->event_id)
    g_source_remove(job->event_id);

  job->pid = pid;
  job->refresh_delay = refresh_delay;
  job->refresh_duration = refresh_duration;

  if (suspend_delay)
    job->event_id = G_TIMEOUT_ADD(suspend_delay, (GSourceFunc) suspend_callback, job);
  else {
    job->event_id = 0;
    suspend_callback(job);
  }
}

/* Resume a process (only if it has previously been stopped by us) and cancel
 * outstanding suspend/refresh jobs */
void resume_process(int pid) {
  if (!pid) {
    debug("resume_process: ignoring PID 0");
    return;
  }

  for (unsigned int i = 0; i < MAXJOBS; ++i)
    if (jobs[i].pid == pid) {
      if (jobs[i].event_id)
        g_source_remove(jobs[i].event_id);
      jobs[i].pid = 0;
      kill(pid, SIGCONT);
      break;
    }
}

/* Restore the process states */
void restore_processes() {
  verbose("Restoring processes ...");
  for (unsigned int i = 0; i < MAXJOBS; ++i)
    if (jobs[i].pid) {
      if (jobs[i].event_id)
        g_source_remove(jobs[i].event_id);
      kill(jobs[i].pid, SIGCONT);
      jobs[i].pid = 0;
      break;
    }
}

static gboolean suspend_callback(SuspendJob *job) {
  debug("Sending SIGSTOP to %d\n", job->pid);
  kill(job->pid, SIGSTOP);
  if (job->refresh_delay)
    job->event_id = G_TIMEOUT_ADD(job->refresh_delay, (GSourceFunc) refresh_callback, job);
  else
    job->event_id = 0;
  return false;
}

static gboolean refresh_callback(SuspendJob *job) {
  debug("Sending SIGCONT to %d\n", job->pid);
  kill(job->pid, SIGCONT);
  job->event_id = G_TIMEOUT_ADD(job->refresh_duration, (GSourceFunc) suspend_callback, job);
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
  wnck_application_dump(app);
  WnckWindow *win;
  Statement suspend_stmt = {
    .type = STATEMENT_SUSPEND,
    .klass.suspend = { 0 }
  };
  StatementList statements = { 0 };

  for (GList *l = wnck_application_get_windows(app); l; l = l->next) {
    win = l->data;
    statements.size = 0;
    statement_get_matched(config, win, &statements);
    bool have_suspend = false;

    // Execute all statements except the suspend statement.
    for (unsigned int j = 0; j < statements.size; ++j) {
      if (DEBUG)
        statement_dump(statements.list[j], 0, 0);

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
      suspend_stmt.type = STATEMENT_RESUME;
    }
  }

  statement_execute(&suspend_stmt, win);
  free(statements.list);
}

/* Apply the configuration rules to all applications of the screen */
static void screen_apply_rules() {
  debug("screen::apply_rules()\n");
  for (GSList *l = apps; l; l = l->next)
    application_apply_rules(l->data);
}

/* ============================================================================
 * Window Signals
 *
 * Code for (dis-)connecting window events.
 * ==========================================================================*/

static void on_win_signal(WnckWindow* window, gpointer signame) {
  debug("window::%s(): %s\n", (char*) signame, windump(window));
  screen_apply_rules();
}

static void on_window_state_changed(WnckWindow* window,
    WnckWindowState changed_mask,
    WnckWindowState new_state,
    gpointer self)
{
  debug("window::window-state-changed(): %s", windump(window));
  screen_apply_rules();
}

static void window_connect_signals(WnckWindow *win) {
  g_signal_connect(win, "class-changed",     (GCallback) on_win_signal, "class-changed");
  g_signal_connect(win, "name-changed",      (GCallback) on_win_signal, "name-changed");
  g_signal_connect(win, "role-changed",      (GCallback) on_win_signal, "role-changed");
  g_signal_connect(win, "state-changed",     (GCallback) on_window_state_changed, NULL);
  g_signal_connect(win, "type-changed",      (GCallback) on_win_signal, "type-changed");
  g_signal_connect(win, "workspace-changed", (GCallback) on_win_signal, "workspace-changed");
  //g_signal_connect(win, "actions_changed", ...)
  //g_signal_connect(win, "geometry_changed", ...)
  //g_signal_connect(win, "icon_changed",...)
}

static void window_disconnect_signals(WnckWindow *win) {
  guint i;
  g_signal_parse_name("class-changed", wnck_window_get_type(), &i, NULL, FALSE);
  g_signal_handler_disconnect(win, i);
  g_signal_parse_name("name-changed",  wnck_window_get_type(), &i, NULL, FALSE);
  g_signal_handler_disconnect(win, i);
  g_signal_parse_name("role-changed",  wnck_window_get_type(), &i, NULL, FALSE);
  g_signal_handler_disconnect(win, i);
  g_signal_parse_name("state-changed", wnck_window_get_type(), &i, NULL, FALSE);
  g_signal_handler_disconnect(win, i);
  g_signal_parse_name("type-changed",  wnck_window_get_type(), &i, NULL, FALSE);
  g_signal_handler_disconnect(win, i);
  g_signal_parse_name("workspace-changed", wnck_window_get_type(), &i, NULL, FALSE);
  g_signal_handler_disconnect(win, i);
}

/* ============================================================================
 * Screen Signals
 *
 * Code for dis/connecting screen events.
 * ==========================================================================*/

// === Applications ===
static void on_application_opened(WnckScreen* screen, WnckApplication* app, gpointer unused) {
  apps = g_slist_append(apps, app);
  debug("manager::on_application_opened(): added PID %d\n", wnck_application_get_pid(app));
}

static void on_application_closed(WnckScreen* screen, WnckApplication* app, gpointer unused) {
  apps = g_slist_remove(apps, app);
  debug("manager::on_application_closed(): removed PID %d\n", wnck_application_get_pid(app));
}

// === Windows ===
static void on_window_opened(WnckScreen* screen, WnckWindow* window, gpointer self) {
  debug("manager::on_window_opened(): %s\n", windump(window));
  window_connect_signals(window);
  screen_apply_rules();
}

static void on_window_closed(WnckScreen* screen, WnckWindow* window, gpointer self) {
  debug("manager::on_window_closed(): %s\n", windump(window));
  window_disconnect_signals(window);
  screen_apply_rules();
}

// === Window Stacking ===
static void on_window_stacking_changed(WnckScreen* screen, gpointer self) {
  debug("manager::on_window_stacking_changed()\n");
  screen_apply_rules();
}

#if 0
// Don't need this. (on_window_stacking_changed() did the work already)
static void on_active_window_changed(WnckScreen* screen, WnckWindow* prev, gpointer self) {
  debug("manager::on_active_window_changed()\n");
  screen_apply_rules();
}
#endif

// === Workspaces ===
static void on_active_workspace_changed(WnckScreen* screen, WnckWorkspace* prev, gpointer self) {
  debug("manager::on_active_workspace_changed()\n");
  screen_apply_rules();
}

// === Misc ===
static void on_showing_desktop_changed(WnckScreen* screen, gpointer self) {
  debug("manager::on_showing_desktop_changed()\n");
  screen_apply_rules();
}

void manager_init() {
  screen = wnck_screen_get_default();
  // Applications
  g_signal_connect(screen, "application-opened", (GCallback) on_application_opened, NULL);
  g_signal_connect(screen, "application-closed", (GCallback) on_application_closed, NULL);

  // Windows
  g_signal_connect(screen, "window-opened", (GCallback) on_window_opened, NULL);
  g_signal_connect(screen, "window-closed", (GCallback) on_window_closed, NULL);
  g_signal_connect(screen, "window-stacking-changed", (GCallback) on_window_stacking_changed, NULL);

  // Workspaces
  g_signal_connect(screen, "active-workspace-changed", (GCallback) on_active_workspace_changed, NULL);

  // Misc
  g_signal_connect(screen, "showing-desktop-changed", (GCallback) on_showing_desktop_changed, NULL);
}
