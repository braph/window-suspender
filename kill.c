#include "kill.h"
#include "common.h"

#include <time.h>
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

/* TODO: This is actually part of our configuration ... */
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
static int get_ppid(const char *pid) {
  int fd, n;
  char buf[128];
  const char *name_end, *ppid_begin;
  sprintf(buf, PROC_FS "/%.16s/stat", pid);
  if ((fd = open(buf, O_RDONLY)) < 0)
    return 0;
  n = read(fd, buf, sizeof(buf) - 1);
  close(fd);
  if (n <= 0)
    return 0;
  buf[n] = '\0';
  if (! (name_end   = strrchr(buf, ')')))               return 0;
  if (! (ppid_begin = strpbrk(name_end, "0123456789"))) return 0;
  return atoi(ppid_begin);
}

static const char* get_name(const char *pid) {
  int fd, n;
  static char buf[256];
  sprintf(buf, PROC_FS "/%.16s/cmdline", pid);
  if ((fd = open(buf, O_RDONLY)) < 0)
    return NULL;
  n = read(fd, buf, sizeof(buf) - 1);
  close(fd);
  if (n <= 0)
    return NULL;
  buf[n] = '\0';
  // WTF? man procfs(5) says arguments should actually be NUL separated ...
  buf[strcspn(buf, " \t\n")] = '\0';
  const char *lastslash = strrchr(buf, '/');
  return (lastslash ? lastslash + 1 : buf);
}

static int proc_cmp_ppid(const void *a, const void *b) {
  return ((struct proc*) a)->ppid - ((struct proc*) b)->ppid;
}

static void read_processes()
{
  processes_size = 0;

  DIR *dir = opendir(PROC_FS);
  if (dir) {
    struct dirent *file;
    while ((file = readdir(dir))) {
      int pid = atoi(file->d_name);
      if (pid < MINIMUM_PROCESS_ID)
        continue;
      int ppid = get_ppid(file->d_name);
      if (!ppid)
        continue;
      const char *name = get_name(file->d_name);
      if (!name /* error */ || !*name /* zombie */)
        continue;

      if (processes_size >= processes_allocated) {
        processes_allocated += 16;
        processes = realloc(processes, processes_allocated * sizeof(*processes));
        for (unsigned int i = processes_allocated - 16; i < processes_allocated; ++i)
          processes[i].name = NULL;
      }

      processes[processes_size].pid = pid;
      processes[processes_size].ppid = ppid;
      // Maybe we're lucky and the string fits into existing memory
      if (processes[processes_size].name && strlen(processes[processes_size].name) >= strlen(name));
      else processes[processes_size].name = realloc(processes[processes_size].name, strlen(name) + 1);
      strcpy(processes[processes_size].name, name);
      ++processes_size;
    }
    closedir(dir);
  }

  qsort(processes, processes_size, sizeof(*processes), proc_cmp_ppid);

  //for (unsigned int i = 0; i < processes_size; ++i)
  //  printf("Process: %5d PPID=%5d [%s]\n", processes[i].pid, processes[i].ppid, processes[i].name);
}

static void kill_children_impl(int pid, int sig)
{
  struct proc *parent = NULL;
  int subproc_start = -1;
  unsigned int i;

  // Find parent process
  for (i = 0; !parent && i < processes_size; ++i)
    if (processes[i].pid == pid)
      parent = &processes[i];
    else if (subproc_start == -1 && processes[i].ppid == pid)
      subproc_start = i; // Found child "by accident"

  // Find beginning of child processes
  for (; subproc_start == -1 && i < processes_size; ++i)
    if (processes[i].ppid == pid)
      subproc_start = i;
    else if (processes[i].ppid > pid)
      break; // Not found (list sorted by PPID)

  if (parent && subproc_start != -1) {
    // Find allowed subprocess names for the parent processs
    GSList *allowed_subprocesses = NULL;
    for (GSList *p = process_rules; p; p = p->next)
      if (!strcmp(parent->name, ((ProcessRule*) p->data)->name)) {
        allowed_subprocesses = ((ProcessRule*) p->data)->subprocesses;
        break;
      }

    for (i = subproc_start; i < processes_size && processes[i].ppid == pid; ++i) {
      struct proc *child = &processes[i];
      if (!strcmp(child->name, parent->name))
        goto KILL; // Child has same name as parent
      else
        for (GSList *subprocess = allowed_subprocesses; subprocess; subprocess = subprocess->next)
          if (!strcmp(child->name, subprocess->data))
            goto KILL;
      continue;
KILL: kill(child->pid, sig);
      kill_children(child->pid, sig);
      //printf("Killed %d with %d\n", child->pid, sig);
    }
  }
}

void kill_children(int pid, int sig)
{
  time_t now = time(NULL);
  if (G_UNLIKELY(now - last_update >= PROCESS_UPDATE_INTERVAL)) {
    read_processes();
    last_update = now;
  }

  kill_children_impl(pid, sig);
}

