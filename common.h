#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>

extern int verbosity;

#ifndef DEBUG
#define DEBUG 0
#endif

#define printerr(...) \
  fprintf(stderr, ##__VA_ARGS__)

#define verbose(...) do { \
  if (DEBUG || verbosity) \
    printerr(__VA_ARGS__); \
} while(0)

#if DEBUG

#define debug(...) \
  printerr(__VA_ARGS__)

#else // DEBUG

#define debug(...) \
  ((void)0)

#endif // DEBUG

#endif
