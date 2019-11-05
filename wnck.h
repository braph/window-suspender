#ifndef _WNCK_H
#define _WNCK_H

#define WNCK_I_KNOW_THIS_IS_UNSTABLE
#include <libwnck/libwnck.h>
#include "common.h"

/* ============================================================================
 * LibWnck related code
 * ==========================================================================*/

// Introducing a new window state
#define WNCK_WINDOW_STATE_INACTIVE_WORKSPACE (\
  MAX(WNCK_WINDOW_STATE_MINIMIZED, \
  MAX(WNCK_WINDOW_STATE_MAXIMIZED_HORIZONTALLY, \
  MAX(WNCK_WINDOW_STATE_MAXIMIZED_VERTICALLY, \
  MAX(WNCK_WINDOW_STATE_SHADED, \
  MAX(WNCK_WINDOW_STATE_SKIP_PAGER, \
  MAX(WNCK_WINDOW_STATE_SKIP_TASKLIST, \
  MAX(WNCK_WINDOW_STATE_HIDDEN, \
  MAX(WNCK_WINDOW_STATE_FULLSCREEN, \
  MAX(WNCK_WINDOW_STATE_DEMANDS_ATTENTION, \
  MAX(WNCK_WINDOW_STATE_ABOVE, \
  MAX(WNCK_WINDOW_STATE_BELOW, \
  MAX(WNCK_WINDOW_STATE_STICKY, \
    0)))))))))))) << 1)

#define WNCK_WINDOW_STATE_HIGHEST_BIT \
  GET_BIT_POSITION_OF_FLAG(WNCK_WINDOW_STATE_INACTIVE_WORKSPACE)

extern WnckScreen *screen;

unsigned int window_get_stackposition(WnckWindow*);
int window_get_workspace_number(WnckWindow*);
WnckWindowState window_get_state(WnckWindow*);
const char* window_get_workspace_name(WnckWindow*);
const char* window_type_to_str(WnckWindowType);
const char* window_state_to_str(WnckWindowState);
const char* window_states_to_str(WnckWindowState);
const char* windump(WnckWindow*);
void wnck_application_dump(WnckApplication*);

// Prevent usage of the original wnck command
#define wnck_window_get_state(...) USE_WINDOW_GET_STATE_INSTEAD

#endif
