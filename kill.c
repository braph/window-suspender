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

/* We're caching the PPID + name of processes we're possibly interested in
 * since hitting /proc everytime can become very expensive. */

static time_t last_update;

static struct proc { int pid, ppid; char *name; } *processes;
static unsigned int processes_size;
static unsigned int processes_allocated;

/* TODO: This is a bit messy for now, clean this up. */
typedef struct {
  char *name;
  GSList *subprocesses;
} ProcessRule;

static GSList *process_rules;

void process_rule_add(const char *name, GSList *subprocesses) {
  ProcessRule *rule;
  for (GSList *l = process_rules; l; l = l->next) {
    rule = l->data;
    if (!strcmp(rule->name, name)) {
      rule->subprocesses = g_slist_concat(rule->subprocesses, subprocesses);
      return;
    }
  }

  rule = malloc(sizeof(ProcessRule));
  rule->name = strdup(name);
  rule->subprocesses = subprocesses;
  process_rules = g_slist_prepend(process_rules, rule);
}

/* cat /proc/$PID/stat:
 *  10993 (blabla) S 10415 ...
 *  PID     NAME   S PPID  ...
 */
static int get_ppid_and_name(const char *pid, char **name) {
  int fd, n, ppid;
  char buf[256], *name_begin, *name_end, *ppid_begin;
  sprintf(buf, PROC_FS "/%s/stat", pid);
  if ((fd = open(buf, O_RDONLY)) < 0)
    return 0;
  n = read(fd, buf, sizeof(buf) - 1);
  close(fd);
  if (n <= 0)
    return 0;
  buf[n] = '\0';
  if (! (name_begin = strchr(buf, '(')))                return 0;
  if (! (name_end   = strrchr(name_begin, ')')))        return 0;
  if (! (ppid_begin = strpbrk(name_end, "0123456789"))) return 0;
  if (! (ppid       = atoi(ppid_begin)))                return 0;
  *name_end = '\0';
  *name = strdup(name_begin + 1);
  return ppid;
}

static void read_processes()
{
  while (processes_size)
    free(processes[--processes_size].name);

  DIR *dir = opendir(PROC_FS);
  if (dir) {
    struct dirent *file;
    struct proc process;
    while ((file = readdir(dir))) {
      process.pid = atoi(file->d_name);
      if (process.pid < MINIMUM_PROCESS_ID)
        continue;
      process.ppid = get_ppid_and_name(file->d_name, &process.name);
      if (!process.ppid)
        continue;

      if (processes_size == processes_allocated) {
        processes_allocated += 16;
        processes = realloc(processes, processes_allocated * sizeof(*processes));
      }
      processes[processes_size++] = process;
      //printf("Added %5d PPID=%5d %s\n", process.pid, process.ppid, process.name);
    }
    closedir(dir);
  }
}

void kill_children(int pid, int sig)
{
  time_t now = time(NULL);
  if (now - last_update >= PROCESS_UPDATE_INTERVAL) {
    read_processes();
    last_update = now;
  }

  struct proc *parent = NULL;
  GSList *children = NULL;

  for (unsigned int i = 0; i < processes_size; ++i)
    if (processes[i].ppid == pid)
      children = g_slist_prepend(children, &processes[i]);
    else if (processes[i].pid == pid)
      parent = &processes[i];

  if (parent && children) {
    GSList *allowed_subprocesses = NULL;
    for (GSList *p = process_rules; p; p = p->next)
      if (!strcmp(parent->name, ((ProcessRule*) p->data)->name)) {
        allowed_subprocesses = ((ProcessRule*) p->data)->subprocesses;
        break;
      }

    for (; children; children = children->next) {
      struct proc *child = children->data;
      if (!strcmp(child->name, parent->name))
        goto KILL;
      for (GSList *subprocess = allowed_subprocesses; subprocess; subprocess = subprocess->next)
        if (!strcmp(child->name, subprocess->data))
          goto KILL;
      continue;
KILL: kill(child->pid, sig);
      kill_children(child->pid, sig);
    }
  }

  g_slist_free(children);
}
