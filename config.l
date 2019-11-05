%{
#include <stdlib.h>
#include <string.h>
#include "wnck.h"
#include "conditional.h"
#include "y.tab.h"

// Actions are generated using:
// cpp -P -D GENERATE_LEX_RULES actions.gen.h | column -t
%}

%option yylineno
%option noyywrap
%option nounput
%option noinput

%%
close                    return CLOSE;
minimize                 return MINIMIZE;
unminimize               return UNMINIMIZE;
maximize                 return MAXIMIZE;
unmaximize               return UNMAXIMIZE;
maximize_horizontally    return MAXIMIZE_HORIZONTALLY;
unmaximize_horizontally  return UNMAXIMIZE_HORIZONTALLY;
maximize_vertically      return MAXIMIZE_VERTICALLY;
unmaximize_vertically    return UNMAXIMIZE_VERTICALLY;
shade                    return SHADE;
unshade                  return UNSHADE;
make_above               return MAKE_ABOVE;
unmake_above             return UNMAKE_ABOVE;
make_below               return MAKE_BELOW;
unmake_below             return UNMAKE_BELOW;
stick                    return STICK;
unstick                  return UNSTICK;
set_skip_pager           return SET_SKIP_PAGER;
unset_skip_pager         return UNSET_SKIP_PAGER;
set_skip_tasklist        return SET_SKIP_TASKLIST;
unset_skip_tasklist      return UNSET_SKIP_TASKLIST;
set_fullscreen           return SET_FULLSCREEN;
unset_fullscreen         return UNSET_FULLSCREEN;
pin                      return PIN;
unpin                    return UNPIN;
activate                 return ACTIVATE;
activate_transient       return ACTIVATE_TRANSIENT;

minimized              return yylval.state=WNCK_WINDOW_STATE_MINIMIZED              , WINDOW_STATE;
maximized_horizontally return yylval.state=WNCK_WINDOW_STATE_MAXIMIZED_HORIZONTALLY , WINDOW_STATE;
maximized_vertically   return yylval.state=WNCK_WINDOW_STATE_MAXIMIZED_VERTICALLY   , WINDOW_STATE;
shaded                 return yylval.state=WNCK_WINDOW_STATE_SHADED                 , WINDOW_STATE;
skip_pager             return yylval.state=WNCK_WINDOW_STATE_SKIP_PAGER             , WINDOW_STATE;
skip_tasklist          return yylval.state=WNCK_WINDOW_STATE_SKIP_TASKLIST          , WINDOW_STATE;
hidden                 return yylval.state=WNCK_WINDOW_STATE_HIDDEN                 , WINDOW_STATE;
fullscreen             return yylval.state=WNCK_WINDOW_STATE_FULLSCREEN             , WINDOW_STATE;
demands_attention      return yylval.state=WNCK_WINDOW_STATE_DEMANDS_ATTENTION      , WINDOW_STATE;
above                  return yylval.state=WNCK_WINDOW_STATE_ABOVE                  , WINDOW_STATE;
below                  return yylval.state=WNCK_WINDOW_STATE_BELOW                  , WINDOW_STATE;
inactive_workspace     return yylval.state=WNCK_WINDOW_STATE_INACTIVE_WORKSPACE     , WINDOW_STATE;
sticky                 return yylval.state=WNCK_WINDOW_STATE_STICKY                 , WINDOW_STATE;

desktop                return yylval.type=WNCK_WINDOW_DESKTOP      , WINDOW_TYPE;
dialog                 return yylval.type=WNCK_WINDOW_DIALOG       , WINDOW_TYPE;
dock                   return yylval.type=WNCK_WINDOW_DOCK         , WINDOW_TYPE;
menu                   return yylval.type=WNCK_WINDOW_MENU         , WINDOW_TYPE;
normal                 return yylval.type=WNCK_WINDOW_NORMAL       , WINDOW_TYPE;
splashscreen           return yylval.type=WNCK_WINDOW_SPLASHSCREEN , WINDOW_TYPE;
toolbar                return yylval.type=WNCK_WINDOW_TOOLBAR      , WINDOW_TYPE;
utility                return yylval.type=WNCK_WINDOW_UTILITY      , WINDOW_TYPE;

title                  return yylval.field=WINDOW_TITLE     , WINDOW_FIELD;
groupname              return yylval.field=WINDOW_GROUP     , WINDOW_FIELD;
classname              return yylval.field=WINDOW_CLASS     , WINDOW_FIELD;
iconname               return yylval.field=WINDOW_ICON_NAME , WINDOW_FIELD;
role                   return yylval.field=WINDOW_ROLE      , WINDOW_FIELD;
workspace              return yylval.field=WINDOW_WORKSPACE , WINDOW_FIELD;
workspace_number       return COND_WORKSPACE_NUMBER;
type                   return COND_TYPE;
stackposition          return COND_STACKPOSITION;
state                  return COND_STATE;

print                  return PRINT;
system                 return SYSTEM;
return                 return RETURN;
suspend                return SUSPEND;
process                return PROCESS;
children               return CHILDREN;
has                    return HAS;
--suspend-delay=?      return OPTION_SUSPEND_DELAY;
--refresh-delay=?      return OPTION_REFRESH_DELAY;
--refresh-duration=?   return OPTION_REFRESH_DURATION;

==                     return yylval.comparison=COMPARE_EQUAL         , EQUAL;
!=                     return yylval.comparison=COMPARE_UNEQUAL       , UNEQUAL;
>                      return yylval.comparison=COMPARE_GREATER       , GREATER;
>=                     return yylval.comparison=COMPARE_GREATER_EQUAL , GREATER_EQUAL;
\<                     return yylval.comparison=COMPARE_LESS          , LESS;
\<=                    return yylval.comparison=COMPARE_LESS_EQUAL    , LESS_EQUAL;
=~                     return /* --------------------------------- */   REGEX_EQUAL;
!~                     return /* --------------------------------- */   REGEX_UNEQUAL;
contains               return /* --------------------------------- */   CONTAINS;

!                      return NOT;
&&                     return AND_AND;
\|\|                   return OR_OR;

[0-9]+                 return yylval.number=atoi(yytext), NUMBER;
'[^']*'|\"[^"]*\"      return yylval.string=strdup(yytext+1), yylval.string[yyleng-2]='\0', STRING;

#[^\n]*\n              /* ignore comments */;
[ \t\n]+               /* ignore whitespace */;

.                      return yytext[0];
%%
