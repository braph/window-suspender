#include "windowtype.h"
#include <string.h>

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

WnckWindowType window_type_from_str(const char *name) {
#define scase(STR, VAL) \
  if (!strcmp(STR, name)) return VAL

  scase("desktop",       WNCK_WINDOW_DESKTOP);
  scase("dialog",        WNCK_WINDOW_DIALOG);
  scase("dock",          WNCK_WINDOW_DOCK);
  scase("menu",          WNCK_WINDOW_MENU);
  scase("normal",        WNCK_WINDOW_NORMAL);
  scase("splashscreen",  WNCK_WINDOW_SPLASHSCREEN);
  scase("toolbar",       WNCK_WINDOW_TOOLBAR);
  scase("utility",       WNCK_WINDOW_UTILITY);
  return -1;

#undef scase
}
