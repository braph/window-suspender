#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>
#include <glib.h>
#include <glib-unix.h>
#include "wnck.h"
#include "common.h"
#include "manager.h"
#include "statement.h"

#define USAGE \
  "Usage: %s [-v] [-c CONFIG]\n" \
  "Suspend windows according to configured rules\n" \
  "-f   Run program in foreground\n" \
  "-v   Log verbose\n" \
  "-vv  Log events\n" \
  "-vvv Log signals\n"

Statement *config;
Statement *parse_config(const char*); /* y.tab.c */
static char* find_config_file();

static gboolean on_quit(gpointer loop) {
  g_main_loop_quit((GMainLoop*) loop);
  return true;
}

int main(int argc, char *argv[]) {
  char *config_file = NULL;
  unsigned int foreground = 0;

OPT:
  switch (getopt(argc, argv, "hvfc:")) {
    case 'c':
      config_file = optarg;
      goto OPT;
    case 'f':
      foreground = 1;
      goto OPT;
    case 'v':
      verbosity++;
      goto OPT;
    case 'h':
      printf(USAGE, argv[0]);
      return 0;
    case -1: break;
    default: return 1;
  }

  if (config_file)
    config = parse_config(config_file);
  else if ((config_file = find_config_file())) {
    config = parse_config(config_file);
    free(config_file);
  }
  else {
    printerr("No configuration file found. Exiting\n");
    return 1;
  }

  if (! config)
    return 1;

  if (DEBUG)
    statement_dump(config, 0, true);

  if (! foreground)
    switch (fork()) {
      case -1: perror("fork()");
               return 1;
      case 0:  break;
      default: return 0;
    }

  gdk_init(&argc, &argv);
  GMainLoop *loop = g_main_loop_new(NULL, FALSE);
  g_unix_signal_add_full(0, SIGINT, &on_quit, loop, NULL);
  g_unix_signal_add_full(0, SIGTERM, &on_quit, loop, NULL);
  if (! manager_init()) {
    printerr("Failed. Is X11 running?\n");
    return 1;
  }
  // TODO: Is this useful?
  //wnck_set_default_icon_size(1);
  //wnck_set_default_mini_icon_size(1);
  wnck_set_client_type(WNCK_CLIENT_TYPE_APPLICATION);
  g_main_loop_run(loop);
  // TODO: Destroy the app in the right order
  restore_processes();
  statement_free(config);
  //wnck_shutdown();
  //g_main_loop_unref(loop);
  //g_source_remove(sigint);
  //g_source_remove(sigterm);
  return 0;
}

#define CONFIG_FILE "window-suspender.rc"
char* find_config_file() {
  const int size = 4096;
	char *file = malloc(size);
	const char *home;

  snprintf(file, size, "./" CONFIG_FILE);
  if (! access(file, F_OK))
    return file;

	home = getenv("XDG_CONFIG_HOME");
	if (home && *home) {
		snprintf(file, size, "%s/" CONFIG_FILE, home);
		if (! access(file, F_OK))
      return file;
	}

	home = getenv("HOME");
	if (!home || !*home) {
		struct passwd *pwd = getpwuid(getuid());
		if (pwd)
			home = pwd->pw_dir;
	}

	if (home && *home) {
		snprintf(file, size, "%s/.config/" CONFIG_FILE, home);
		if (! access(file, F_OK))
			return file;

		snprintf(file, size, "%s/." CONFIG_FILE, home);
		if (! access(file, F_OK))
			return file;
	}

  free(file);
  return NULL;
}

