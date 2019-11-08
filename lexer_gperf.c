/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf -m1000 -n config.gperf.c  */
/* Computed positions: -k'3-4,7,15,$' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 1 "config.gperf.c"
struct __attribute__((packed)) token {
  const char *name;
  unsigned short yylval;
  unsigned char  yytokentype;
};

/* Hand written lexer using gperf for generating the token lookup table */
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "wnck.h"
#include "common.h"
#include "statement.h"
#include "y.tab.h"

// Change order of struct members in initialization and compress `yytokentype`
#define T(YY_TOKEN_TYPE, YY_LVAL) YY_LVAL, YY_TOKEN_TYPE - 255
#define TOKEN_GET_TOKEN_TYPE(TOK) ((TOK)->yytokentype + 255)


// STRING,                   T(TOKEN       , YYLVAL);

#define TOTAL_KEYWORDS 82
#define MIN_WORD_LENGTH 3
#define MAX_WORD_LENGTH 24
#define MIN_HASH_VALUE 0
#define MAX_HASH_VALUE 129
/* maximum key range = 130, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash (register const char *str, register size_t len)
{
  static const unsigned char asso_values[] =
    {
      130, 130, 130, 130, 130, 130, 130, 130, 130, 130,
      130, 130, 130, 130, 130, 130, 130, 130, 130, 130,
      130, 130, 130, 130, 130, 130, 130, 130, 130, 130,
      130, 130, 130, 130, 130, 130, 130, 130, 130, 130,
      130, 130, 130, 130, 130, 130, 130, 130, 130, 130,
      130, 130, 130, 130, 130, 130, 130, 130, 130, 130,
      130, 130, 130, 130, 130, 130, 130, 130, 130, 130,
      130, 130, 130, 130, 130, 130, 130, 130, 130, 130,
      130, 130, 130, 130, 130, 130, 130, 130, 130, 130,
      130, 130, 130, 130, 130,  50, 130,   4,  11,  26,
       17,   1,  18,  19,  49,   3, 130,  16,   7,   6,
        0,  21,  24, 130,  11,   1,   0,  46,  15,  48,
       15,  25,  16, 130, 130, 130, 130, 130
    };
  register unsigned int hval = 0;

  switch (len)
    {
      default:
        hval += asso_values[(unsigned char)str[14]];
      /*FALLTHROUGH*/
      case 14:
      case 13:
      case 12:
      case 11:
      case 10:
      case 9:
      case 8:
      case 7:
        hval += asso_values[(unsigned char)str[6]];
      /*FALLTHROUGH*/
      case 6:
      case 5:
      case 4:
        hval += asso_values[(unsigned char)str[3]];
      /*FALLTHROUGH*/
      case 3:
        hval += asso_values[(unsigned char)str[2]];
        break;
    }
  return hval + asso_values[(unsigned char)str[len - 1]];
}

const struct __attribute__((packed)) token *
in_word_set (register const char *str, register size_t len)
{
  static const struct __attribute__((packed)) token wordlist[] =
    {
#line 49 "config.gperf.c"
      {"pin",                       T(ACTION_TYPE , STATEMENT_PIN)},
#line 104 "config.gperf.c"
      {"contains",                  T(CONTAINS                , 0)},
#line 103 "config.gperf.c"
      {"has",                       T(HAS                     , 0)},
#line 97 "config.gperf.c"
      {"print",                     T(PRINT                   , 0)},
#line 51 "config.gperf.c"
      {"activate",                  T(ACTION_TYPE , STATEMENT_ACTIVATE)},
#line 95 "config.gperf.c"
      {"state",                     T(COND_STATE              , 0)},
#line 52 "config.gperf.c"
      {"activate_transient",        T(ACTION_TYPE , STATEMENT_ACTIVATE_TRANSIENT)},
#line 98 "config.gperf.c"
      {"system",                    T(SYSTEM                  , 0)},
#line 86 "config.gperf.c"
      {"title",                     T(WINDOW_FIELD , WINDOW_TITLE)},
#line 90 "config.gperf.c"
      {"role",                      T(WINDOW_FIELD , WINDOW_ROLE)},
#line 88 "config.gperf.c"
      {"classname",                 T(WINDOW_FIELD , WINDOW_CLASS)},
#line 102 "config.gperf.c"
      {"children",                  T(CHILDREN                , 0)},
#line 83 "config.gperf.c"
      {"splashscreen",              T(WINDOW_TYPE ,  WNCK_WINDOW_SPLASHSCREEN)},
#line 107 "config.gperf.c"
      {"--refresh-duration",        T(OPTION_REFRESH_DURATION , 0)},
#line 73 "config.gperf.c"
      {"demands_attention",         T(WINDOW_STATE , WNCK_WINDOW_STATE_DEMANDS_ATTENTION)},
#line 44 "config.gperf.c"
      {"unset_skip_pager",          T(ACTION_TYPE , STATEMENT_UNSET_SKIP_PAGER)},
#line 28 "config.gperf.c"
      {"unminimize",                T(ACTION_TYPE , STATEMENT_UNMINIMIZE)},
#line 30 "config.gperf.c"
      {"unmaximize",                T(ACTION_TYPE , STATEMENT_UNMAXIMIZE)},
#line 53 "config.gperf.c"
      {"opened",                    T(HOOK_TYPE , HOOK_OPENED)},
#line 46 "config.gperf.c"
      {"unset_skip_tasklist",       T(ACTION_TYPE , STATEMENT_UNSET_SKIP_TASKLIST)},
#line 27 "config.gperf.c"
      {"minimize",                  T(ACTION_TYPE , STATEMENT_MINIMIZE)},
#line 48 "config.gperf.c"
      {"unset_fullscreen",          T(ACTION_TYPE , STATEMENT_UNSET_FULLSCREEN)},
#line 35 "config.gperf.c"
      {"shade",                     T(ACTION_TYPE , STATEMENT_SHADE)},
#line 26 "config.gperf.c"
      {"close",                     T(ACTION_TYPE , STATEMENT_CLOSE)},
#line 82 "config.gperf.c"
      {"normal",                    T(WINDOW_TYPE ,  WNCK_WINDOW_NORMAL)},
#line 72 "config.gperf.c"
      {"fullscreen",                T(WINDOW_STATE , WNCK_WINDOW_STATE_FULLSCREEN)},
#line 93 "config.gperf.c"
      {"type",                      T(COND_TYPE               , 0)},
#line 50 "config.gperf.c"
      {"unpin",                     T(ACTION_TYPE , STATEMENT_UNPIN)},
#line 89 "config.gperf.c"
      {"iconname",                  T(WINDOW_FIELD , WINDOW_ICON_NAME)},
#line 37 "config.gperf.c"
      {"make_above",                T(ACTION_TYPE , STATEMENT_MAKE_ABOVE)},
#line 79 "config.gperf.c"
      {"dialog",                    T(WINDOW_TYPE ,  WNCK_WINDOW_DIALOG)},
#line 70 "config.gperf.c"
      {"skip_tasklist",             T(WINDOW_STATE , WNCK_WINDOW_STATE_SKIP_TASKLIST)},
#line 91 "config.gperf.c"
      {"workspace",                 T(WINDOW_FIELD , WINDOW_WORKSPACE)},
#line 42 "config.gperf.c"
      {"unstick",                   T(ACTION_TYPE , STATEMENT_UNSTICK)},
#line 71 "config.gperf.c"
      {"hidden",                    T(WINDOW_STATE , WNCK_WINDOW_STATE_HIDDEN)},
#line 29 "config.gperf.c"
      {"maximize",                  T(ACTION_TYPE , STATEMENT_MAXIMIZE)},
#line 65 "config.gperf.c"
      {"minimized",                 T(WINDOW_STATE , WNCK_WINDOW_STATE_MINIMIZED)},
#line 74 "config.gperf.c"
      {"above",                     T(WINDOW_STATE , WNCK_WINDOW_STATE_ABOVE)},
#line 68 "config.gperf.c"
      {"shaded",                    T(WINDOW_STATE , WNCK_WINDOW_STATE_SHADED)},
#line 54 "config.gperf.c"
      {"closed",                    T(HOOK_TYPE , HOOK_CLOSED)},
      {""},
#line 34 "config.gperf.c"
      {"unmaximize_vertically",     T(ACTION_TYPE , STATEMENT_UNMAXIMIZE_VERTICALLY)},
#line 69 "config.gperf.c"
      {"skip_pager",                T(WINDOW_STATE , WNCK_WINDOW_STATE_SKIP_PAGER)},
#line 92 "config.gperf.c"
      {"workspace_number",          T(COND_WORKSPACE_NUMBER   , 0)},
#line 32 "config.gperf.c"
      {"unmaximize_horizontally",   T(ACTION_TYPE , STATEMENT_UNMAXIMIZE_HORIZONTALLY)},
#line 41 "config.gperf.c"
      {"stick",                     T(ACTION_TYPE , STATEMENT_STICK)},
#line 99 "config.gperf.c"
      {"return",                    T(RETURN                  , 0)},
#line 61 "config.gperf.c"
      {"state_changed",             T(HOOK_TYPE , HOOK_STATE_CHANGED)},
#line 55 "config.gperf.c"
      {"class_changed",             T(HOOK_TYPE , HOOK_CLASS_CHANGED)},
#line 101 "config.gperf.c"
      {"process",                   T(PROCESS                 , 0)},
#line 84 "config.gperf.c"
      {"toolbar",                   T(WINDOW_TYPE ,  WNCK_WINDOW_TOOLBAR)},
#line 94 "config.gperf.c"
      {"stackposition",             T(COND_STACKPOSITION      , 0)},
#line 36 "config.gperf.c"
      {"unshade",                   T(ACTION_TYPE , STATEMENT_UNSHADE)},
#line 96 "config.gperf.c"
      {"hook",                      T(COND_HOOK               , 0)},
#line 77 "config.gperf.c"
      {"sticky",                    T(WINDOW_STATE , WNCK_WINDOW_STATE_STICKY)},
      {""},
#line 45 "config.gperf.c"
      {"set_skip_tasklist",         T(ACTION_TYPE , STATEMENT_SET_SKIP_TASKLIST)},
#line 47 "config.gperf.c"
      {"set_fullscreen",            T(ACTION_TYPE , STATEMENT_SET_FULLSCREEN)},
#line 80 "config.gperf.c"
      {"dock",                      T(WINDOW_TYPE ,  WNCK_WINDOW_DOCK)},
#line 100 "config.gperf.c"
      {"suspend",                   T(SUSPEND                 , 0)},
#line 85 "config.gperf.c"
      {"utility",                   T(WINDOW_TYPE ,  WNCK_WINDOW_UTILITY)},
#line 38 "config.gperf.c"
      {"unmake_above",              T(ACTION_TYPE , STATEMENT_UNMAKE_ABOVE)},
#line 67 "config.gperf.c"
      {"maximized_vertically",      T(WINDOW_STATE , WNCK_WINDOW_STATE_MAXIMIZED_VERTICALLY)},
#line 106 "config.gperf.c"
      {"--refresh-delay",           T(OPTION_REFRESH_DELAY    , 0)},
#line 43 "config.gperf.c"
      {"set_skip_pager",            T(ACTION_TYPE , STATEMENT_SET_SKIP_PAGER)},
#line 78 "config.gperf.c"
      {"desktop",                   T(WINDOW_TYPE ,  WNCK_WINDOW_DESKTOP)},
#line 39 "config.gperf.c"
      {"make_below",                T(ACTION_TYPE , STATEMENT_MAKE_BELOW)},
#line 60 "config.gperf.c"
      {"workspace_changed",         T(HOOK_TYPE , HOOK_WORKSPACE_CHANGED)},
      {""}, {""},
#line 76 "config.gperf.c"
      {"inactive_workspace",        T(WINDOW_STATE , WNCK_WINDOW_STATE_INACTIVE_WORKSPACE)},
      {""},
#line 87 "config.gperf.c"
      {"groupname",                 T(WINDOW_FIELD , WINDOW_GROUP)},
#line 56 "config.gperf.c"
      {"name_changed",              T(HOOK_TYPE , HOOK_NAME_CHANGED)},
#line 57 "config.gperf.c"
      {"role_changed",              T(HOOK_TYPE , HOOK_ROLE_CHANGED)},
#line 66 "config.gperf.c"
      {"maximized_horizontally",    T(WINDOW_STATE , WNCK_WINDOW_STATE_MAXIMIZED_HORIZONTALLY)},
#line 75 "config.gperf.c"
      {"below",                     T(WINDOW_STATE , WNCK_WINDOW_STATE_BELOW)},
      {""}, {""}, {""},
#line 31 "config.gperf.c"
      {"maximize_horizontally",     T(ACTION_TYPE , STATEMENT_MAXIMIZE_HORIZONTALLY)},
      {""}, {""}, {""}, {""},
#line 33 "config.gperf.c"
      {"maximize_vertically",       T(ACTION_TYPE , STATEMENT_MAXIMIZE_VERTICALLY)},
      {""},
#line 59 "config.gperf.c"
      {"icon_changed",              T(HOOK_TYPE , HOOK_ICON_CHANGED)},
      {""}, {""}, {""},
#line 58 "config.gperf.c"
      {"type_changed",              T(HOOK_TYPE , HOOK_TYPE_CHANGED)},
#line 81 "config.gperf.c"
      {"menu",                      T(WINDOW_TYPE ,  WNCK_WINDOW_MENU)},
      {""}, {""}, {""},
#line 63 "config.gperf.c"
      {"active_workspace_changed",  T(HOOK_TYPE , HOOK_ACTIVE_WORKSPACE_CHANGED)},
      {""},
#line 105 "config.gperf.c"
      {"--suspend-delay",           T(OPTION_SUSPEND_DELAY    , 0)},
      {""}, {""}, {""}, {""},
#line 62 "config.gperf.c"
      {"window_stacking_changed",   T(HOOK_TYPE , HOOK_WINDOW_STACKING_CHANGED)},
      {""}, {""}, {""}, {""},
#line 40 "config.gperf.c"
      {"unmake_below",              T(ACTION_TYPE , STATEMENT_UNMAKE_BELOW)},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 64 "config.gperf.c"
      {"showing_desktop_changed",   T(HOOK_TYPE , HOOK_SHOWING_DESKTOP_CHANGED)}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}
#line 108 "config.gperf.c"


#define LEX_ERROR '\1' // TODO: What does yylex() really return on error?

FILE *yyin;
char *yytext;
unsigned int yyleng;
static unsigned int yyallocated;
unsigned int yylineno = 1;
unsigned int yylinepos = 0;

static int GETC() {
  int c = fgetc(yyin);
  yylinepos++;
  if (c == '\n') {
    yylineno++;
    yylinepos = 0;
  }
  return c;
}

static void UNGETC(int c) {
  yylinepos--;
  if (c == '\n') {
    yylineno--;
    yylinepos = -1; // Well...
  }
  ungetc(c, yyin);
}

static void yytext_append(int c) {
  if (yyleng >= yyallocated) {
    yyallocated += 1024;
    yytext = realloc(yytext, yyallocated);
  }
  yytext[yyleng++] = c;
}

static void yytext_finalize() {
  if (yyleng >= yyallocated) {
    yyallocated += 1024;
    yytext = realloc(yytext, yyallocated);
  }
  yytext[yyleng] = '\0';
}

static int yylex_impl() {
  int c;
  yyleng = 0;
  const struct token *tok;

NEXT:
  c = GETC();
  switch (c) {
    case '\t':
    case '\n':
    case ' ':
      goto NEXT;
    case '#':
      while (c = GETC(), (c != '\n' && c != EOF));
      goto NEXT;
    case '/':
      if ((c = GETC()) != '*')
        return UNGETC(c), '/';

      while ((c = GETC()) != EOF)
        if (c == '*')
WAIT_FOR_TERMINATING_SLASH:
          switch ((c = GETC())) {
            case '*': goto WAIT_FOR_TERMINATING_SLASH;
            case '/': goto NEXT; }

      printerr("Warning: EOF reached while scanning /* */ comment\n");
      return EOF;
    case '&':
      switch ((c = GETC())) {
        case '&':           return AND_AND;
        default: UNGETC(c); return '&'; }
    case '|':
      switch ((c = GETC())) {
        case '|':           return OR_OR;
        default: UNGETC(c); return '|'; }
    case '!':
      switch ((c = GETC())) {
        case '=': yylval.comparison = COMPARE_UNEQUAL; return UNEQUAL;
        case '~':                                      return REGEX_UNEQUAL;
        default: UNGETC(c);                            return NOT; }
    case '=':
      switch ((c = GETC())) {
        case '=': yylval.comparison = COMPARE_EQUAL; return EQUAL;
        case '~':                                    return REGEX_EQUAL;
        default: UNGETC(c);                          return '='; }
    case '<':
      switch ((c = GETC())) {
        case '=': yylval.comparison = COMPARE_LESS_EQUAL;     return LESS_EQUAL;
        default: UNGETC(c); yylval.comparison = COMPARE_LESS; return LESS; }
    case '>':
      switch ((c = GETC())) {
        case '=': yylval.comparison = COMPARE_GREATER_EQUAL;     return GREATER_EQUAL;
        default: UNGETC(c); yylval.comparison = COMPARE_GREATER; return GREATER; }
    case '\'':
      for (;;)
        switch ((c = GETC())) {
          case EOF:  printerr("Warning: EOF reached while scanning single quoted string\n");
          case '\'': return yytext_finalize(), yylval.string = strdup(yytext), STRING;
          default:   yytext_append(c); }
    case '"':
      for (;;)
        switch ((c = GETC())) {
          case EOF:  printerr("Warning: EOF reached while scanning double quoted string\n");
          case '"':  return yytext_finalize(), yylval.string = strdup(yytext), STRING;
          default:   yytext_append(c); }
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
      for (yytext_append(c); isdigit(c = GETC()); yytext_append(c));
      UNGETC(c);
      yytext_finalize();
      return yylval.number = atoi(yytext), NUMBER;
    default:
      if (! (isalpha(c) || c == '-' || c == '_'))
        return c; // Cannot be a keyword

      do {
        yytext_append(c);
      } while (c = GETC(), (isalpha(c) || c == '-' || c == '_'));
      UNGETC(c);
      yytext_finalize();
      //printf("Having keyword: >%s< (strlen(%d) == yyleng(%d))\n", yytext, strlen(yytext), yyleng);

      tok = in_word_set(yytext, yyleng);
      if (tok) {
        // An --option may have a trailing '='
        switch (TOKEN_GET_TOKEN_TYPE(tok)) {
          case OPTION_SUSPEND_DELAY:
          case OPTION_REFRESH_DELAY:
          case OPTION_REFRESH_DURATION:
            if ((c = GETC()) != '=')
              UNGETC(c);
        }
        return yylval.number = tok->yylval, TOKEN_GET_TOKEN_TYPE(tok);
      }
      else {
        printerr("Error: unknown keyword: %.*s\n", yyleng, yytext);
        return LEX_ERROR;
      }
  }

  return c;
}

int yylex() {
  int t = yylex_impl();
#if DEBUG
  switch (t) {
    case NUMBER: printf("Returning NUMBER %d\n", yylval.number); break;
    case STRING: printf("Returning STRING %s\n", yylval.string); break;
    default:     if (t < 255)
                   printf("Returning CHAR %c\n", t);
                 else
                   printf("Returning TOKEN %d\n", t);
  }
#endif
  if (t == EOF && yytext) {
    free(yytext);
    yytext = NULL;
    yyallocated = 0;
  }
  return t;
}
