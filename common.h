#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>

extern int verbosity;

typedef enum {
  LOG_SILENT = 0,
  LOG_VERBOSE,
  LOG_EVENT,
  LOG_SIGNAL,
} LogLevel;

#ifndef DEBUG
#define DEBUG 0
#endif

#define printerr(...) \
  fprintf(stderr, ##__VA_ARGS__)

#define verbose(...) do { \
  if (DEBUG || verbosity) \
    printerr(__VA_ARGS__); \
} while(0)

#define log_event(...) do { \
  if (verbosity >= LOG_EVENT) printerr(__VA_ARGS__); \
} while(0)

#define log_signal(...) do { \
  if (verbosity >= LOG_SIGNAL) printerr(__VA_ARGS__); \
} while(0)

#if DEBUG

#include <assert.h>
#define debug(...) \
  printerr(__VA_ARGS__)

#else // DEBUG

#define assert(...) \
  ((void)0)
#define debug(...) \
  ((void)0)

#endif // DEBUG

#endif
