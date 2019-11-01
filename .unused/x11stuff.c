#include "x11stuff.h"
#include <stdio.h>


#include <X11/Xlib.h>
Display *display;

static gboolean on_xevent(gpointer loop) {
  printf("FOO\n");
  XEvent e;
  XNextEvent(display, &e);
  printf("LOLOLOLOLOL\n");
  if (e.type == ConfigureNotify) {
  }
  sleep(1);
  return 1;
}

int x11_init(GMainLoop *loop) {
  display = XOpenDisplay(NULL);
  if (!display) {
    fprintf(stderr, "Failed to open display\n");
    return -1;
  }

  int screen_count = ScreenCount(display);
  Window roots[screen_count];

  XSelectInput(display, 0, StructureNotifyMask);
  for (int i = 0; i < screen_count; i++) {
    roots[i] = RootWindow(display, i);
    XSelectInput(display, roots[i], StructureNotifyMask);
  }
  XEvent e;
  XNextEvent(display, &e);
  printf("LOLOLOLOLOL\n");

  int fd = ConnectionNumber(display);
  GSource *src = g_unix_fd_source_new(fd, G_IO_OUT);
  g_source_set_callback(src, &on_xevent, loop, NULL);
  g_source_attach(src, g_main_loop_get_context(loop));
}

#if 0
#include <xcb/xcb_aux.h>
#include <xcb/xcb.h>
xcb_connection_t *xconn;

static gboolean on_xevent(gpointer loop) {
  xcb_generic_event_t *e;
  printf("FOO\n");
  e = xcb_wait_for_event(xconn);
  if (e) {
    printf("ev->response_type = %X\n", e->response_type);
  }
  sleep(1);
  return 1;
}

int x11_init(GMainLoop *loop) {
  int screen_no = 0;
  xcb_screen_t *xscreen;

  xconn = xcb_connect(NULL, &screen_no);
  if (xcb_connection_has_error(xconn)) {
    printf("Error!\n");
    return 1;
  }
  xscreen = xcb_aux_get_screen(xconn, screen_no);
  if (! xscreen) {
    printf("Error getting screen\n");
    return 1;
  }
  unsigned int values[1] = {
    XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT
      | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY
      | XCB_EVENT_MASK_PROPERTY_CHANGE
      | XCB_EVENT_MASK_BUTTON_PRESS
  };

  xcb_generic_error_t *error = xcb_request_check(xconn,
      xcb_change_window_attributes_checked(xconn, xscreen->root,
        XCB_CW_EVENT_MASK, values));
  xcb_flush(xconn);

  //XCB_SELECTION_NOTIFY

  int fd = xcb_get_file_descriptor(xconn);
  GSource *src = g_unix_fd_source_new(fd, G_IO_IN);
  g_source_set_callback(src, &on_xevent, loop, NULL);
  g_source_attach(src, g_main_loop_get_context(loop));

}
void x11_destroy() {
  xcb_disconnect(xconn);
}

#endif
void x11_destroy() {
  return;
}
