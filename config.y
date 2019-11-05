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
extern int yylineno;
static Statement *config;
static int parsing_error;

void yyerror(const char *str)
{
  printerr("Error: %s (line: %d)\n", str, yylineno);
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
  WSWindowString field;
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
%token <comparison> EQUAL UNEQUAL LESS LESS_EQUAL GREATER GREATER_EQUAL
%token REGEX_EQUAL REGEX_UNEQUAL CONTAINS
%token PRINT SYSTEM SUSPEND RETURN PROCESS HAS CHILDREN
%token OPTION_SUSPEND_DELAY OPTION_REFRESH_DELAY OPTION_REFRESH_DURATION
%token COND_TYPE COND_STACKPOSITION COND_STATE COND_WORKSPACE_NUMBER
%token NOT AND_AND OR_OR

/* Actions */
%token CLOSE MINIMIZE UNMINIMIZE MAXIMIZE UNMAXIMIZE MAXIMIZE_HORIZONTALLY
%token UNMAXIMIZE_HORIZONTALLY MAXIMIZE_VERTICALLY UNMAXIMIZE_VERTICALLY
%token SHADE UNSHADE MAKE_ABOVE UNMAKE_ABOVE MAKE_BELOW UNMAKE_BELOW STICK UNSTICK
%token SET_SKIP_PAGER UNSET_SKIP_PAGER SET_SKIP_TASKLIST UNSET_SKIP_TASKLIST
%token SET_FULLSCREEN UNSET_FULLSCREEN PIN UNPIN ACTIVATE ACTIVATE_TRANSIENT

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
    | COND_TYPE EQUAL WINDOW_TYPE     { $$ = conditional_windowtype_new($3); }
    | COND_STATE EQUAL WINDOW_STATE   { $$ = conditional_windowstate_new($3); }
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
    | suspend_statement ';'
    /* Actions are generated using:
          cpp -P -D GENERATE_YACC_RULES actions.gen.h | column -t
    */
    | CLOSE                   ';' { $$ = statement_close_new();                   }
    | MINIMIZE                ';' { $$ = statement_minimize_new();                }
    | UNMINIMIZE              ';' { $$ = statement_unminimize_new();              }
    | MAXIMIZE                ';' { $$ = statement_maximize_new();                }
    | UNMAXIMIZE              ';' { $$ = statement_unmaximize_new();              }
    | MAXIMIZE_HORIZONTALLY   ';' { $$ = statement_maximize_horizontally_new();   }
    | UNMAXIMIZE_HORIZONTALLY ';' { $$ = statement_unmaximize_horizontally_new(); }
    | MAXIMIZE_VERTICALLY     ';' { $$ = statement_maximize_vertically_new();     }
    | UNMAXIMIZE_VERTICALLY   ';' { $$ = statement_unmaximize_vertically_new();   }
    | SHADE                   ';' { $$ = statement_shade_new();                   }
    | UNSHADE                 ';' { $$ = statement_unshade_new();                 }
    | MAKE_ABOVE              ';' { $$ = statement_make_above_new();              }
    | UNMAKE_ABOVE            ';' { $$ = statement_unmake_above_new();            }
    | MAKE_BELOW              ';' { $$ = statement_make_below_new();              }
    | UNMAKE_BELOW            ';' { $$ = statement_unmake_below_new();            }
    | STICK                   ';' { $$ = statement_stick_new();                   }
    | UNSTICK                 ';' { $$ = statement_unstick_new();                 }
    | SET_SKIP_PAGER          ';' { $$ = statement_set_skip_pager_new();          }
    | UNSET_SKIP_PAGER        ';' { $$ = statement_unset_skip_pager_new();        }
    | SET_SKIP_TASKLIST       ';' { $$ = statement_set_skip_tasklist_new();       }
    | UNSET_SKIP_TASKLIST     ';' { $$ = statement_unset_skip_tasklist_new();     }
    | SET_FULLSCREEN          ';' { $$ = statement_set_fullscreen_new();          }
    | UNSET_FULLSCREEN        ';' { $$ = statement_unset_fullscreen_new();        }
    | PIN                     ';' { $$ = statement_pin_new();                     }
    | UNPIN                   ';' { $$ = statement_unpin_new();                   }
    | ACTIVATE                ';' { $$ = statement_activate_new();                }
    | ACTIVATE_TRANSIENT      ';' { $$ = statement_activate_transient_new();      }
    ;

