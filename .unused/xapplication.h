#ifndef _XAPPLICATION_H
#define _XAPPLICATION_H

typedef struct xapplication {
   int pid;
   WnckApplication *app;
   WnckWindow     **windows;
   int              windows_size;
   /*
     private ResumeJob? resumeJob = null;
     private SuspendJob? suspendJob = null;
     private HashTable<Wnck.Window, XWindow> windows;
  */
} xapplication;

xapplication* xapplication_new(int pid, WnckApplication *app);
void          xapplication_apply_rules(xapplication *xapp);
void          xapplication_add_window(xapplication *xapp, WnckWindow *win);
void          xapplication_remove_window(xapplication *xapp, WnckWindow *win);

#endif
