#include "statement.h"
#include "manager.h"
#include "system.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define this (*self)

/* Get all statements that *should* be executed and store them in $list.
 * If a "return" statement occurs, return FALSE and stop traversing the
 * statement list. */
bool statement_get_matched(Statement *self, WnckWindow *win, StatementList *list) {
  bool continue_ = true;

  switch (this.type) {
  case STATEMENT_IF:
    if (conditional_check(this.klass._if.conditional, win))
      continue_ = statement_get_matched(this.klass._if.statement, win, list);
    break;
  case STATEMENT_RETURN:
    return false;
  case STATEMENT_NOOP:
    break;
  default:
    if (list->size == list->allocated) {
      list->allocated += 8;
      list->list = realloc(list->list, sizeof(Statement*) * list->allocated);
    }
    list->list[list->size++] = self;
    break;
  }

  if (continue_ && self->next)
    return statement_get_matched(self->next, win, list);
  return continue_;
}

/* Execute a single statement */
void statement_execute(Statement *self, WnckWindow *win) {
  switch (this.type) {
  case STATEMENT_PRINT:
    printf("%s\n", this.klass.print.string);
    break;
  case STATEMENT_SYSTEM:
    system_with_winenv(this.klass.system.string, win);
    break;
  case STATEMENT_SUSPEND:
    window_suspend(win,
       this.klass.suspend.suspend_delay,
       this.klass.suspend.refresh_delay,
       this.klass.suspend.refresh_duration);
    break;
  case STATEMENT_RESUME:
    window_resume(win);
    break;
  default:
    break; /* -Wpedantic */
  }
}

/* Free the statements list */
void statement_free(Statement *self) {
  switch (this.type) {
  case STATEMENT_IF:
    conditional_free(this.klass._if.conditional);
    statement_free(this.klass._if.statement);
    break;
  default:
    break; /* -Wpedantic */
  }
  if (this.next)
    statement_free(this.next);
  free(self);
}

#define CREATE_SELF_PLUS_SPACE(TYPE, SPACE) \
  Statement* self = malloc(sizeof(Statement) + SPACE); \
  this.next = NULL; \
  this.type = TYPE

#define CREATE_SELF(TYPE) \
  CREATE_SELF_PLUS_SPACE(TYPE, 0)

Statement* statement_if_new(Conditional *conditional, Statement *statement) {
  CREATE_SELF(STATEMENT_IF);
  this.klass._if.statement = statement;
  this.klass._if.conditional = conditional;
  return self;
}

Statement* statement_return_new() {
  CREATE_SELF(STATEMENT_RETURN);
  return self;
}

Statement* statement_print_new(const char *string) {
  CREATE_SELF_PLUS_SPACE(STATEMENT_PRINT, strlen(string));
  strcpy(this.klass.print.string, string);
  return self;
}

Statement* statement_system_new(const char *string) {
  CREATE_SELF_PLUS_SPACE(STATEMENT_SYSTEM, strlen(string));
  strcpy(this.klass.system.string, string);
  return self;
}

Statement* statement_suspend_new(unsigned int suspend_delay, unsigned int refresh_delay, unsigned int refresh_duration) {
  CREATE_SELF(STATEMENT_SUSPEND);
  this.klass.suspend.suspend_delay = suspend_delay;
  this.klass.suspend.refresh_delay = refresh_delay;
  this.klass.suspend.refresh_duration = refresh_duration;
  return self;
}

Statement* statement_noop_new() {
  CREATE_SELF(STATEMENT_NOOP);
  return self;
}

#if DEBUG
void statement_dump(Statement *self, int indent, bool with_trailing) {
  #define outf(FMT, ...) \
    printf("%*s" FMT, indent, "", ##__VA_ARGS__) 

  switch (this.type) {
  case STATEMENT_IF:
    outf("(");
    conditional_dump(this.klass._if.conditional);
    printf(")\n");
    outf("{\n");
    statement_dump(this.klass._if.statement, indent+4, with_trailing);
    outf("}\n");
    break;
  case STATEMENT_RETURN:
    outf("return;\n");
    break;
  case STATEMENT_SUSPEND:
    outf("suspend --suspend_delay=%u --refresh_delay=%u --refresh_duration=%u;\n",
        this.klass.suspend.suspend_delay,
        this.klass.suspend.refresh_delay,
        this.klass.suspend.refresh_duration);
    break;
  case STATEMENT_PRINT:
    outf("print \"%s\";\n", this.klass.print.string);
    break;
  case STATEMENT_SYSTEM:
    outf("system \"%s\";\n", this.klass.system.string);
    break;
  case STATEMENT_NOOP:
    outf("NOOP;\n");
    break;
  }
  if (with_trailing && this.next)
    statement_dump(this.next, indent, with_trailing);
}
#endif
