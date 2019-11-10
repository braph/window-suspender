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
#include <glib/gstringchunk.h>

/* We're caching the PPID + name of processes we're possibly interested in
 * since hitting /proc everytime can become very expensive. */
static time_t last_update;
static struct proc { int ppid, pid; const char *name; } *processes;
static unsigned int processes_size;
static unsigned int processes_allocated;

/* We're using GStringChunk for string deduplication, and pointer comparision
 * instead of strcmp */
static GStringChunk *chunks;
#define init_chunks() if (!chunks) chunks = g_string_chunk_new(512)
#define store_string(S) g_string_chunk_insert_const(chunks, S)

/* TODO: This is actually part of our configuration ... */
typedef struct ProcessRule {
  const char *name;
  GSList *children_names;
} ProcessRule;

static GSList *process_rules;

void process_rule_add(const char *name, GSList *children_names) {
  init_chunks();
  name = store_string(name);

  ProcessRule *rule;
  for (GSList *l = process_rules; l; l = l->next) {
    rule = l->data;
    if (rule->name == name) {
      rule->children_names = g_slist_concat(rule->children_names, children_names);
      goto STORE_STRINGS;
    }
  }

  /* Rule has not been found */
  rule = malloc(sizeof(ProcessRule));
  rule->name = name;
  rule->children_names = children_names;
  process_rules = g_slist_prepend(process_rules, rule);

STORE_STRINGS:
  for (GSList *l = rule->children_names; l; l = l->next)
    l->data = store_string(l->data);
}

/* cat /proc/$PID/stat:
 *  10993 (blabla) S 10415 ...
 *  PID     NAME   S PPID  ...
 */
static int get_ppid(const char *pid) {
  int fd;
  char buf[128];
  const char *name_end, *ppid_begin;
  sprintf(buf, PROC_FS "/%.16s/stat", pid);
  if ((fd = open(buf, O_RDONLY)) < 0)
    return 0;
  int n = read(fd, buf, sizeof(buf) - 1);
  close(fd);
  if (n <= 0)
    return 0;
  buf[n] = '\0';
  if (! (name_end   = strrchr(buf, ')')))               return 0;
  if (! (ppid_begin = strpbrk(name_end, "0123456789"))) return 0;
  return atoi(ppid_begin);
}

static const char* get_name(const char *pid) {
  int fd;
  static char buf[128];
  sprintf(buf, PROC_FS "/%.16s/cmdline", pid);
  if ((fd = open(buf, O_RDONLY)) < 0)
    return NULL;
  int n = read(fd, buf, sizeof(buf) - 1);
  close(fd);
  if (n <= 0)
    return NULL;
  buf[n] = '\0';
  // Although procfs(5) says that arguments are separated by NUL I found
  // some processes where they are separated by spaces ...
  buf[strcspn(buf, " \t\n")] = '\0';
  const char *lastslash = strrchr(buf, '/');
  return (lastslash ? lastslash + 1 : buf);
}

static int proc_cmp_ppid(const void *a, const void *b) {
  return ((const struct proc*) a)->ppid - ((const struct proc*) b)->ppid;
}

static void read_processes()
{
  init_chunks();
  processes_size = 0;

  DIR *dir = opendir(PROC_FS);
  if (dir) {
    const struct dirent *file;
    struct proc process;
    while ((file = readdir(dir))) {
      process.pid = atoi(file->d_name);
      if (process.pid < MINIMUM_PROCESS_ID)
        continue;
      process.ppid = get_ppid(file->d_name);
      if (!process.ppid)
        continue;
      process.name = get_name(file->d_name);
      if (!process.name /* error */ || !*process.name /* zombie */)
        continue;
      process.name = store_string(process.name);

      if (processes_size >= processes_allocated) {
        processes_allocated += 16;
        processes = realloc(processes, processes_allocated * sizeof(*processes));
      }

      processes[processes_size++] = process;
    }
    closedir(dir);
  }

  qsort(processes, processes_size, sizeof(*processes), proc_cmp_ppid);

#if 0
  for (unsigned int i = 0; i < processes_size; ++i)
    printf("Process: %5d PPID=%5d [%0X = %s]\n",
        processes[i].pid, processes[i].ppid, processes[i].name, processes[i].name);
#endif
}

static void kill_children_impl(int pid, int sig)
{
  struct proc *parent = NULL;
  int subproc_start = -1;
  unsigned int i;

  /* Start with searching for the first child. Since the list is sorted we can
   * leave this function early if we see a PPID > PID. */
  for (i = 0; i < processes_size; ++i)
    if (processes[i].ppid == pid) {
      subproc_start = i;
      break;
    }
    else if (processes[i].ppid > pid)
      return; // Not found
    else if (processes[i].pid == pid)
      parent = &processes[i];

  if (subproc_start == -1)
    return;

  // Find parent process
  if (!parent)
    for (; i < processes_size; ++i)
      if (processes[i].pid == pid) {
        parent = &processes[i];
        break;
      }

  if (!parent)
    return;

  // Find allowed subprocess names for the parent processs
  GSList *allowed_children_names = NULL;
  for (GSList *p = process_rules; p; p = p->next) {
    const struct ProcessRule *rule = p->data;
    if (parent->name == rule->name) {
      allowed_children_names = rule->children_names;
      break;
    }
  }

  for (i = subproc_start; i < processes_size && processes[i].ppid == pid; ++i) {
    const struct proc *child = &processes[i];
    if (child->name == parent->name)
      goto KILL; // Child has same name as parent
    else
      for (GSList *name = allowed_children_names; name; name = name->next)
        if (child->name == name->data)
          goto KILL;
    continue;
KILL:
    kill(child->pid, sig);
    kill_children(child->pid, sig);
    //printf("Killed %d with %d\n", child->pid, sig);
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

