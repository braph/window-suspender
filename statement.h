#ifndef _STATEMENT_H
#define _STATEMENT_H

#include <stdbool.h>
#include "wnck.h"
#include "conditional.h"

typedef enum {
  STATEMENT_ROOT = 0, // INTERNAL
  STATEMENT_IF,
  STATEMENT_PRINT,
  STATEMENT_RETURN,
  STATEMENT_SUSPEND,
  STATEMENT_RESUME, // INTERNAL
} statement_type;

struct Statement;
typedef struct Statement Statement;

struct StatementList;
typedef struct StatementList StatementList;

struct Statement {
  statement_type type;
  Statement *next;

  union {
    struct {
      Conditional *conditional;
      Statement *statement;
    } _if;

    struct {
      char string[1];
    } print;

    struct {
      unsigned int suspend_delay;
      unsigned int refresh_delay;
      unsigned int refresh_duration;
    } suspend;

  } klass;
};

// Constructors
Statement* statement_if_new(Conditional*, Statement*);
Statement* statement_return_new();
Statement* statement_print_new(const char *);
Statement* statement_suspend_new(unsigned int, unsigned int, unsigned int);

// Methods
bool statement_get_matched(Statement*, WnckWindow*, StatementList*);
void statement_execute(Statement*, WnckWindow*);
void statement_dump(Statement*, int, bool);

// Destructor
void statement_free(Statement*);

// ============================================================================

// Structure for storing matches
struct StatementList {
  unsigned int size;
  unsigned int allocated;
  Statement **list;
};

#endif
