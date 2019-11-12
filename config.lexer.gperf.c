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

%struct-type
%readonly-tables
%7bit

// STRING,                   T(TOKEN       , YYLVAL)
%%
"close",                     T(ACTION_TYPE , STATEMENT_CLOSE)
"minimize",                  T(ACTION_TYPE , STATEMENT_MINIMIZE)
"unminimize",                T(ACTION_TYPE , STATEMENT_UNMINIMIZE)
"maximize",                  T(ACTION_TYPE , STATEMENT_MAXIMIZE)
"unmaximize",                T(ACTION_TYPE , STATEMENT_UNMAXIMIZE)
"maximize_horizontally",     T(ACTION_TYPE , STATEMENT_MAXIMIZE_HORIZONTALLY)
"unmaximize_horizontally",   T(ACTION_TYPE , STATEMENT_UNMAXIMIZE_HORIZONTALLY)
"maximize_vertically",       T(ACTION_TYPE , STATEMENT_MAXIMIZE_VERTICALLY)
"unmaximize_vertically",     T(ACTION_TYPE , STATEMENT_UNMAXIMIZE_VERTICALLY)
"shade",                     T(ACTION_TYPE , STATEMENT_SHADE)
"unshade",                   T(ACTION_TYPE , STATEMENT_UNSHADE)
"make_above",                T(ACTION_TYPE , STATEMENT_MAKE_ABOVE)
"unmake_above",              T(ACTION_TYPE , STATEMENT_UNMAKE_ABOVE)
"make_below",                T(ACTION_TYPE , STATEMENT_MAKE_BELOW)
"unmake_below",              T(ACTION_TYPE , STATEMENT_UNMAKE_BELOW)
"stick",                     T(ACTION_TYPE , STATEMENT_STICK)
"unstick",                   T(ACTION_TYPE , STATEMENT_UNSTICK)
"set_skip_pager",            T(ACTION_TYPE , STATEMENT_SET_SKIP_PAGER)
"unset_skip_pager",          T(ACTION_TYPE , STATEMENT_UNSET_SKIP_PAGER)
"set_skip_tasklist",         T(ACTION_TYPE , STATEMENT_SET_SKIP_TASKLIST)
"unset_skip_tasklist",       T(ACTION_TYPE , STATEMENT_UNSET_SKIP_TASKLIST)
"set_fullscreen",            T(ACTION_TYPE , STATEMENT_SET_FULLSCREEN)
"unset_fullscreen",          T(ACTION_TYPE , STATEMENT_UNSET_FULLSCREEN)
"pin",                       T(ACTION_TYPE , STATEMENT_PIN)
"unpin",                     T(ACTION_TYPE , STATEMENT_UNPIN)
"activate",                  T(ACTION_TYPE , STATEMENT_ACTIVATE)
"activate_transient",        T(ACTION_TYPE , STATEMENT_ACTIVATE_TRANSIENT)
"set_geometry",              T(SET_GEOMETRY, 0 /* Action */)
"opened",                    T(HOOK_TYPE , HOOK_OPENED)
"closed",                    T(HOOK_TYPE , HOOK_CLOSED)
"class_changed",             T(HOOK_TYPE , HOOK_CLASS_CHANGED)
"name_changed",              T(HOOK_TYPE , HOOK_NAME_CHANGED)
"role_changed",              T(HOOK_TYPE , HOOK_ROLE_CHANGED)
"type_changed",              T(HOOK_TYPE , HOOK_TYPE_CHANGED)
"icon_changed",              T(HOOK_TYPE , HOOK_ICON_CHANGED)
"workspace_changed",         T(HOOK_TYPE , HOOK_WORKSPACE_CHANGED)
"state_changed",             T(HOOK_TYPE , HOOK_STATE_CHANGED)
"window_stacking_changed",   T(HOOK_TYPE , HOOK_WINDOW_STACKING_CHANGED)
"active_workspace_changed",  T(HOOK_TYPE , HOOK_ACTIVE_WORKSPACE_CHANGED)
"showing_desktop_changed",   T(HOOK_TYPE , HOOK_SHOWING_DESKTOP_CHANGED)
"minimized",                 T(WINDOW_STATE , WNCK_WINDOW_STATE_MINIMIZED)
"maximized_horizontally",    T(WINDOW_STATE , WNCK_WINDOW_STATE_MAXIMIZED_HORIZONTALLY)
"maximized_vertically",      T(WINDOW_STATE , WNCK_WINDOW_STATE_MAXIMIZED_VERTICALLY)
"shaded",                    T(WINDOW_STATE , WNCK_WINDOW_STATE_SHADED)
"skip_pager",                T(WINDOW_STATE , WNCK_WINDOW_STATE_SKIP_PAGER)
"skip_tasklist",             T(WINDOW_STATE , WNCK_WINDOW_STATE_SKIP_TASKLIST)
"hidden",                    T(WINDOW_STATE , WNCK_WINDOW_STATE_HIDDEN)
"fullscreen",                T(WINDOW_STATE , WNCK_WINDOW_STATE_FULLSCREEN)
"demands_attention",         T(WINDOW_STATE , WNCK_WINDOW_STATE_DEMANDS_ATTENTION)
"above",                     T(WINDOW_STATE , WNCK_WINDOW_STATE_ABOVE)
"below",                     T(WINDOW_STATE , WNCK_WINDOW_STATE_BELOW)
"inactive_workspace",        T(WINDOW_STATE , WNCK_WINDOW_STATE_INACTIVE_WORKSPACE)
"sticky",                    T(WINDOW_STATE , WNCK_WINDOW_STATE_STICKY)
"desktop",                   T(WINDOW_TYPE , WNCK_WINDOW_DESKTOP)
"dialog",                    T(WINDOW_TYPE , WNCK_WINDOW_DIALOG)
"dock",                      T(WINDOW_TYPE , WNCK_WINDOW_DOCK)
"menu",                      T(WINDOW_TYPE , WNCK_WINDOW_MENU)
"normal",                    T(WINDOW_TYPE , WNCK_WINDOW_NORMAL)
"splashscreen",              T(WINDOW_TYPE , WNCK_WINDOW_SPLASHSCREEN)
"toolbar",                   T(WINDOW_TYPE , WNCK_WINDOW_TOOLBAR)
"utility",                   T(WINDOW_TYPE , WNCK_WINDOW_UTILITY)
"title",                     T(WINDOW_FIELD , WINDOW_TITLE)
"groupname",                 T(WINDOW_FIELD , WINDOW_GROUP)
"classname",                 T(WINDOW_FIELD , WINDOW_CLASS)
"iconname",                  T(WINDOW_FIELD , WINDOW_ICON_NAME)
"role",                      T(WINDOW_FIELD , WINDOW_ROLE)
"workspace",                 T(WINDOW_FIELD , WINDOW_WORKSPACE)
"workspace_number",          T(COND_WORKSPACE_NUMBER   , 0)
"type",                      T(COND_TYPE               , 0)
"stackposition",             T(COND_STACKPOSITION      , 0)
"state",                     T(COND_STATE              , 0)
"hook",                      T(COND_HOOK               , 0)
"print",                     T(PRINT                   , 0)
"system",                    T(SYSTEM                  , 0)
"return",                    T(RETURN                  , 0)
"suspend",                   T(SUSPEND                 , 0)
"process",                   T(PROCESS                 , 0)
"children",                  T(CHILDREN                , 0)
"has",                       T(HAS                     , 0)
"contains",                  T(CONTAINS                , 0)
"--delay",                   T(OPTION_DELAY            , YYLVAL_OPTION_WITH_ARG)
"--refresh-after",           T(OPTION_REFRESH_AFTER    , YYLVAL_OPTION_WITH_ARG)
"--refresh-duration",        T(OPTION_REFRESH_DURATION , YYLVAL_OPTION_WITH_ARG)
"-x",                        T(OPTION_X                , YYLVAL_OPTION_WITH_ARG)
"-y",                        T(OPTION_Y                , YYLVAL_OPTION_WITH_ARG)
"--width",                   T(OPTION_WIDTH            , YYLVAL_OPTION_WITH_ARG)
"--height",                  T(OPTION_HEIGHT           , YYLVAL_OPTION_WITH_ARG)
"--current",                 T(GRAVITY , WNCK_WINDOW_GRAVITY_CURRENT)
"--northwest",               T(GRAVITY , WNCK_WINDOW_GRAVITY_NORTHWEST)
"--north",                   T(GRAVITY , WNCK_WINDOW_GRAVITY_NORTH)
"--northeast",               T(GRAVITY , WNCK_WINDOW_GRAVITY_NORTHEAST)
"--west",                    T(GRAVITY , WNCK_WINDOW_GRAVITY_WEST)
"--center",                  T(GRAVITY , WNCK_WINDOW_GRAVITY_CENTER)
"--east",                    T(GRAVITY , WNCK_WINDOW_GRAVITY_EAST)
"--southwest",               T(GRAVITY , WNCK_WINDOW_GRAVITY_SOUTHWEST)
"--south",                   T(GRAVITY , WNCK_WINDOW_GRAVITY_SOUTH)
"--southeast",               T(GRAVITY , WNCK_WINDOW_GRAVITY_SOUTHEAST)
"--static",                  T(GRAVITY , WNCK_WINDOW_GRAVITY_STATIC)
%%

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
