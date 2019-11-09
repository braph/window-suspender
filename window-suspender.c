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
  "Usage: %s [-vf] [-c CONFIG]\n" \
  "Suspend windows according to configured rules\n" \
  "-f   Run program in foreground\n" \
  "-v   Log verbose\n" \
  "-vv  Log events\n" \
  "-vvv Log signals\n"

Statement *config;
Statement *parse_config(const char*); /* y.tab.c */
static char* find_config_file();

int main(int argc, char **argv) {
  char *config_file = NULL;
  bool foreground = 0;

OPT:
  switch (getopt(argc, argv, "hvfc:")) {
    default:  return 1;
    case 'c': config_file = optarg;
              goto OPT;
    case 'f': foreground = 1;
              goto OPT;
    case 'v': verbosity++;
              goto OPT;
    case 'h': printf(USAGE, *argv);
              return 0;
    case -1:  if (optind != argc)
                return printerr("Too many arguments\n"), 1;
  }

  if (config_file)
    config = parse_config(config_file);
  else if ((config_file = find_config_file()))
    config = parse_config(config_file),
    free(config_file);
  else
    return printerr("No configuration file found. Exiting\n"), 1;

  if (! config)
    return 1;

  if (DEBUG)
    statement_dump(config, 0, true);

  if (! foreground)
    switch (fork()) {
      case -1: return perror("fork()"), 1;
      case 0:  break;
      default: return 0;
    }

  gdk_init(&argc, &argv);
  GMainLoop *l = g_main_loop_new(NULL, FALSE);
  g_unix_signal_add(SIGINT, (GSourceFunc) g_main_loop_quit, l);
  g_unix_signal_add(SIGTERM, (GSourceFunc) g_main_loop_quit, l);
  if (! manager_init())
    return printerr("Failed. Is X11 running?\n"), 1;
  wnck_set_client_type(WNCK_CLIENT_TYPE_APPLICATION);
  g_main_loop_run(l);
  restore_processes();
  return 0;
}

#define CONFIG_FILE "window-suspender.rc"
char* find_config_file() {
  char file[4096];
  const char *home;

  if (!access("./" CONFIG_FILE, F_OK))
    return strdup("./" CONFIG_FILE);

  home = getenv("XDG_CONFIG_HOME");
  if (home && *home) {
    snprintf(file, sizeof(file), "%s/" CONFIG_FILE, home);
    if (!access(file, F_OK))
      goto found;
  }

  home = getenv("HOME");
  if (!home || !*home) {
    struct passwd *pwd = getpwuid(getuid());
    if (pwd)
      home = pwd->pw_dir;
  }

  if (home && *home) {
    snprintf(file, sizeof(file), "%s/.config/" CONFIG_FILE, home);
    if (!access(file, F_OK))
      goto found;

    snprintf(file, sizeof(file), "%s/." CONFIG_FILE, home);
    if (!access(file, F_OK))
      goto found;
  }

  return NULL;
found:
  return strdup(file);
}

