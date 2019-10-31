%{
#include <stdlib.h>
#include <string.h>
#include "wnck.h"
#include "conditional.h"
#include "y.tab.h"
%}

%option yylineno

%%
minimized              yylval.state=WNCK_WINDOW_STATE_MINIMIZED              ;  return WINDOW_STATE;
maximized_horizontally yylval.state=WNCK_WINDOW_STATE_MAXIMIZED_HORIZONTALLY ;  return WINDOW_STATE;
maximized_vertically   yylval.state=WNCK_WINDOW_STATE_MAXIMIZED_VERTICALLY   ;  return WINDOW_STATE;
shaded                 yylval.state=WNCK_WINDOW_STATE_SHADED                 ;  return WINDOW_STATE;
skip_pager             yylval.state=WNCK_WINDOW_STATE_SKIP_PAGER             ;  return WINDOW_STATE;
skip_tasklist          yylval.state=WNCK_WINDOW_STATE_SKIP_TASKLIST          ;  return WINDOW_STATE;
hidden                 yylval.state=WNCK_WINDOW_STATE_HIDDEN                 ;  return WINDOW_STATE;
fullscreen             yylval.state=WNCK_WINDOW_STATE_FULLSCREEN             ;  return WINDOW_STATE;
demands_attention      yylval.state=WNCK_WINDOW_STATE_DEMANDS_ATTENTION      ;  return WINDOW_STATE;
above                  yylval.state=WNCK_WINDOW_STATE_ABOVE                  ;  return WINDOW_STATE;
below                  yylval.state=WNCK_WINDOW_STATE_BELOW                  ;  return WINDOW_STATE;
inactive_workspace     yylval.state=WNCK_WINDOW_STATE_INACTIVE_WORKSPACE     ;  return WINDOW_STATE;
sticky                 yylval.state=WNCK_WINDOW_STATE_STICKY                 ;  return WINDOW_STATE;

desktop                yylval.type=WNCK_WINDOW_DESKTOP      ; return WINDOW_TYPE;
dialog                 yylval.type=WNCK_WINDOW_DIALOG       ; return WINDOW_TYPE;
dock                   yylval.type=WNCK_WINDOW_DOCK         ; return WINDOW_TYPE;
menu                   yylval.type=WNCK_WINDOW_MENU         ; return WINDOW_TYPE;
normal                 yylval.type=WNCK_WINDOW_NORMAL       ; return WINDOW_TYPE;
splashscreen           yylval.type=WNCK_WINDOW_SPLASHSCREEN ; return WINDOW_TYPE;
toolbar                yylval.type=WNCK_WINDOW_TOOLBAR      ; return WINDOW_TYPE;
utility                yylval.type=WNCK_WINDOW_UTILITY      ; return WINDOW_TYPE;

title                  yylval.field=WINDOW_TITLE     ; return WINDOW_FIELD;
groupname              yylval.field=WINDOW_GROUP     ; return WINDOW_FIELD;
classname              yylval.field=WINDOW_CLASS     ; return WINDOW_FIELD;
iconname               yylval.field=WINDOW_ICON_NAME ; return WINDOW_FIELD;
role                   yylval.field=WINDOW_ROLE      ; return WINDOW_FIELD;
type                   return COND_TYPE;
stackposition          return COND_STACKPOSITION;
state                  return COND_STATE;

print                  return PRINT;
system                 return SYSTEM;
return                 return RETURN;
suspend                return SUSPEND;
--suspend-delay=?      return OPTION_SUSPEND_DELAY;
--refresh-delay=?      return OPTION_REFRESH_DELAY;
--refresh-duration=?   return OPTION_REFRESH_DURATION;

!                      /* -----------------------------------*/  return NOT;
==                     yylval.comparison=COMPARE_EQUAL        ;  return EQUAL;
!=                     yylval.comparison=COMPARE_UNEQUAL      ;  return UNEQUAL;
=~                     /* -----------------------------------*/  return REGEX_EQUAL;
!~                     /* -----------------------------------*/  return REGEX_UNEQUAL;
>                      yylval.comparison=COMPARE_GREATER      ;  return GREATER;
>=                     yylval.comparison=COMPARE_GREATER_EQUAL;  return GREATER_EQUAL;
\<                     yylval.comparison=COMPARE_LESS         ;  return LESS;
\<=                    yylval.comparison=COMPARE_LESS_EQUAL   ;  return LESS_EQUAL;
contains               /* -----------------------------------*/  return CONTAINS;

&&                     yylval.logic=CONDITIONAL_LOGIC_AND; return AND_AND;
\|\|                   yylval.logic=CONDITIONAL_LOGIC_OR ; return OR_OR;

[0-9]+                 yylval.number=atoi(yytext);   return NUMBER;

'[^']*'|\"[^"]*\"      {
                          yylval.string=strdup(yytext+1);
                          yylval.string[strlen(yylval.string)-1] = '\0';
                          return STRING;
                       }

#[^\n]*\n              /* ignore comments */;
[ \t\n]+               /* ignore whitespace */;

.                      return yytext[0];
%%
