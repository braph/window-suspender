#ifndef _KILL_H
#define _KILL_H

#include <glib.h>

#define PROC_FS "/proc"
#define PROCESS_UPDATE_INTERVAL 30
#define MINIMUM_PROCESS_ID 100 // PIDs below are kernel threads

void kill_children(int, int);
void process_rule_add(const char*, GSList*);

#endif
