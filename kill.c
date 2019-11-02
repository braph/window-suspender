#include "kill.h"

#include <time.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

/* We're caching the PPID of processes we're possibly interested in 
 * since hitting /proc everytime can become very expensive. */

static time_t last_update;
struct { int pid, ppid; } *processes;
unsigned int processes_size;
unsigned int processes_allocated;

static int str_is_digit(const char *str) {
  while (*str)
    if (! isdigit(*str++))
      return 0;
  return 1;
}

// 10993 (blabla) S 10415
// PID     NAME   S PPID
static int get_ppid(const char *pid) {
  char buf[256];
  sprintf(buf, PROC_FS "/%s/stat", pid);
  int stat_fd = open(buf, O_RDONLY);
  if (stat_fd < 0)
    return 0;
  int n = read(stat_fd, buf, sizeof(buf) - 1);
  close(stat_fd);
  if (n <= 0)
    return 0;
  buf[n] = '\0';
  const char *tmp, *ppid_str = buf;
  while ((tmp = strchr(ppid_str, ')')))
    ppid_str = tmp + 1;
  if (buf != ppid_str) {
    ppid_str = strpbrk(ppid_str, "0123456789");
    if (ppid_str)
      return atoi(ppid_str);
  }
  return 0;
}

static void read_processes()
{
  processes_size = 0;

  DIR *procfs = opendir(PROC_FS);
  if (procfs) {
    struct dirent *proc_file;
    while ((proc_file = readdir(procfs))) {
      if (str_is_digit(proc_file->d_name)) {
        int ppid = get_ppid(proc_file->d_name);
        if (!ppid || ppid == 1)
          continue;

        if (processes_size == processes_allocated) {
          processes_allocated += 16;
          processes = realloc(processes, processes_allocated * sizeof(*processes));
        }
        processes[processes_size].pid = atoi(proc_file->d_name);
        processes[processes_size].ppid = ppid;
        ++processes_size;
      }
    }
    closedir(procfs);
  }
}

void kill_children(int pid, int sig)
{
  time_t now = time(NULL);
  if (now - last_update >= PROCESS_UPDATE_INTERVAL) {
    read_processes();
    last_update = now;
  }

  for (unsigned int i = 0; i < processes_size; ++i)
    if (processes[i].ppid == pid)
      kill(processes[i].pid, sig);
}
