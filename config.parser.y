%{
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "kill.h"
#include "wnck.h"
#include "common.h"
#include "statement.h"
#include "conditional.h"

#define C_NOT(X) conditional_logic_not_new(X)

int yylex(), yyparse();
extern FILE *yyin;
extern int yylineno, yylinepos;
static Statement *config;

static void yyerror(const char *str) {
  printerr("Error: %s (line: %d:%d)\n", str, yylineno, yylinepos);
}

Statement* parse_config(const char *file) {
  if (! (yyin = fopen(file, "r")))
    return printerr("Error: %s: %s\n", file, strerror(errno)), NULL;

  config = NULL;
  errno = yyparse();
  if (errno) {
    if (config)
      statement_free(config);
    config = NULL;
  }
  fclose(yyin);
  return config;
}

%}

%union
{
  int number;
  char *string;
  WnckWindowType type;
  WnckWindowState state;
  WnckWindowGravity gravity;
  window_string_id field;
  statement_type action_type;
  hook_type hook;
  comparison_type comparison;
  struct Statement *statement;
  struct Conditional *conditional;
  GSList *string_list;
}

%token <string>       STRING
%token <number>       NUMBER
%token <state>        WINDOW_STATE
%token <type>         WINDOW_TYPE
%token <field>        WINDOW_FIELD
%token <hook>         HOOK_TYPE
%token <gravity>      GRAVITY
%token <action_type>  ACTION_TYPE
%token <comparison>   EQUAL UNEQUAL LESS LESS_EQUAL GREATER GREATER_EQUAL
%token SET_GEOMETRY
%token REGEX_EQUAL REGEX_UNEQUAL CONTAINS
%token PRINT SYSTEM SUSPEND RETURN PROCESS HAS CHILDREN
%token OPTION_DELAY OPTION_REFRESH_AFTER OPTION_REFRESH_DURATION
%token OPTION_X OPTION_Y OPTION_WIDTH OPTION_HEIGHT
%token COND_TYPE COND_STACKPOSITION COND_STATE COND_HOOK COND_WORKSPACE_NUMBER
%token NOT AND_AND OR_OR

%type <conditional> condition
%type <statement> config statement statement_list set_geometry_statement
%type <statement> command_statement if_statement suspend_statement
%type <comparison> numeric_comparison
%type <string_list> string_list

%right AND_AND
%right OR_OR
%right NOT

%start config
%%

config
    : /*empty*/      { config = statement_noop_new(); }
    | statement_list { config = $1; }
    | process_children_list statement_list { config = $2; }
    ;

process_children_list
    : process_children
    | process_children process_children_list
    ;

process_children
    : PROCESS STRING HAS CHILDREN string_list ';' { process_rule_add($2, $5); free($2); }
    ;

string_list
    : STRING                  { $$ = g_slist_append(NULL, $1); }
    | string_list ',' STRING  { $$ = g_slist_append($1, $3);   }
    ;

numeric_comparison
    : EQUAL | UNEQUAL | LESS | LESS_EQUAL | GREATER | GREATER_EQUAL
    ;

condition
    : WINDOW_FIELD EQUAL STRING       { $$ = conditional_string_match_new($1, $3);         free($3); }
    | WINDOW_FIELD UNEQUAL STRING     { $$ = C_NOT(conditional_string_match_new($1, $3));  free($3); }
    | WINDOW_FIELD REGEX_EQUAL STRING   { $$ = conditional_regex_match_new($1, $3);        free($3); }
    | WINDOW_FIELD REGEX_UNEQUAL STRING { $$ = C_NOT(conditional_regex_match_new($1, $3)); free($3); }
    | WINDOW_FIELD CONTAINS STRING    { $$ = conditional_string_contains_new($1, $3);      free($3); }
    | SYSTEM STRING                   { $$ = conditional_system_new($2);                   free($2); }
    | COND_TYPE EQUAL WINDOW_TYPE     { $$ = conditional_windowtype_new($3);         }
    | COND_TYPE UNEQUAL WINDOW_TYPE   { $$ = C_NOT(conditional_windowtype_new($3));  }
    | COND_HOOK EQUAL HOOK_TYPE       { $$ = conditional_hook_new($3);               }
    | COND_HOOK UNEQUAL HOOK_TYPE     { $$ = C_NOT(conditional_hook_new($3));        }
    | COND_STATE EQUAL WINDOW_STATE   { $$ = conditional_windowstate_new($3);        }
    | COND_STATE UNEQUAL WINDOW_STATE { $$ = C_NOT(conditional_windowstate_new($3)); }
    | COND_STACKPOSITION numeric_comparison NUMBER { $$ = conditional_stackposition_new($2, $3); }
    | COND_WORKSPACE_NUMBER numeric_comparison NUMBER { $$ = conditional_workspace_number_new($2, $3); }
    | condition AND_AND condition     { $$ = conditional_logic_and_new($1, $3); }
    | condition OR_OR condition %prec AND_AND { $$ = conditional_logic_or_new($1, $3); }
    | NOT condition  %prec OR_OR      { $$ = C_NOT($2); }
    | '(' condition ')'               { $$ = $2; }
    ;

statement
    : if_statement
    | command_statement ';'  { $$ = $1; }
    | '{' statement_list '}' { $$ = $2; }
    ;

if_statement
    : '(' condition ')' statement { $$ = statement_if_new($2, $4); }
    ;

statement_list
    : statement
    | statement statement_list { $1->next = $2; }
    ;

suspend_statement
    : SUSPEND                                          { $$ = statement_suspend_new(0,0,0);       }
    | suspend_statement OPTION_DELAY NUMBER            { $1->klass.suspend.suspend_delay = $3;    }
    | suspend_statement OPTION_REFRESH_AFTER NUMBER    { $1->klass.suspend.refresh_delay = $3;    }
    | suspend_statement OPTION_REFRESH_DURATION NUMBER { $1->klass.suspend.refresh_duration = $3; }
    ;

set_geometry_statement
    : SET_GEOMETRY
      { $$ = statement_action_set_geometry_new(); }
    | set_geometry_statement OPTION_X NUMBER
      { $1->klass.geometry.mask |= WNCK_WINDOW_CHANGE_X;      $1->klass.geometry.x = $3;      }
    | set_geometry_statement OPTION_Y NUMBER
      { $1->klass.geometry.mask |= WNCK_WINDOW_CHANGE_Y;      $1->klass.geometry.y = $3;      }
    | set_geometry_statement OPTION_WIDTH NUMBER
      { $1->klass.geometry.mask |= WNCK_WINDOW_CHANGE_WIDTH;  $1->klass.geometry.width = $3;  }
    | set_geometry_statement OPTION_HEIGHT NUMBER
      { $1->klass.geometry.mask |= WNCK_WINDOW_CHANGE_HEIGHT; $1->klass.geometry.height= $3;  }
    | set_geometry_statement GRAVITY
      { $1->klass.geometry.gravity = $2; }
    ;

command_statement
    : /* empty */   { $$ = statement_noop_new();               }
    | PRINT STRING  { $$ = statement_print_new($2);  free($2); }
    | SYSTEM STRING { $$ = statement_system_new($2); free($2); }
    | RETURN        { $$ = statement_return_new();             }
    | ACTION_TYPE   { $$ = statement_action_new($1);           }
    | set_geometry_statement
    | suspend_statement
    ;
