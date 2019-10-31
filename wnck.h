#ifndef _WNCK_H
#define _WNCK_H

#ifndef WNCK_I_KNOW_THIS_IS_UNSTABLE
#define WNCK_I_KNOW_THIS_IS_UNSTABLE 1
#endif
#include <libwnck/libwnck.h>

/* ============================================================================
 * LibWnck related code
 * ==========================================================================*/

#define WNCK_WINDOW_STATE_INACTIVE_WORKSPACE 11 // TODO

extern WnckScreen *screen;

unsigned int window_get_stackposition(WnckWindow*);
const char* window_type_to_str(WnckWindowType);
const char* window_state_to_str(WnckWindowState);
const char* windump(WnckWindow*);
void wnck_application_dump(WnckApplication*);

#endif
