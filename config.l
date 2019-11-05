%{
#include <stdlib.h>
#include <string.h>
#include "wnck.h"
#include "conditional.h"
#include "statement.h"
#include "y.tab.h"
%}

%option yylineno
%option noyywrap
%option nounput
%option noinput

%%
 /* Actions */
 /*   cpp -P -D GENERATE_LEX_RULES "actions.gen.h" | column -t     */
close                    return yylval.action_type=STATEMENT_CLOSE                   , ACTION_TYPE;
minimize                 return yylval.action_type=STATEMENT_MINIMIZE                , ACTION_TYPE;
unminimize               return yylval.action_type=STATEMENT_UNMINIMIZE              , ACTION_TYPE;
maximize                 return yylval.action_type=STATEMENT_MAXIMIZE                , ACTION_TYPE;
unmaximize               return yylval.action_type=STATEMENT_UNMAXIMIZE              , ACTION_TYPE;
maximize_horizontally    return yylval.action_type=STATEMENT_MAXIMIZE_HORIZONTALLY   , ACTION_TYPE;
unmaximize_horizontally  return yylval.action_type=STATEMENT_UNMAXIMIZE_HORIZONTALLY , ACTION_TYPE;
maximize_vertically      return yylval.action_type=STATEMENT_MAXIMIZE_VERTICALLY     , ACTION_TYPE;
unmaximize_vertically    return yylval.action_type=STATEMENT_UNMAXIMIZE_VERTICALLY   , ACTION_TYPE;
shade                    return yylval.action_type=STATEMENT_SHADE                   , ACTION_TYPE;
unshade                  return yylval.action_type=STATEMENT_UNSHADE                 , ACTION_TYPE;
make_above               return yylval.action_type=STATEMENT_MAKE_ABOVE              , ACTION_TYPE;
unmake_above             return yylval.action_type=STATEMENT_UNMAKE_ABOVE            , ACTION_TYPE;
make_below               return yylval.action_type=STATEMENT_MAKE_BELOW              , ACTION_TYPE;
unmake_below             return yylval.action_type=STATEMENT_UNMAKE_BELOW            , ACTION_TYPE;
stick                    return yylval.action_type=STATEMENT_STICK                   , ACTION_TYPE;
unstick                  return yylval.action_type=STATEMENT_UNSTICK                 , ACTION_TYPE;
set_skip_pager           return yylval.action_type=STATEMENT_SET_SKIP_PAGER          , ACTION_TYPE;
unset_skip_pager         return yylval.action_type=STATEMENT_UNSET_SKIP_PAGER        , ACTION_TYPE;
set_skip_tasklist        return yylval.action_type=STATEMENT_SET_SKIP_TASKLIST       , ACTION_TYPE;
unset_skip_tasklist      return yylval.action_type=STATEMENT_UNSET_SKIP_TASKLIST     , ACTION_TYPE;
set_fullscreen           return yylval.action_type=STATEMENT_SET_FULLSCREEN          , ACTION_TYPE;
unset_fullscreen         return yylval.action_type=STATEMENT_UNSET_FULLSCREEN        , ACTION_TYPE;
pin                      return yylval.action_type=STATEMENT_PIN                     , ACTION_TYPE;
unpin                    return yylval.action_type=STATEMENT_UNPIN                   , ACTION_TYPE;
activate                 return yylval.action_type=STATEMENT_ACTIVATE                , ACTION_TYPE;
activate_transient       return yylval.action_type=STATEMENT_ACTIVATE_TRANSIENT      , ACTION_TYPE;

 /* Hook types */
opened                   return yylval.hook=HOOK_OPENED                   , HOOK_TYPE;
closed                   return yylval.hook=HOOK_CLOSED                   , HOOK_TYPE;
class_changed            return yylval.hook=HOOK_CLASS_CHANGED            , HOOK_TYPE;
name_changed             return yylval.hook=HOOK_NAME_CHANGED             , HOOK_TYPE;
role_changed             return yylval.hook=HOOK_ROLE_CHANGED             , HOOK_TYPE;
type_changed             return yylval.hook=HOOK_TYPE_CHANGED             , HOOK_TYPE;
icon_changed             return yylval.hook=HOOK_ICON_CHANGED             , HOOK_TYPE;
workspace_changed        return yylval.hook=HOOK_WORKSPACE_CHANGED        , HOOK_TYPE;
state_changed            return yylval.hook=HOOK_STATE_CHANGED            , HOOK_TYPE;
window_stacking_changed  return yylval.hook=HOOK_WINDOW_STACKING_CHANGED  , HOOK_TYPE;
active_workspace_changed return yylval.hook=HOOK_ACTIVE_WORKSPACE_CHANGED , HOOK_TYPE;
showing_desktop_changed  return yylval.hook=HOOK_SHOWING_DESKTOP_CHANGED  , HOOK_TYPE;

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
hook                   return COND_HOOK;

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
