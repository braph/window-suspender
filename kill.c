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

struct proc { int pid, ppid; char *name; } *processes;
unsigned int processes_size;
unsigned int processes_allocated;

/* TODO: This is a bit messy for now, clean this up. */
GSList *process_rules;

void process_rule_add(const char *name, GSList *children) {
  ProcessRule *rule;
  for (GSList *l = process_rules; l; l = l->next) {
    rule = l->data;
    if (!strcmp(rule->name, name)) {
      rule->children = g_slist_concat(rule->children, children);
      return;
    }
  }

  rule = malloc(sizeof(ProcessRule));
  rule->name = strdup(name);
  rule->children = children;
  process_rules = g_slist_prepend(process_rules, rule);
}

/* cat /proc/$PID/stat:
 *  10993 (blabla) S 10415 ...
 *  PID     NAME   S PPID  ...
 */
static int get_ppid_and_name(const char *pid, char **name) {
  char buf[128];
  sprintf(buf, PROC_FS "/%s/stat", pid);
  int fd = open(buf, O_RDONLY);
  if (fd < 0)
    return 0;
  int n = read(fd, buf, sizeof(buf) - 1);
  close(fd);
  if (n <= 0)
    return 0;
  buf[n] = '\0';
  char *name_begin = strchr(buf, '(');
  if (!name_begin)
    return 0;
  name_begin++;
  char *name_end = strrchr(name_begin, ')');
  if (!name_end)
    return 0;
  char *ppid_begin = strpbrk(name_end, "0123456789");
  if (ppid_begin) {
    int ppid = atoi(ppid_begin);
    if (!ppid)
      return 0;
    *name_end = '\0';
    *name = strdup(name_begin);
    return ppid;
  }
  return 0;
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

  if (parent) {
    for (; children; children = children->next) {
      struct proc *child = children->data;
      if (!strcmp(child->name, parent->name))
        goto KILL;
      for (GSList *p = process_rules; p; p = p->next)
        for (GSList *c = ((ProcessRule*)p->data)->children; c; c = c->next)
          if (!strcmp(child->name, c->data))
            goto KILL;
      continue;
KILL: kill(child->pid, sig);
      kill_children(child->pid, sig);
    }
  }

  g_slist_free(children);
}
