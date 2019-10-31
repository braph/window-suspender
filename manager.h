#ifndef _MANAGER_H
#define _MANAGER_H

void manager_init();
void resume_process(int);
void suspend_process(int, int, int, int);
void restore_processes();

#endif
