#include <string.h>
#include "wnck.h"
#include "common.h"

WnckScreen *screen;

/* ============================================================================
 * window_type_to_str()
 * ==========================================================================*/

const char* window_type_to_str(WnckWindowType type) {
  switch (type) {
    case WNCK_WINDOW_DESKTOP:       return "desktop";
    case WNCK_WINDOW_DIALOG:        return "dialog";
    case WNCK_WINDOW_DOCK:          return "dock";
    case WNCK_WINDOW_MENU:          return "menu";
    case WNCK_WINDOW_NORMAL:        return "normal";
    case WNCK_WINDOW_SPLASHSCREEN:  return "splashscreen";
    case WNCK_WINDOW_TOOLBAR:       return "toolbar";
    case WNCK_WINDOW_UTILITY:       return "utility";
    default:                        return NULL;
  }
}

/* ============================================================================
 * window_state_to_str() / window_states_to_str()
 * ==========================================================================*/

const char* window_state_to_str(WnckWindowState state) {
  switch (state) {
    case WNCK_WINDOW_STATE_MINIMIZED:               return "minimized";
    case WNCK_WINDOW_STATE_MAXIMIZED_HORIZONTALLY:  return "maximized_horizontally";
    case WNCK_WINDOW_STATE_MAXIMIZED_VERTICALLY:    return "maximized_vertically";
    case WNCK_WINDOW_STATE_SHADED:                  return "shaded";
    case WNCK_WINDOW_STATE_SKIP_PAGER:              return "skip_pager";
    case WNCK_WINDOW_STATE_SKIP_TASKLIST:           return "skip_tasklist";
    case WNCK_WINDOW_STATE_STICKY:                  return "sticky";
    case WNCK_WINDOW_STATE_HIDDEN:                  return "hidden";
    case WNCK_WINDOW_STATE_FULLSCREEN:              return "fullscreen";
    case WNCK_WINDOW_STATE_DEMANDS_ATTENTION:       return "demands_attention";
    case WNCK_WINDOW_STATE_ABOVE:                   return "above";
    case WNCK_WINDOW_STATE_BELOW:                   return "below";
    case WNCK_WINDOW_STATE_INACTIVE_WORKSPACE:      return "inactive_workspace";
    default:                                        return NULL;
  }
}

const char *window_states_to_str(WnckWindowState state) {
  static char str[256];
  const char *tmp;

  if (!state)
    return "";
  else if ((tmp = window_state_to_str(state)))
    return tmp;

  str[0] = '\0';
  for (unsigned int i = 0; state && i <= WNCK_WINDOW_STATE_MAX; ++i) {
    if (state & (1 << i)) {
      state |= (1 << i);
      if ((tmp = window_state_to_str(1 << i))) {
        if (str[0])
          strcat(str, ",");
        strcat(str, tmp);
      }
    }
  }

  return str;
}

/* ============================================================================
 * window_get_stackposition()
 * ==========================================================================*/

// TODO: Make this a configurable option?
#define option_ignore_above 1

unsigned int window_get_stackposition(WnckWindow *win) {
  unsigned int size = 0;
  unsigned int index = 0;
  for (GList* l = wnck_screen_get_windows_stacked(screen); l; l = l->next) {
    if (option_ignore_above && wnck_window_is_above((WnckWindow*) l->data))
      /* ignore */;
    else
      ++size;
    if (win == l->data)
      index = size;
  }

  return size - index;
}

/* ============================================================================
 * windump()
 * ==========================================================================*/

#define MAXLEN 50
const char* windump(WnckWindow *win) {
    static char buf[1024];

    if (win == NULL)
      return "(window == NULL)";

    const char
      *title = wnck_window_get_name(win),
      *klass = wnck_window_get_class_group_name(win),
      *group = wnck_window_get_class_instance_name(win),
      *type  = window_type_to_str(wnck_window_get_window_type(win)),
      *states = window_states_to_str(wnck_window_get_state(win));
    int
      pid = wnck_window_get_pid(win),
      number = 0, // TODO: desktop number
      stack_pos = window_get_stackposition(win),
      title_len = strlen(title);

    char *title_truncated = strdup(title);
    if (title_len > MAXLEN) {
      title_truncated[MAXLEN/2 + 0] = '.';
      title_truncated[MAXLEN/2 + 1] = '.';
      title_truncated[MAXLEN/2 + 2] = '.';
      strcpy(&title_truncated[MAXLEN/2 + 3], title + (title_len - MAXLEN/2 + 3));
    }

    snprintf(buf, sizeof(buf), "\"%s\" CLASS=%s GROUP=%s [%d]{%d} PID=%d TYPE=%s STATES=%s",
      title_truncated, klass, group, number, stack_pos, pid, type, states);

    free(title_truncated);
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

