#include <glib.h>
#include <glib-unix.h>
#include <unistd.h>
#include "wnck.h"
#include "common.h"
#include "manager.h"
#include "statement.h"

#define USAGE \
  "Usage: %s [-c CONFIG]\n" \
  "Suspend windows according to configured rules\n"

Statement *config;
Statement *parse_config(const char*); /* y.tab.c */

static gboolean on_quit(gpointer loop) {
  restore_processes();
  g_main_loop_quit((GMainLoop*) loop);
  return true;
}

static void attach_signal(GMainLoop *loop, int sig) {
  GSource *src = g_unix_signal_source_new(sig);
  g_source_set_callback(src, &on_quit, loop, NULL);
  g_source_attach(src, g_main_loop_get_context(loop));
}

int main(int argc, char *argv[]) {
  gdk_init(&argc, &argv);

  GMainLoop *loop = g_main_loop_new(NULL, FALSE);
  const char *config_file = "test.statements";

OPTS:
  switch (getopt(argc, argv, "hc:")) {
    case 'c':
      config_file = optarg;
      goto OPTS;
    case 'h':
      printf(USAGE, argv[0]);
      return 0;
    case -1: break;
    default: return 1;
  }

  config = parse_config(config_file);
  if (! config) {
    fprintf(stderr, "FOOO!\n");
    return 1;
  }
  if (DEBUG)
    statement_dump(config, 0, true);

  manager_init();
  attach_signal(loop, SIGINT);
  attach_signal(loop, SIGTERM);
  g_main_loop_run(loop);
  return 0;
}
