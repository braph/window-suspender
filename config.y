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

int yylex();
int yyparse();
extern FILE *yyin;
extern int yylineno, yylinepos;
static Statement *config;
static int parsing_error;

void yyerror(const char *str)
{
  printerr("Error: %s (line: %d:%d)\n", str, yylineno, yylinepos);
  parsing_error = 1;
}

Statement* parse_config(const char *file) {
  config = NULL;
  parsing_error = 0;
  if (! (yyin = fopen(file, "r"))) {
    printerr("Error: %s: %s\n", file, strerror(errno));
    return NULL;
  }
  yyparse();
  fclose(yyin);
  if (parsing_error) {
    if (config) {
      statement_free(config);
      config = NULL;
    }
  }
  return config;
}

%}

%union
{
  int number;
  char *string;
  WnckWindowType type;
  WnckWindowState state;
  window_string_id field;
  statement_type action_type;
  hook_type hook;
  comparison_type comparison;
  struct Statement *statement;
  struct Conditional *conditional;
  GSList *string_list;
}

%token <string> STRING
%token <number> NUMBER
%token <state>  WINDOW_STATE
%token <type>   WINDOW_TYPE
%token <field>  WINDOW_FIELD
%token <hook>   HOOK_TYPE
%token <action_type> ACTION_TYPE
%token <comparison> EQUAL UNEQUAL LESS LESS_EQUAL GREATER GREATER_EQUAL
%token REGEX_EQUAL REGEX_UNEQUAL CONTAINS
%token PRINT SYSTEM SUSPEND RETURN PROCESS HAS CHILDREN
%token OPTION_SUSPEND_DELAY OPTION_REFRESH_DELAY OPTION_REFRESH_DURATION
%token COND_TYPE COND_STACKPOSITION COND_STATE COND_HOOK COND_WORKSPACE_NUMBER
%token NOT AND_AND OR_OR

%type <conditional> condition
%type <statement> config statement statement_list
%type <statement> compound_statement command_statement if_statement suspend_statement
%type <comparison> numeric_comparison
%type <string_list> string_list

%start config

%left AND_AND
%right OR_OR
%left NOT

%%

config
    : /*empty*/      { config = statement_return_new(); }
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
    | WINDOW_FIELD REGEX_EQUAL STRING { $$ = conditional_regex_match_new($1, $3);          free($3); }
    | WINDOW_FIELD UNEQUAL STRING     { $$ = C_NOT(conditional_string_match_new($1, $3));  free($3); }
    | WINDOW_FIELD REGEX_UNEQUAL STRING { $$ = C_NOT(conditional_regex_match_new($1, $3)); free($3); }
    | WINDOW_FIELD CONTAINS STRING    { $$ = conditional_string_contains_new($1, $3);      free($3); }
    | SYSTEM STRING                   { $$ = conditional_system_new($2);                   free($2); }
    | COND_TYPE EQUAL WINDOW_TYPE     { $$ = conditional_windowtype_new($3);  }
    | COND_STATE EQUAL WINDOW_STATE   { $$ = conditional_windowstate_new($3); }
    | COND_HOOK EQUAL HOOK_TYPE       { $$ = conditional_hook_new($3);        }
    | COND_STACKPOSITION numeric_comparison NUMBER { $$ = conditional_stackposition_new($2, $3); }
    | COND_WORKSPACE_NUMBER numeric_comparison NUMBER { $$ = conditional_workspace_number_new($2, $3); }
    | condition AND_AND condition     { $$ = conditional_logic_and_new($1, $3); }
    | condition OR_OR condition %prec AND_AND { $$ = conditional_logic_or_new($1, $3); }
    | NOT condition  %prec OR_OR      { $$ = C_NOT($2); }
    | '(' condition ')'               { $$ = $2; }
    ;

statement
    : if_statement
    | command_statement
    | compound_statement
    ;

if_statement
    : '(' condition ')' statement { $$ = statement_if_new($2, $4); }
    ;

compound_statement
    : '{' statement_list '}' { $$ = $2; }
    ;

statement_list
    : statement
    | statement statement_list { $1->next = $2; }
    ;

suspend_statement
    : SUSPEND { $$ = statement_suspend_new(0,0,0); }
    | suspend_statement OPTION_SUSPEND_DELAY NUMBER    { $1->klass.suspend.suspend_delay = $3; }
    | suspend_statement OPTION_REFRESH_DELAY NUMBER    { $1->klass.suspend.refresh_delay = $3; }
    | suspend_statement OPTION_REFRESH_DURATION NUMBER { $1->klass.suspend.refresh_duration = $3; }
    ;

command_statement
    : ';'               { $$ = statement_noop_new(); }
    | PRINT STRING ';'  { $$ = statement_print_new($2);  free($2); }
    | SYSTEM STRING ';' { $$ = statement_system_new($2); free($2); }
    | RETURN ';'        { $$ = statement_return_new(); }
    | ACTION_TYPE ';'   { $$ = statement_action_new($1); }
    | suspend_statement ';'
    ;

