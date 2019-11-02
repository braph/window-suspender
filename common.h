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

#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif

#define GET_BIT_POSITION_OF_FLAG(N) (\
  (N == (1U << 0)  ? 1 : \
  (N == (1U << 1)  ? 2 : \
  (N == (1U << 2)  ? 3 : \
  (N == (1U << 3)  ? 4 : \
  (N == (1U << 4)  ? 5 : \
  (N == (1U << 5)  ? 6 : \
  (N == (1U << 6)  ? 7 : \
  (N == (1U << 7)  ? 8 : \
  (N == (1U << 8)  ? 9 : \
  (N == (1U << 9)  ? 10 : \
  (N == (1U << 10) ? 11 : \
  (N == (1U << 11) ? 12 : \
  (N == (1U << 12) ? 13 : \
  (N == (1U << 13) ? 14 : \
  (N == (1U << 14) ? 15 : \
  (N == (1U << 15) ? 16 : \
  (N == (1U << 16) ? 17 : \
  (N == (1U << 17) ? 18 : \
  (N == (1U << 18) ? 19 : \
  (N == (1U << 19) ? 20 : \
  (N == (1U << 20) ? 21 : \
  (N == (1U << 21) ? 22 : \
  (N == (1U << 22) ? 23 : \
  (N == (1U << 23) ? 24 : \
  (N == (1U << 24) ? 25 : \
  (N == (1U << 25) ? 26 : \
  (N == (1U << 26) ? 27 : \
  (N == (1U << 27) ? 28 : \
  (N == (1U << 28) ? 29 : \
  (N == (1U << 29) ? 30 : \
  (N == (1U << 30) ? 31 : \
  (N == (1U << 31) ? 32 : \
   0)))))))))))))))))))))))))))))))))

#endif
