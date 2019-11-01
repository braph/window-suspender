#ifndef _X11_STUFF
#define _X11_STUFF

#include <glib.h>
#include <glib-unix.h>

int x11_init(GMainLoop *loop);
void x11_destroy();

#endif
