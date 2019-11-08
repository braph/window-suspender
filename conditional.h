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
  CONDITIONAL_HOOK,
  CONDITIONAL_STACKPOSITION,
  CONDITIONAL_WORKSPACE_NUMBER,
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
  WINDOW_GROUP,
  WINDOW_ROLE,
  WINDOW_ICON_NAME,
  WINDOW_WORKSPACE,
} window_string_id;

typedef enum {
  HOOK_OPENED,
  HOOK_CLOSED,
  HOOK_CLASS_CHANGED,
  HOOK_NAME_CHANGED,
  HOOK_ROLE_CHANGED,
  HOOK_TYPE_CHANGED,
  HOOK_ICON_CHANGED,
  HOOK_WORKSPACE_CHANGED,
  HOOK_STATE_CHANGED,
  HOOK_WINDOW_STACKING_CHANGED,
  HOOK_ACTIVE_WORKSPACE_CHANGED,
  HOOK_SHOWING_DESKTOP_CHANGED,
} hook_type;

// This variable is updated by manager.c and holds information
// about the event type.
extern hook_type window_hook;

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
      comparison_type comparison;
      int number;
    } numeric;

    struct {
      window_string_id field;
      char string[1];
    } string_match;

    struct {
      window_string_id field;
      regex_t regex;
    } regex_match;

    struct {
      WnckWindowType type;
    } windowtype;

    struct {
      WnckWindowState state;
    } windowstate;

    struct {
      hook_type hook;
    } hook;

    struct {
      char string[1];
    } system;

  } klass;
};

// Constructors
Conditional* conditional_logic_new(conditional_type, Conditional*, Conditional*);
#define      conditional_logic_not_new(C)   conditional_logic_new(CONDITIONAL_LOGIC_NOT, C, NULL)
#define      conditional_logic_or_new(A,B)  conditional_logic_new(CONDITIONAL_LOGIC_OR, A, B)
#define      conditional_logic_and_new(A,B) conditional_logic_new(CONDITIONAL_LOGIC_AND, A, B)
Conditional* conditional_string_match_new(window_string_id, const char*);
Conditional* conditional_string_contains_new(window_string_id, const char*);
Conditional* conditional_regex_match_new(window_string_id, const char*);
Conditional* conditional_windowtype_new(WnckWindowType);
Conditional* conditional_windowstate_new(WnckWindowState);
Conditional* conditional_hook_new(hook_type);
Conditional* conditional_stackposition_new(comparison_type, int);
Conditional* conditional_workspace_number_new(comparison_type, int);
Conditional* conditional_system_new(const char*);

// Methods
bool conditional_check(Conditional*, WnckWindow*);
void conditional_dump(Conditional*);

// Destructor
void conditional_free(Conditional*);

// Helpers
const char* hook_to_str(hook_type);
const char* window_string_id_to_str(window_string_id);
const char* window_get_string_by_id(WnckWindow*, window_string_id);

#endif
