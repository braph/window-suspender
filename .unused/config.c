#include <err.h>
#include <errno.h>
#include <unistd.h>

#include "parser.h"
#include "common.h"
#include "statements.h"

#define CONFIG_FILE   "window-suspender.conf"
#define PARSING_ERROR -1

IStatement* load_config(const char *file) {
  FILE *fh = fopen(file, "r");
  if (! fh)
    return NULL;//TODO;

  parser_t parser;
  parser_init(&parser, fh);
  IStatement *out = parser_parse(&parser);
  parser_destroy(&parser);
  return out;
}

IStatement* load_config_at(const char *path, const char *file) {
  char newfile[4096];

  snprintf(newfile, sizeof(newfile), "%s/%s", path, file);
  if (access(file, F_OK)) {
    return NULL;
  }

  debug("loading %s/%s", path, file);

  return load_config(newfile);
}

// error on missing files
IStatement* load_default_files() {
  char path[4096];
  char *xdg_home, *home;
  IStatement *ret;

  if ((xdg_home = getenv("XDG_CONFIG_HOME"))) {
    ret = load_config_at(xdg_home, CONFIG_FILE);
    return ret;
  }

  if ((home = getenv("HOME"))) {
    snprintf(path, sizeof(path), "%s/.config", home);
    ret = load_config_at(path, CONFIG_FILE);
    return ret;
  }

  snprintf(path, sizeof(path), ".%s", CONFIG_FILE);
  ret = load_config_at(path, CONFIG_FILE);
  return ret;
}

