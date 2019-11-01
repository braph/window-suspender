#ifndef _MANAGER_H
#define _MANAGER_H

#include "wnck.h"

void manager_init();
void window_resume(WnckWindow*);
void window_suspend(WnckWindow*, int, int, int);
void restore_processes();

#endif
