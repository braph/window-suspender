#include "conditional.h"
#include "system.h"
#include "common.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

hook_type window_hook;

#define this (*self)

bool conditional_check(Conditional *self, WnckWindow *win) {
  int cmp;
  const char *str;
  switch (this.type) {
    case CONDITIONAL_LOGIC_NOT:
      return!conditional_check(this.klass.logic.a, win);
    case CONDITIONAL_LOGIC_OR:
      return conditional_check(this.klass.logic.a, win)
          || conditional_check(this.klass.logic.b, win);
    case CONDITIONAL_LOGIC_AND:
      return conditional_check(this.klass.logic.a, win)
          && conditional_check(this.klass.logic.b, win);
    case CONDITIONAL_STRING_MATCH:
      str = window_get_string_by_id(win, this.klass.string_match.field);
      return str && !strcmp(str, this.klass.string_match.string);
    case CONDITIONAL_STRING_CONTAINS:
      str = window_get_string_by_id(win, this.klass.string_match.field);
      return str && strstr(str, this.klass.string_match.string);
    case CONDITIONAL_REGEX_MATCH:
      str = window_get_string_by_id(win, this.klass.regex_match.field);
      return str && !regexec(&this.klass.regex_match.regex, str, 0, 0, 0);
    case CONDITIONAL_WINDOWTYPE:
      return wnck_window_get_window_type(win) == this.klass.windowtype.type;
    case CONDITIONAL_WINDOWSTATE:
      return window_get_state(win) & this.klass.windowstate.state;
    case CONDITIONAL_HOOK:
      return window_hook == this.klass.hook.hook;
    case CONDITIONAL_STACKPOSITION:
      cmp = window_get_stackposition(win);
      goto compare_number;
    case CONDITIONAL_WORKSPACE_NUMBER:
      cmp = window_get_workspace_number(win);
compare_number:
      cmp -= this.klass.numeric.number;
      switch (this.klass.numeric.comparison) {
        case COMPARE_EQUAL:         return cmp == 0;
        case COMPARE_UNEQUAL:       return cmp != 0;
        case COMPARE_LESS:          return cmp <  0;
        case COMPARE_LESS_EQUAL:    return cmp <= 0;
        case COMPARE_GREATER:       return cmp >  0;
        case COMPARE_GREATER_EQUAL: return cmp >= 0;
      }
    case CONDITIONAL_SYSTEM:
      return system_with_window_environment(this.klass.system.string, win);
  }
  assert(!"reached");
  return 0;
}

void conditional_free(Conditional* self) {
  switch (this.type) {
  case CONDITIONAL_LOGIC_OR:
  case CONDITIONAL_LOGIC_AND:
    conditional_free(this.klass.logic.b);
    /*FALLTHROUGH*/
  case CONDITIONAL_LOGIC_NOT:
    conditional_free(this.klass.logic.a);
    break;
  case CONDITIONAL_REGEX_MATCH:
    regfree(&this.klass.regex_match.regex);
    break;
  default:
    break; /* -Wpedantic */
  }
  free(self);
}

#define CREATE_SELF_PLUS_SPACE(TYPE_ENUM, KLASS_STRUCT, SPACE) \
  Conditional* self = malloc(CLASS_SIZE(Conditional, KLASS_STRUCT) + SPACE); \
  this.type = TYPE_ENUM

#define CREATE_SELF(TYPE_ENUM, KLASS_STRUCT) \
  CREATE_SELF_PLUS_SPACE(TYPE_ENUM, KLASS_STRUCT, 0)

Conditional* conditional_logic_new(conditional_type type, Conditional *a, Conditional *b) {
  CREATE_SELF(type, logic);
  this.klass.logic.a = a;
  this.klass.logic.b = b;
  return self;
}

Conditional* conditional_string_match_new(window_string_id field, const char *string) {
  CREATE_SELF_PLUS_SPACE(CONDITIONAL_STRING_MATCH, string_match, strlen(string));
  this.klass.string_match.field = field;
  strcpy(this.klass.string_match.string, string);
  return self;
}

Conditional* conditional_string_contains_new(window_string_id field, const char *string) {
  CREATE_SELF_PLUS_SPACE(CONDITIONAL_STRING_CONTAINS, string_match, strlen(string));
  this.klass.string_match.field = field;
  strcpy(this.klass.string_match.string, string);
  return self;
}

Conditional* conditional_regex_match_new(window_string_id field, const char *string) {
  CREATE_SELF(CONDITIONAL_REGEX_MATCH, regex_match);
  this.klass.regex_match.field = field;
  if ((errno = regcomp(&this.klass.regex_match.regex, string, REG_NOSUB|REG_EXTENDED))) {
    char msg[256];
    regerror(errno, &this.klass.regex_match.regex, msg, sizeof(msg));
    printerr("Invalid Regex: `%s`: %s\n", string, msg);
    //TODO: Error handling...
  }
  return self;
}

Conditional* conditional_windowtype_new(WnckWindowType type) {
  CREATE_SELF(CONDITIONAL_WINDOWTYPE, windowtype);
  this.klass.windowtype.type = type;
  return self;
}

Conditional* conditional_windowstate_new(WnckWindowState state) {
  CREATE_SELF(CONDITIONAL_WINDOWSTATE, windowstate);
  this.klass.windowstate.state = state;
  return self;
}

Conditional* conditional_hook_new(hook_type hook) {
  CREATE_SELF(CONDITIONAL_HOOK, hook);
  this.klass.hook.hook = hook;
  return self;
}

Conditional* conditional_stackposition_new(comparison_type comparison, int number) {
  CREATE_SELF(CONDITIONAL_STACKPOSITION, numeric);
  this.klass.numeric.number = number;
  this.klass.numeric.comparison = comparison;
  return self;
}

Conditional* conditional_workspace_number_new(comparison_type comparison, int number) {
  CREATE_SELF(CONDITIONAL_WORKSPACE_NUMBER, numeric);
  this.klass.numeric.number = number;
  this.klass.numeric.comparison = comparison;
  return self;
}

Conditional* conditional_system_new(const char *string) {
  CREATE_SELF_PLUS_SPACE(CONDITIONAL_SYSTEM, system, strlen(string));
  strcpy(this.klass.system.string, string);
  return self;
}

void conditional_dump(Conditional* self) {
#define o(...) printerr(__VA_ARGS__);
  switch (this.type) {
  case CONDITIONAL_LOGIC_NOT:
    o("!(");
    conditional_dump(this.klass.logic.a);
    o(")");
    break;
  case CONDITIONAL_LOGIC_OR:
  case CONDITIONAL_LOGIC_AND:
    o("(");
    conditional_dump(this.klass.logic.a);
    o(" %s ", (this.type == CONDITIONAL_LOGIC_OR ? "||" : "&&"));
    conditional_dump(this.klass.logic.b);
    o(")");
    break;
  case CONDITIONAL_STRING_MATCH:
    o("%s == %s", window_string_id_to_str(this.klass.string_match.field), this.klass.string_match.string);
    break;
  case CONDITIONAL_STRING_CONTAINS:
    o("%s contains %s", window_string_id_to_str(this.klass.string_match.field), this.klass.string_match.string);
    break;
  case CONDITIONAL_REGEX_MATCH:
    o("%s =~ $regex", window_string_id_to_str(this.klass.regex_match.field));
    break;
  case CONDITIONAL_WINDOWTYPE:
    o("type == %s", window_type_to_str(this.klass.windowtype.type));
    break;
  case CONDITIONAL_WINDOWSTATE:
    o("state == %s", window_state_to_str(this.klass.windowstate.state));
    break;
  case CONDITIONAL_HOOK:
    o("hook == %s", hook_to_str(this.klass.hook.hook));
    break;
  case CONDITIONAL_STACKPOSITION:
  case CONDITIONAL_WORKSPACE_NUMBER:
    o("%s %s %d",
        (this.type == CONDITIONAL_STACKPOSITION
         ? "stackposition"
         : "workspace_number"),
        (const char[6][3]) {
        [COMPARE_EQUAL]         = "==",
        [COMPARE_UNEQUAL]       = "!=",
        [COMPARE_LESS]          = "<",
        [COMPARE_LESS_EQUAL]    = "<=",
        [COMPARE_GREATER]       = ">",
        [COMPARE_GREATER_EQUAL] = ">=" }[this.klass.numeric.comparison],
        this.klass.numeric.number);
    break;
  case CONDITIONAL_SYSTEM:
    o("system \"%s\"", this.klass.system.string);
    break;
  }
}

const char* hook_to_str(hook_type hook) {
  switch (hook) {
    case HOOK_OPENED:                   return "opened";
    case HOOK_CLOSED:                   return "closed";
    case HOOK_CLASS_CHANGED:            return "class_changed";
    case HOOK_NAME_CHANGED:             return "name_changed";
    case HOOK_ROLE_CHANGED:             return "role_changed";
    case HOOK_TYPE_CHANGED:             return "type_changed";
    case HOOK_ICON_CHANGED:             return "icon_changed";
    case HOOK_WORKSPACE_CHANGED:        return "workspace_changed";
    case HOOK_STATE_CHANGED:            return "state_changed";
    case HOOK_WINDOW_STACKING_CHANGED:  return "window_stacking_changed";
    case HOOK_ACTIVE_WORKSPACE_CHANGED: return "active_workspace_changed";
    case HOOK_SHOWING_DESKTOP_CHANGED:  return "showing_desktop_changed";
    case HOOK_ACTIVE_WINDOW_CHANGED:    return "active_window_changed";
    default:                            return NULL;
  }
}

const char* window_string_id_to_str(window_string_id field) {
  switch (field) {
    case WINDOW_TITLE:     return "title";
    case WINDOW_GROUP:     return "groupname";
    case WINDOW_CLASS:     return "classname";
    case WINDOW_ROLE:      return "role";
    case WINDOW_ICON_NAME: return "iconname";
    case WINDOW_WORKSPACE: return "workspace";
    default:               return NULL;
  }
}

const char* window_get_string_by_id(WnckWindow *win, window_string_id field) {
  switch (field) {
    case WINDOW_TITLE:     return wnck_window_get_name(win);
    case WINDOW_GROUP:     return wnck_window_get_class_instance_name(win);
    case WINDOW_CLASS:     return wnck_window_get_class_group_name(win);
    case WINDOW_ROLE :     return wnck_window_get_role(win);
    case WINDOW_ICON_NAME: return wnck_window_get_icon_name(win);
    case WINDOW_WORKSPACE: return window_get_workspace_name(win);
    default:               return NULL;
  }
}
