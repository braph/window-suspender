#include "windowstate.h"

#include <string.h>

static const char* _window_state_to_str(WnckWindowState state) {
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

const char *window_state_to_str(WnckWindowState state) {
  static char *str = 0;
  free(str);
  str = 0;

  const char *tmp = _window_state_to_str(state);
  if (tmp)
    return tmp;

  str = malloc(1024);
  for (int i = 0; i <= 16; ++i) {
    if (state & (1 << i) && (tmp = _window_state_to_str(1 << i))) {
      if (str[0])
        strcat(str, ",");
      strcat(str, tmp);
    }
  }

  return str;
}

WnckWindowState window_state_from_string(const char *str) {
#define scase(STR, VAL) \
  if (!strcmp(STR, str)) return VAL

  scase("minimized",               WNCK_WINDOW_STATE_MINIMIZED);
  scase("maximized_horizontally",  WNCK_WINDOW_STATE_MAXIMIZED_HORIZONTALLY);
  scase("maximized_vertically",    WNCK_WINDOW_STATE_MAXIMIZED_VERTICALLY);
  scase("shaded",                  WNCK_WINDOW_STATE_SHADED);
  scase("skip_pager",              WNCK_WINDOW_STATE_SKIP_PAGER);
  scase("skip_tasklist",           WNCK_WINDOW_STATE_SKIP_TASKLIST);
  scase("sticky",                  WNCK_WINDOW_STATE_STICKY);
  scase("hidden",                  WNCK_WINDOW_STATE_HIDDEN);
  scase("fullscreen",              WNCK_WINDOW_STATE_FULLSCREEN);
  scase("demands_attention",       WNCK_WINDOW_STATE_DEMANDS_ATTENTION);
  scase("above",                   WNCK_WINDOW_STATE_ABOVE);
  scase("below",                   WNCK_WINDOW_STATE_BELOW);
  scase("inactive_workspace",      WNCK_WINDOW_STATE_INACTIVE_WORKSPACE);
  return -1;

#undef scase
}
