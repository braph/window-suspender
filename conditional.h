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
} WSWindowString;

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

// Storing this here for now.
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
      WSWindowString field;
      char string[1];
    } string_match;

    struct {
      WSWindowString field;
      regex_t *regex;
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
Conditional* conditional_string_match_new(WSWindowString, const char*);
Conditional* conditional_string_contains_new(WSWindowString, const char*);
Conditional* conditional_regex_match_new(WSWindowString, const char*);
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
#define window_get_string(WIN, FIELD) (\
  (FIELD == WINDOW_TITLE ?     wnck_window_get_name(WIN) : \
  (FIELD == WINDOW_GROUP ?     wnck_window_get_class_instance_name(WIN) : \
  (FIELD == WINDOW_CLASS ?     wnck_window_get_class_group_name(WIN) : \
  (FIELD == WINDOW_ROLE  ?     wnck_window_get_role(WIN) : \
  (FIELD == WINDOW_ICON_NAME ? wnck_window_get_icon_name(WIN) : \
  (FIELD == WINDOW_WORKSPACE ? window_get_workspace_name(WIN) : \
   NULL)))))))

#define window_field_to_string(FIELD) (\
  (FIELD == WINDOW_TITLE ?     "title" : \
  (FIELD == WINDOW_GROUP ?     "groupname" : \
  (FIELD == WINDOW_CLASS ?     "classname" : \
  (FIELD == WINDOW_ROLE  ?     "role" : \
  (FIELD == WINDOW_ICON_NAME ? "iconname" : \
  (FIELD == WINDOW_WORKSPACE ? "workspace" : \
   NULL)))))))

#define hook_to_str(HOOK) (\
  (HOOK == HOOK_OPENED ?          "opened" : \
  (HOOK == HOOK_CLOSED ?          "closed" : \
  (HOOK == HOOK_CLASS_CHANGED ?   "class_changed" : \
  (HOOK == HOOK_NAME_CHANGED ?    "name_changed" : \
  (HOOK == HOOK_ROLE_CHANGED ?    "role_changed" : \
  (HOOK == HOOK_TYPE_CHANGED ?    "type_changed" : \
  (HOOK == HOOK_ICON_CHANGED ?    "icon_changed" : \
  (HOOK == HOOK_WORKSPACE_CHANGED ? "workspace_changed" : \
  (HOOK == HOOK_STATE_CHANGED ?   "state_changed" : \
  (HOOK == HOOK_WINDOW_STACKING_CHANGED ?  "window_stacking_changed" : \
  (HOOK == HOOK_ACTIVE_WORKSPACE_CHANGED ? "active_workspace_changed" : \
  (HOOK == HOOK_SHOWING_DESKTOP_CHANGED  ? "showing_desktop_changed" : \
   NULL)))))))))))))

#endif
