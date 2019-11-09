#include <string.h>
#include "wnck.h"
#include "common.h"

WnckScreen *screen;

/* ============================================================================
 * window_type_to_str()
 * ==========================================================================*/

const char* window_type_to_str(WnckWindowType type) {
  static const char* const window_types[] = {
    [WNCK_WINDOW_DESKTOP]      = "desktop",
    [WNCK_WINDOW_DIALOG]       = "dialog",
    [WNCK_WINDOW_DOCK]         = "dock",
    [WNCK_WINDOW_MENU]         = "menu",
    [WNCK_WINDOW_NORMAL]       = "normal",
    [WNCK_WINDOW_SPLASHSCREEN] = "splashscreen",
    [WNCK_WINDOW_TOOLBAR]      = "toolbar",
    [WNCK_WINDOW_UTILITY]      = "utility",
  };

  if (type < ARRAY_SIZE(window_types) && window_types[type])
    return window_types[type];
  return "UNKNOWN";
}

/* ============================================================================
 * window_state_to_str() / window_states_to_str()
 * ==========================================================================*/

#define S(STATE, STR) \
  [GET_BIT_POSITION_OF_FLAG(WNCK_WINDOW_STATE_ ## STATE)-1] = STR

static const char* const window_states[WNCK_WINDOW_STATE_HIGHEST_BIT] = {
  S(MINIMIZED,              "minimized"),
  S(MAXIMIZED_HORIZONTALLY, "maximized_horizontally"),
  S(MAXIMIZED_VERTICALLY,   "maximized_vertically"),
  S(SHADED,                 "shaded"),
  S(SKIP_PAGER,             "skip_pager"),
  S(SKIP_TASKLIST,          "skip_tasklist"),
  S(STICKY,                 "sticky"),
  S(HIDDEN,                 "hidden"),
  S(FULLSCREEN,             "fullscreen"),
  S(DEMANDS_ATTENTION,      "demands_attention"),
  S(URGENT,                 "urgent"),
  S(ABOVE,                  "above"),
  S(BELOW,                  "below"),
  S(INACTIVE_WORKSPACE,     "inactive_workspace"),
};

const char* window_state_to_str(WnckWindowState state) {
  for (unsigned int i = 0; i < WNCK_WINDOW_STATE_HIGHEST_BIT; ++i)
    if ((1<<i) == state)
      if (window_states[i])
        return window_states[i];
  return "UNKNOWN";
}

const char *window_states_to_str(WnckWindowState state) {
  static char str[192];

  str[0] = '\0';
  str[1] = '\0';
  for (unsigned int i = 0; state && i < WNCK_WINDOW_STATE_HIGHEST_BIT; ++i) {
    if (state & (1 << i)) {
      state |= (1 << i);
      if (window_states[i]) {
        strcat(str, ",");
        strcat(str, window_states[i]);
      }
    }
  }

  return str + 1;
}

/* ============================================================================
 * window_get_state()
 * ==========================================================================*/

#undef wnck_window_get_state /* enable this funtion */

WnckWindowState window_get_state(WnckWindow *win) {
  unsigned int state = wnck_window_get_state(win);
  WnckWorkspace
    *cur_workspace = wnck_screen_get_active_workspace(screen),
    *win_workspace = wnck_window_get_workspace(win);

  if (cur_workspace && win_workspace && win_workspace != cur_workspace)
    return state | WNCK_WINDOW_STATE_INACTIVE_WORKSPACE;
  else
    return state;
}

#define wnck_window_get_state(...) USE_WINDOW_GET_STATE_INSTEAD

/* ============================================================================
 * window_get_stackposition()
 * ==========================================================================*/

// TODO: Make this a configurable option?
#define option_ignore_above 1

unsigned int window_get_stackposition(WnckWindow *win) {
  unsigned int size = 0;
  unsigned int index = 0;
  WnckWorkspace *workspace;

  workspace = wnck_screen_get_active_workspace(screen);
  if (! workspace)
    workspace = wnck_window_get_workspace(win);

  // We don't know the workspace. The most sane value to return is a
  // stackposition of 0.
  if (! workspace)
    return 0;

  WnckWorkspace *win_workspace;
  for (GList* l = wnck_screen_get_windows_stacked(screen); l; l = l->next) {
    win_workspace = wnck_window_get_workspace((WnckWindow*) l->data);
    if (!win_workspace || workspace == win_workspace) {
      if (option_ignore_above && wnck_window_is_above((WnckWindow*) l->data))
        /* ignore */;
      else
        ++size;
    }
    if (win == l->data)
      index = size;
  }

  return size - index;
}

/* ============================================================================
 * window_get_workspace_number() / window_get_workspace_name()
 * ==========================================================================*/

int window_get_workspace_number(WnckWindow *win) {
  WnckWorkspace *workspace = wnck_window_get_workspace(win);
  return (workspace ? wnck_workspace_get_number(workspace) : -1);
}

const char* window_get_workspace_name(WnckWindow *win) {
  WnckWorkspace *workspace = wnck_window_get_workspace(win);
  return (workspace ? wnck_workspace_get_name(workspace) : "<ALL>");
}

/* ============================================================================
 * windump()
 * ==========================================================================*/

const char* windump(WnckWindow *win) {
  static char buf[512];

  if (G_UNLIKELY(win == NULL))
    return "(window == NULL)";

  const char
    *title = wnck_window_get_name(win),
    *klass = wnck_window_get_class_group_name(win),
    *group = wnck_window_get_class_instance_name(win),
    *type  = window_type_to_str(wnck_window_get_window_type(win)),
    *states = window_states_to_str(window_get_state(win)),
    *workspace = window_get_workspace_name(win);
  int
    pid = wnck_window_get_pid(win),
    stack_pos = window_get_stackposition(win),
    workspace_num = window_get_workspace_number(win),
    title_len = strlen(title);

  if (title_len > 50 + 3 /*...*/)
    snprintf(buf, sizeof(buf), "[ \"%.25s...%s\" CLASS=%s GROUP=%s [%d:%s]{%d} PID=%d TYPE=%s STATES=%s ]",
      title, title + (title_len - 25),
      klass, group, workspace_num, workspace, stack_pos, pid, type, states);
  else
    snprintf(buf, sizeof(buf), "[ \"%s\" CLASS=%s GROUP=%s [%d:%s]{%d} PID=%d TYPE=%s STATES=%s ]",
      title,
      klass, group, workspace_num, workspace, stack_pos, pid, type, states);

  return buf;
}

/* ============================================================================
 * wnck_application_dump()
 * ==========================================================================*/

void wnck_application_dump(WnckApplication *app) {
  printerr("[ Application PID=%d ]\n", wnck_application_get_pid(app));
  for (GList *l = wnck_application_get_windows(app); l; l = l->next)
    printerr(" `- %s\n", windump(l->data));
}

