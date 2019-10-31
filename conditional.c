#include "conditional.h"
#include "common.h"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define this (*self)

bool conditional_check(Conditional *self, WnckWindow *win) {
  int cmp;
  const char *str;
  switch (this.type) {
  case CONDITIONAL_LOGIC_NOT:
    return!conditional_check(this.klass.logic.a, win);
  case CONDITIONAL_LOGIC_OR:
    return conditional_check(this.klass.logic.a, win) || conditional_check(this.klass.logic.b, win);
  case CONDITIONAL_LOGIC_AND:
    return conditional_check(this.klass.logic.a, win) && conditional_check(this.klass.logic.b, win);
  case CONDITIONAL_STRING_MATCH:
      str = window_get_string(win, this.klass.string_match.field);
      return str && !strcmp(this.klass.string_match.string, str);
  case CONDITIONAL_REGEX_MATCH:
      str = window_get_string(win, this.klass.string_match.field);
      return str && !regexec(&this.klass.regex_match.regex, str, 0, 0, 0);
  case CONDITIONAL_WINDOWTYPE:
    return wnck_window_get_window_type(win) == this.klass.windowtype.type;
  case CONDITIONAL_WINDOWSTATE:
    return wnck_window_get_state(win) & this.klass.windowstate.state;
  case CONDITIONAL_STACKPOSITION:
      cmp = window_get_stackposition(win) - this.klass.stackposition.number;
      switch (this.klass.stackposition.comparison) {
      case COMPARE_EQUAL:         return cmp == 0;
      case COMPARE_UNEQUAL:       return cmp != 0;
      case COMPARE_LESS:          return cmp <  0;
      case COMPARE_LESS_EQUAL:    return cmp <= 0;
      case COMPARE_GREATER:       return cmp >  0;
      case COMPARE_GREATER_EQUAL: return cmp >= 0;
      default: abort();
    }
  default: abort();
  }
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

#define CREATE_SELF_PLUS_SPACE(TYPE, SPACE) \
  Conditional* self = malloc(sizeof(Conditional) + SPACE); \
  this.type = TYPE

#define CREATE_SELF(TYPE) \
  CREATE_SELF_PLUS_SPACE(TYPE, 0)

Conditional* conditional_logic_new(conditional_type type, Conditional *a, Conditional *b) {
  CREATE_SELF(type);
  this.klass.logic.a = a;
  this.klass.logic.b = b;
  return self;
}

Conditional* conditional_string_match_new(WSWindowString field, const char *string) {
  CREATE_SELF_PLUS_SPACE(CONDITIONAL_STRING_MATCH, strlen(string));
  this.klass.string_match.field = field;
  strcpy(this.klass.string_match.string, string);
  return self;
}

Conditional* conditional_regex_match_new(WSWindowString field, const char *string) {
  CREATE_SELF(CONDITIONAL_REGEX_MATCH);
  this.klass.regex_match.field = field;
  if ((errno = regcomp(&this.klass.regex_match.regex, string, 0))) {
    char msg[256];
    regerror(errno, &this.klass.regex_match.regex, msg, sizeof(msg));
    printerr("Invalid Regex: `%s`: %s\n", string, msg);
    free(self);
    return NULL;
  }
  return self;
}

Conditional* conditional_windowtype_new(WnckWindowType type) {
  CREATE_SELF(CONDITIONAL_WINDOWTYPE);
  this.klass.windowtype.type = type;
  return self;
}

Conditional* conditional_windowstate_new(WnckWindowState state) {
  CREATE_SELF(CONDITIONAL_WINDOWSTATE);
  this.klass.windowstate.state = state;
  return self;
}

Conditional* conditional_stackposition_new(comparison_type comparison, unsigned int number) {
  CREATE_SELF(CONDITIONAL_STACKPOSITION);
  this.klass.stackposition.number = number;
  this.klass.stackposition.comparison = comparison;
  return self;
}

#if DEBUG
void conditional_dump(Conditional* self, int indent) {
  #define outf(FMT, ...) \
    printf("%*s" FMT, indent, "", ##__VA_ARGS__) 

  switch (this.type) {
  case CONDITIONAL_LOGIC_NOT:
    outf("<NOT>\n");
    conditional_dump(this.klass.logic.a, indent+4);
    outf("</NOT>\n");
    break;
  case CONDITIONAL_LOGIC_OR:
  case CONDITIONAL_LOGIC_AND:
    outf("<%s>\n", (this.type == CONDITIONAL_LOGIC_OR ? "OR" : "AND"));
    conditional_dump(this.klass.logic.a, indent+4);
    outf("<... %s ...>\n", (this.type == CONDITIONAL_LOGIC_OR ? "OR" : "AND"));
    conditional_dump(this.klass.logic.b, indent+4);
    outf("</%s>\n", (this.type == CONDITIONAL_LOGIC_OR ? "OR" : "AND"));
    break;
  case CONDITIONAL_STRING_MATCH:
    outf("<STRING_MATCH %d %s>\n", this.klass.string_match.field, this.klass.string_match.string);
    break;
  case CONDITIONAL_REGEX_MATCH:
    outf("<REGEX_MATCH %d>\n", this.klass.regex_match.field);
    break;
  case CONDITIONAL_WINDOWTYPE:
    outf("<WINDOW_TYPE %d>\n", this.klass.windowtype.type);
    break;
  case CONDITIONAL_WINDOWSTATE:
    outf("<WINDOW_STATE %d>\n", this.klass.windowstate.state);
    break;
  case CONDITIONAL_STACKPOSITION:
    outf("<WINDOW_STACKPOSITION ");
    switch (this.klass.stackposition.comparison) {
    case COMPARE_EQUAL:         printf("=="); break;
    case COMPARE_UNEQUAL:       printf("!="); break;
    case COMPARE_LESS:          printf("<");  break;
    case COMPARE_LESS_EQUAL:    printf("<="); break;
    case COMPARE_GREATER:       printf(">");  break;
    case COMPARE_GREATER_EQUAL: printf(">="); break;
    }
    printf(" %d>\n", this.klass.stackposition.number);
    break;
  }
}
#endif
