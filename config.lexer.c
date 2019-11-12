/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf -m1000 -n config.lexer.gperf.c  */
/* Computed positions: -k'3-4,8,15,$' */

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

#line 1 "config.lexer.gperf.c"
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
#include "config.parser.tab.h"

// Change order of struct members in initialization and compress `yytokentype`
#define T(YY_TOKEN_TYPE, YY_LVAL) YY_LVAL, YY_TOKEN_TYPE - 255
#define TOKEN_GET_TOKEN_TYPE(TOK) ((TOK)->yytokentype + 255)

/* XXX Note XXX
 * A unique value that must be distinct from all other YYLVALs in the token
 * table. It is used to tell if a keyword is an option that takes an argument.  */
#define YYLVAL_OPTION_WITH_ARG 0x1555


// STRING,                   T(TOKEN       , YYLVAL);

#define TOTAL_KEYWORDS 98
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 24
#define MIN_HASH_VALUE 1
#define MAX_HASH_VALUE 144
/* maximum key range = 144, duplicates = 0 */

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
      145, 145, 145, 145, 145, 145, 145, 145, 145, 145,
      145, 145, 145, 145, 145, 145, 145, 145, 145, 145,
      145, 145, 145, 145, 145, 145, 145, 145, 145, 145,
      145, 145, 145, 145, 145, 145, 145, 145, 145, 145,
      145, 145, 145, 145, 145, 145, 145, 145, 145, 145,
      145, 145, 145, 145, 145, 145, 145, 145, 145, 145,
      145, 145, 145, 145, 145, 145, 145, 145, 145, 145,
      145, 145, 145, 145, 145, 145, 145, 145, 145, 145,
      145, 145, 145, 145, 145, 145, 145, 145, 145, 145,
      145, 145, 145, 145, 145,  37,  28,   1,  46,   1,
        1,   0,  20,  53,  64,  11,   9,  26,  22,   0,
        8,   2,  19,  26,  23,   4,  10,  78,  56,  22,
       26,  60,   4, 145, 145, 145, 145, 145, 145
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
        hval += asso_values[(unsigned char)str[7]];
      /*FALLTHROUGH*/
      case 7:
      case 6:
      case 5:
      case 4:
        hval += asso_values[(unsigned char)str[3]+1];
      /*FALLTHROUGH*/
      case 3:
        hval += asso_values[(unsigned char)str[2]];
      /*FALLTHROUGH*/
      case 2:
        break;
    }
  return hval + asso_values[(unsigned char)str[len - 1]];
}

const struct __attribute__((packed)) token *
in_word_set (register const char *str, register size_t len)
{
  static const struct __attribute__((packed)) token wordlist[] =
    {
      {""},
#line 40 "config.lexer.gperf.c"
      {"shade",                     T(ACTION_TYPE , STATEMENT_SHADE)},
#line 74 "config.lexer.gperf.c"
      {"shaded",                    T(WINDOW_STATE , WNCK_WINDOW_STATE_SHADED)},
#line 59 "config.lexer.gperf.c"
      {"opened",                    T(HOOK_TYPE , HOOK_OPENED)},
#line 95 "config.lexer.gperf.c"
      {"iconname",                  T(WINDOW_FIELD , WINDOW_ICON_NAME)},
      {""},
#line 65 "config.lexer.gperf.c"
      {"icon_changed",              T(HOOK_TYPE , HOOK_ICON_CHANGED)},
#line 107 "config.lexer.gperf.c"
      {"process",                   T(PROCESS                 , 0)},
#line 109 "config.lexer.gperf.c"
      {"has",                       T(HAS                     , 0)},
#line 77 "config.lexer.gperf.c"
      {"hidden",                    T(WINDOW_STATE , WNCK_WINDOW_STATE_HIDDEN)},
#line 92 "config.lexer.gperf.c"
      {"title",                     T(WINDOW_FIELD , WINDOW_TITLE)},
#line 94 "config.lexer.gperf.c"
      {"classname",                 T(WINDOW_FIELD , WINDOW_CLASS)},
#line 31 "config.lexer.gperf.c"
      {"close",                     T(ACTION_TYPE , STATEMENT_CLOSE)},
#line 60 "config.lexer.gperf.c"
      {"closed",                    T(HOOK_TYPE , HOOK_CLOSED)},
#line 100 "config.lexer.gperf.c"
      {"stackposition",             T(COND_STACKPOSITION      , 0)},
#line 41 "config.lexer.gperf.c"
      {"unshade",                   T(ACTION_TYPE , STATEMENT_UNSHADE)},
#line 54 "config.lexer.gperf.c"
      {"pin",                       T(ACTION_TYPE , STATEMENT_PIN)},
#line 32 "config.lexer.gperf.c"
      {"minimize",                  T(ACTION_TYPE , STATEMENT_MINIMIZE)},
#line 71 "config.lexer.gperf.c"
      {"minimized",                 T(WINDOW_STATE , WNCK_WINDOW_STATE_MINIMIZED)},
#line 56 "config.lexer.gperf.c"
      {"activate",                  T(ACTION_TYPE , STATEMENT_ACTIVATE)},
#line 33 "config.lexer.gperf.c"
      {"unminimize",                T(ACTION_TYPE , STATEMENT_UNMINIMIZE)},
#line 82 "config.lexer.gperf.c"
      {"inactive_workspace",        T(WINDOW_STATE , WNCK_WINDOW_STATE_INACTIVE_WORKSPACE)},
#line 62 "config.lexer.gperf.c"
      {"name_changed",              T(HOOK_TYPE , HOOK_NAME_CHANGED)},
#line 103 "config.lexer.gperf.c"
      {"print",                     T(PRINT                   , 0)},
#line 80 "config.lexer.gperf.c"
      {"above",                     T(WINDOW_STATE , WNCK_WINDOW_STATE_ABOVE)},
#line 90 "config.lexer.gperf.c"
      {"toolbar",                   T(WINDOW_TYPE , WNCK_WINDOW_TOOLBAR)},
#line 114 "config.lexer.gperf.c"
      {"-x",                        T(OPTION_X                , YYLVAL_OPTION_WITH_ARG)},
#line 108 "config.lexer.gperf.c"
      {"children",                  T(CHILDREN                , 0)},
      {""}, {""},
#line 78 "config.lexer.gperf.c"
      {"fullscreen",                T(WINDOW_STATE , WNCK_WINDOW_STATE_FULLSCREEN)},
#line 106 "config.lexer.gperf.c"
      {"suspend",                   T(SUSPEND                 , 0)},
      {""},
#line 127 "config.lexer.gperf.c"
      {"--southeast",               T(GRAVITY , WNCK_WINDOW_GRAVITY_SOUTHEAST)},
      {""},
#line 34 "config.lexer.gperf.c"
      {"maximize",                  T(ACTION_TYPE , STATEMENT_MAXIMIZE)},
#line 55 "config.lexer.gperf.c"
      {"unpin",                     T(ACTION_TYPE , STATEMENT_UNPIN)},
#line 121 "config.lexer.gperf.c"
      {"--northeast",               T(GRAVITY , WNCK_WINDOW_GRAVITY_NORTHEAST)},
#line 46 "config.lexer.gperf.c"
      {"stick",                     T(ACTION_TYPE , STATEMENT_STICK)},
#line 99 "config.lexer.gperf.c"
      {"type",                      T(COND_TYPE               , 0)},
#line 57 "config.lexer.gperf.c"
      {"activate_transient",        T(ACTION_TYPE , STATEMENT_ACTIVATE_TRANSIENT)},
#line 64 "config.lexer.gperf.c"
      {"type_changed",              T(HOOK_TYPE , HOOK_TYPE_CHANGED)},
#line 96 "config.lexer.gperf.c"
      {"role",                      T(WINDOW_FIELD , WINDOW_ROLE)},
#line 69 "config.lexer.gperf.c"
      {"active_workspace_changed",  T(HOOK_TYPE , HOOK_ACTIVE_WORKSPACE_CHANGED)},
#line 63 "config.lexer.gperf.c"
      {"role_changed",              T(HOOK_TYPE , HOOK_ROLE_CHANGED)},
#line 84 "config.lexer.gperf.c"
      {"desktop",                   T(WINDOW_TYPE , WNCK_WINDOW_DESKTOP)},
#line 97 "config.lexer.gperf.c"
      {"workspace",                 T(WINDOW_FIELD , WINDOW_WORKSPACE)},
#line 43 "config.lexer.gperf.c"
      {"unmake_above",              T(ACTION_TYPE , STATEMENT_UNMAKE_ABOVE)},
#line 42 "config.lexer.gperf.c"
      {"make_above",                T(ACTION_TYPE , STATEMENT_MAKE_ABOVE)},
#line 86 "config.lexer.gperf.c"
      {"dock",                      T(WINDOW_TYPE , WNCK_WINDOW_DOCK)},
#line 102 "config.lexer.gperf.c"
      {"hook",                      T(COND_HOOK               , 0)},
#line 76 "config.lexer.gperf.c"
      {"skip_tasklist",             T(WINDOW_STATE , WNCK_WINDOW_STATE_SKIP_TASKLIST)},
#line 122 "config.lexer.gperf.c"
      {"--west",                    T(GRAVITY , WNCK_WINDOW_GRAVITY_WEST)},
#line 88 "config.lexer.gperf.c"
      {"normal",                    T(WINDOW_TYPE , WNCK_WINDOW_NORMAL)},
#line 85 "config.lexer.gperf.c"
      {"dialog",                    T(WINDOW_TYPE , WNCK_WINDOW_DIALOG)},
#line 125 "config.lexer.gperf.c"
      {"--southwest",               T(GRAVITY , WNCK_WINDOW_GRAVITY_SOUTHWEST)},
#line 124 "config.lexer.gperf.c"
      {"--east",                    T(GRAVITY , WNCK_WINDOW_GRAVITY_EAST)},
#line 35 "config.lexer.gperf.c"
      {"unmaximize",                T(ACTION_TYPE , STATEMENT_UNMAXIMIZE)},
#line 93 "config.lexer.gperf.c"
      {"groupname",                 T(WINDOW_FIELD , WINDOW_GROUP)},
#line 119 "config.lexer.gperf.c"
      {"--northwest",               T(GRAVITY , WNCK_WINDOW_GRAVITY_NORTHWEST)},
#line 115 "config.lexer.gperf.c"
      {"-y",                        T(OPTION_Y                , YYLVAL_OPTION_WITH_ARG)},
      {""}, {""},
#line 81 "config.lexer.gperf.c"
      {"below",                     T(WINDOW_STATE , WNCK_WINDOW_STATE_BELOW)},
      {""},
#line 113 "config.lexer.gperf.c"
      {"--refresh-duration",        T(OPTION_REFRESH_DURATION , YYLVAL_OPTION_WITH_ARG)},
#line 68 "config.lexer.gperf.c"
      {"window_stacking_changed",   T(HOOK_TYPE , HOOK_WINDOW_STACKING_CHANGED)},
#line 123 "config.lexer.gperf.c"
      {"--center",                  T(GRAVITY , WNCK_WINDOW_GRAVITY_CENTER)},
#line 52 "config.lexer.gperf.c"
      {"set_fullscreen",            T(ACTION_TYPE , STATEMENT_SET_FULLSCREEN)},
#line 98 "config.lexer.gperf.c"
      {"workspace_number",          T(COND_WORKSPACE_NUMBER   , 0)},
      {""},
#line 91 "config.lexer.gperf.c"
      {"utility",                   T(WINDOW_TYPE , WNCK_WINDOW_UTILITY)},
#line 83 "config.lexer.gperf.c"
      {"sticky",                    T(WINDOW_STATE , WNCK_WINDOW_STATE_STICKY)},
#line 49 "config.lexer.gperf.c"
      {"unset_skip_pager",          T(ACTION_TYPE , STATEMENT_UNSET_SKIP_PAGER)},
#line 105 "config.lexer.gperf.c"
      {"return",                    T(RETURN                  , 0)},
#line 118 "config.lexer.gperf.c"
      {"--current",                 T(GRAVITY , WNCK_WINDOW_GRAVITY_CURRENT)},
#line 61 "config.lexer.gperf.c"
      {"class_changed",             T(HOOK_TYPE , HOOK_CLASS_CHANGED)},
#line 89 "config.lexer.gperf.c"
      {"splashscreen",              T(WINDOW_TYPE , WNCK_WINDOW_SPLASHSCREEN)},
#line 50 "config.lexer.gperf.c"
      {"set_skip_tasklist",         T(ACTION_TYPE , STATEMENT_SET_SKIP_TASKLIST)},
#line 101 "config.lexer.gperf.c"
      {"state",                     T(COND_STATE              , 0)},
#line 48 "config.lexer.gperf.c"
      {"set_skip_pager",            T(ACTION_TYPE , STATEMENT_SET_SKIP_PAGER)},
#line 111 "config.lexer.gperf.c"
      {"--delay",                   T(OPTION_DELAY            , YYLVAL_OPTION_WITH_ARG)},
#line 104 "config.lexer.gperf.c"
      {"system",                    T(SYSTEM                  , 0)},
      {""},
#line 128 "config.lexer.gperf.c"
      {"--static",                  T(GRAVITY , WNCK_WINDOW_GRAVITY_STATIC)},
#line 70 "config.lexer.gperf.c"
      {"showing_desktop_changed",   T(HOOK_TYPE , HOOK_SHOWING_DESKTOP_CHANGED)},
#line 51 "config.lexer.gperf.c"
      {"unset_skip_tasklist",       T(ACTION_TYPE , STATEMENT_UNSET_SKIP_TASKLIST)},
#line 126 "config.lexer.gperf.c"
      {"--south",                   T(GRAVITY , WNCK_WINDOW_GRAVITY_SOUTH)},
      {""}, {""},
#line 44 "config.lexer.gperf.c"
      {"make_below",                T(ACTION_TYPE , STATEMENT_MAKE_BELOW)},
#line 120 "config.lexer.gperf.c"
      {"--north",                   T(GRAVITY , WNCK_WINDOW_GRAVITY_NORTH)},
      {""},
#line 112 "config.lexer.gperf.c"
      {"--refresh-after",           T(OPTION_REFRESH_AFTER    , YYLVAL_OPTION_WITH_ARG)},
#line 110 "config.lexer.gperf.c"
      {"contains",                  T(CONTAINS                , 0)},
#line 116 "config.lexer.gperf.c"
      {"--width",                   T(OPTION_WIDTH            , YYLVAL_OPTION_WITH_ARG)},
#line 38 "config.lexer.gperf.c"
      {"maximize_vertically",       T(ACTION_TYPE , STATEMENT_MAXIMIZE_VERTICALLY)},
#line 36 "config.lexer.gperf.c"
      {"maximize_horizontally",     T(ACTION_TYPE , STATEMENT_MAXIMIZE_HORIZONTALLY)},
#line 58 "config.lexer.gperf.c"
      {"set_geometry",              T(SET_GEOMETRY, 0 /* Action */)},
#line 72 "config.lexer.gperf.c"
      {"maximized_horizontally",    T(WINDOW_STATE , WNCK_WINDOW_STATE_MAXIMIZED_HORIZONTALLY)},
#line 66 "config.lexer.gperf.c"
      {"workspace_changed",         T(HOOK_TYPE , HOOK_WORKSPACE_CHANGED)},
      {""},
#line 79 "config.lexer.gperf.c"
      {"demands_attention",         T(WINDOW_STATE , WNCK_WINDOW_STATE_DEMANDS_ATTENTION)},
      {""},
#line 117 "config.lexer.gperf.c"
      {"--height",                  T(OPTION_HEIGHT           , YYLVAL_OPTION_WITH_ARG)},
      {""},
#line 73 "config.lexer.gperf.c"
      {"maximized_vertically",      T(WINDOW_STATE , WNCK_WINDOW_STATE_MAXIMIZED_VERTICALLY)},
      {""},
#line 47 "config.lexer.gperf.c"
      {"unstick",                   T(ACTION_TYPE , STATEMENT_UNSTICK)},
      {""},
#line 53 "config.lexer.gperf.c"
      {"unset_fullscreen",          T(ACTION_TYPE , STATEMENT_UNSET_FULLSCREEN)},
      {""}, {""},
#line 75 "config.lexer.gperf.c"
      {"skip_pager",                T(WINDOW_STATE , WNCK_WINDOW_STATE_SKIP_PAGER)},
#line 45 "config.lexer.gperf.c"
      {"unmake_below",              T(ACTION_TYPE , STATEMENT_UNMAKE_BELOW)},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 39 "config.lexer.gperf.c"
      {"unmaximize_vertically",     T(ACTION_TYPE , STATEMENT_UNMAXIMIZE_VERTICALLY)},
#line 37 "config.lexer.gperf.c"
      {"unmaximize_horizontally",   T(ACTION_TYPE , STATEMENT_UNMAXIMIZE_HORIZONTALLY)},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 87 "config.lexer.gperf.c"
      {"menu",                      T(WINDOW_TYPE , WNCK_WINDOW_MENU)},
      {""},
#line 67 "config.lexer.gperf.c"
      {"state_changed",             T(HOOK_TYPE , HOOK_STATE_CHANGED)}
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
#line 129 "config.lexer.gperf.c"


#define LEX_ERROR '\1'

FILE *yyin;
static char *yytext;
static unsigned int yyleng;
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
    yyallocated += 512;
    yytext = realloc(yytext, yyallocated);
  }
  yytext[yyleng++] = c;
}

static void yytext_finalize() {
  if (yyleng >= yyallocated) {
    yyallocated += 512;
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
// ========= Whitespace, shell comments, C-like comments ======================
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
      printerr("Warning: EOF reached while scanning /* comment */\n");
      return EOF;
// ========= Strings ==========================================================
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
// ========= Numbers ==========================================================
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
      for (yytext_append(c); isdigit(c = GETC()); yytext_append(c));
      UNGETC(c);
      yytext_finalize();
      return yylval.number = atoi(yytext), NUMBER;
    default:
// ========= Operators ========================================================
#define TOK(A,B) yylval.number = B, A
#define CHPAIR(C1, C2) C1+(C2<<8)
      if (strchr("<=>!|&~", c)) {
        int nextc = GETC();
        if (nextc == EOF)
          goto ONE_CHARACTER;
        switch (CHPAIR(c,nextc)) {
          case CHPAIR('&','&'): return TOK(AND_AND       , 0);
          case CHPAIR('|','|'): return TOK(OR_OR         , 0);
          case CHPAIR('!','='): return TOK(UNEQUAL       , COMPARE_UNEQUAL);
          case CHPAIR('!','~'): return TOK(REGEX_UNEQUAL , 0);
          case CHPAIR('=','='): return TOK(EQUAL         , COMPARE_EQUAL);
          case CHPAIR('=','~'): return TOK(REGEX_EQUAL   , 0);
          case CHPAIR('<','='): return TOK(LESS_EQUAL    , COMPARE_LESS_EQUAL);
          case CHPAIR('>','='): return TOK(GREATER_EQUAL , COMPARE_GREATER_EQUAL);
          default: UNGETC(nextc);
ONE_CHARACTER:
            switch (c) {
              case '!':         return TOK(NOT           , 0);
              case '<':         return TOK(LESS          , COMPARE_LESS);
              case '>':         return TOK(GREATER       , COMPARE_GREATER);
            }
        }
        printerr("Error: unknown operator: %c\n", c);
        return LEX_ERROR;
      }

// ========= Keywords =========================================================
      if (! (isalpha(c) || c == '-' || c == '_'))
        return c; // Cannot be a keyword

      do yytext_append(c);
      while (c = GETC(), (isalpha(c) || c == '-' || c == '_'));
      UNGETC(c);
      yytext_finalize();
      //printf("Having keyword: >%s< (strlen(%d) == yyleng(%d))\n", yytext, strlen(yytext), yyleng);

      tok = in_word_set(yytext, yyleng);
      if (tok) {
        // An --option may have a trailing '='
        if (tok->yylval == YYLVAL_OPTION_WITH_ARG)
          if ((c = GETC()) != '=')
            UNGETC(c);
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
