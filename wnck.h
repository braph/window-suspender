#ifndef _WNCK_H
#define _WNCK_H

#define WNCK_I_KNOW_THIS_IS_UNSTABLE
#include <libwnck/libwnck.h>

/* ============================================================================
 * LibWnck related code
 * ==========================================================================*/

// TODO: wrap wnck_window_get_state() to return INACTIVE_WORKSPACE 
#define WNCK_WINDOW_STATE_INACTIVE_WORKSPACE (1 << 17)
#define WNCK_WINDOW_STATE_MAX 16

extern WnckScreen *screen;

unsigned int window_get_stackposition(WnckWindow*);
const char* window_type_to_str(WnckWindowType);
const char* window_state_to_str(WnckWindowState);
const char* window_states_to_str(WnckWindowState);
const char* windump(WnckWindow*);
void wnck_application_dump(WnckApplication*);

#endif
