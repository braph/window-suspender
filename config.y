%{
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "wnck.h"
#include "common.h"
#include "statement.h"
#include "conditional.h"

#define C_NOT(X) conditional_logic_not_new(X)

/* TODO:
  Solve SHIFT-REDUCE conflicts (wtf?):
      | condition AND_AND condition  { $$ = conditional_logic_and_new($1, $3); }
      | condition OR_OR condition    { $$ = conditional_logic_or_new($1, $3); }
      | NOT condition                { $$ = C_NOT($2); }
*/

int yylex();
int yyparse();
extern FILE *yyin;
extern int yylineno;
static Statement *config;
static int parsing_error;

void yyerror(const char *str)
{
  printerr("Error: %s (line :%d)\n", str, yylineno);
  parsing_error = 1;
}

int yywrap()
{
  return 1;
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
  WSWindowString field;
  conditional_type logic;
  comparison_type comparison;
  struct Statement *statement;
  struct Conditional *conditional;
}

%token <string> STRING
%token <number> NUMBER
%token <state>  WINDOW_STATE
%token <type>   WINDOW_TYPE
%token <field>  WINDOW_FIELD
%token <logic>  LOGIC_OPERATOR
%token <comparison> EQUAL UNEQUAL LESS LESS_EQUAL GREATER GREATER_EQUAL
%token REGEX_EQUAL REGEX_UNEQUAL CONTAINS
%token PRINT SYSTEM SUSPEND RETURN
%token OPTION_SUSPEND_DELAY OPTION_REFRESH_DELAY OPTION_REFRESH_DURATION
%token COND_TYPE COND_STACKPOSITION COND_STATE
%token NOT AND_AND OR_OR

%type <conditional> condition
%type <statement> config statement statement_list
%type <statement> compound_statement command_statement if_statement suspend_statement
%type <comparison> numeric_comparison

%start config

%left AND_AND
%right OR_OR
%left NOT

%%

config
    : /*empty*/      { config = statement_return_new(); /*empty*/ }
    | statement_list { config = $1; }
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
    | COND_TYPE EQUAL WINDOW_TYPE     { $$ = conditional_windowtype_new($3); }
    | COND_STATE EQUAL WINDOW_STATE   { $$ = conditional_windowstate_new($3); }
    | COND_STACKPOSITION numeric_comparison NUMBER { $$ = conditional_stackposition_new($2, $3); }
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
    | PRINT STRING ';'  { $$ = statement_print_new($2); free($2); }
    | SYSTEM STRING ';' { $$ = statement_system_new($2); free($2); }
    | RETURN ';'        { $$ = statement_return_new(); }
    | suspend_statement ';'
    ;

