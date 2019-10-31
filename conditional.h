#ifndef _CONDITIONAL_H
#define _CONDITIONAL_H

#include <regex.h>
#include <stdint.h>
#include <stdbool.h>
#include "wnck.h"

typedef enum {
  CONDITIONAL_LOGIC_OR,
  CONDITIONAL_LOGIC_AND,
  CONDITIONAL_LOGIC_NOT,
  CONDITIONAL_STRING_MATCH,
  CONDITIONAL_STRING_CONTAINS,
  CONDITIONAL_REGEX_MATCH,
  CONDITIONAL_WINDOWTYPE,
  CONDITIONAL_WINDOWSTATE,
  CONDITIONAL_STACKPOSITION,
  CONDITIONAL_SYSTEM,
} conditional_type;

typedef enum {
  COMPARE_EQUAL,
  COMPARE_UNEQUAL,
  COMPARE_LESS,
  COMPARE_LESS_EQUAL,
  COMPARE_GREATER,
  COMPARE_GREATER_EQUAL,
} comparison_type;

typedef enum {
  WINDOW_TITLE,
  WINDOW_CLASS,
  WINDOW_GROUP
} WSWindowString;

struct Conditional;
typedef struct Conditional Conditional;

struct Conditional {
  conditional_type type;

  union {
    struct {
      Conditional *a;
      Conditional *b;
    } logic;

    struct {
      WSWindowString field;
      char string[1];
    } string_match;

    struct {
      WSWindowString field;
      regex_t regex;
    } regex_match;

    struct {
      WnckWindowType type;
    } windowtype;

    struct {
      WnckWindowState state;
    } windowstate;

    struct {
      comparison_type comparison;
      unsigned int number;
    } stackposition;

    struct {
      char string[1];
    } system;

  } klass;
};

// Constructors
Conditional* conditional_logic_new(conditional_type, Conditional*, Conditional*);
#define      conditional_logic_not_new(C)   conditional_logic_new(CONDITIONAL_LOGIC_NOT, C, NULL)
#define      conditional_logic_and_new(A,B) conditional_logic_new(CONDITIONAL_LOGIC_AND, A, B)
#define      conditional_logic_or_new(A,B)  conditional_logic_new(CONDITIONAL_LOGIC_OR, A, B)
Conditional* conditional_string_match_new(WSWindowString, const char*);
Conditional* conditional_string_contains_new(WSWindowString, const char*);
Conditional* conditional_regex_match_new(WSWindowString, const char*);
Conditional* conditional_windowtype_new(WnckWindowType);
Conditional* conditional_windowstate_new(WnckWindowState);
Conditional* conditional_stackposition_new(comparison_type, unsigned int);
Conditional* conditional_system_new(const char*);

// Methods
bool conditional_check(Conditional*, WnckWindow*);
void conditional_dump(Conditional*, int);

// Destructor
void conditional_free(Conditional*);

// Helpers
#define window_get_string(WIN, FIELD) \
  (FIELD == WINDOW_TITLE ? wnck_window_get_name(WIN) : \
  (FIELD == WINDOW_GROUP ? wnck_window_get_class_instance_name(WIN) : \
  (FIELD == WINDOW_CLASS ? wnck_window_get_class_group_name(WIN) : \
   NULL)))

#endif
