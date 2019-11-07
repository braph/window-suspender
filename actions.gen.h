#if 0
  This file is used for generating the code for window actions (maximize, shade, ...)
  in statement.c and enums statement.h.

  It may also be used for generating Lex rules:
    cpp -P -D GENERATE_LEX_RULES "actions.gen.h"

  Usage:
    #define <COMMAND>
    #include "actions.gen.h"
#endif

#if defined ( INCLUDE_STATEMENT_ACTIONS_ENUMS )
#define X(UPPER, ...) \
  STATEMENT_ ## UPPER,

#elif defined ( INCLUDE_STATEMENT_ACTIONS_EXECUTE_CODE )
#define X(UPPER, LOWER, CODE) \
  case STATEMENT_ ## UPPER: \
    CODE; \
  break;

#elif defined ( INCLUDE_STATEMENT_ACTIONS_DUMP_CODE )
#define X(UPPER, LOWER, ...) \
  case STATEMENT_ ## UPPER: \
    outf(#LOWER); \
  break;

#elif defined ( GENERATE_LEX_RULES )
#define X(UPPER, LOWER, ...) \
  LOWER return yylval.action_type=STATEMENT_ ## UPPER , ACTION_TYPE;

#else
#error "Invalid command specified"
#endif

/* UPPER                   LOWER                    CODE */
X(CLOSE,                   close,                   wnck_window_close(win, 0))
X(MINIMIZE,                minimize,                wnck_window_minimize(win))
X(UNMINIMIZE,              unminimize,              wnck_window_unminimize(win, 0))
X(MAXIMIZE,                maximize,                wnck_window_maximize(win))
X(UNMAXIMIZE,              unmaximize,              wnck_window_unmaximize(win))
X(MAXIMIZE_HORIZONTALLY,   maximize_horizontally,   wnck_window_maximize_horizontally(win))
X(UNMAXIMIZE_HORIZONTALLY, unmaximize_horizontally, wnck_window_unmaximize_horizontally(win))
X(MAXIMIZE_VERTICALLY,     maximize_vertically,     wnck_window_maximize_vertically(win))
X(UNMAXIMIZE_VERTICALLY,   unmaximize_vertically,   wnck_window_unmaximize_vertically(win))
X(SHADE,                   shade,                   wnck_window_shade(win))
X(UNSHADE,                 unshade,                 wnck_window_unshade(win))
X(MAKE_ABOVE,              make_above,              wnck_window_make_above(win))
X(UNMAKE_ABOVE,            unmake_above,            wnck_window_unmake_above(win))
X(MAKE_BELOW,              make_below,              wnck_window_make_below(win))
X(UNMAKE_BELOW,            unmake_below,            wnck_window_unmake_below(win))
X(STICK,                   stick,                   wnck_window_stick(win))
X(UNSTICK,                 unstick,                 wnck_window_unstick(win))
X(SET_SKIP_PAGER,          set_skip_pager,          wnck_window_set_skip_pager(win, 1))
X(UNSET_SKIP_PAGER,        unset_skip_pager,        wnck_window_set_skip_pager(win, 0))
X(SET_SKIP_TASKLIST,       set_skip_tasklist,       wnck_window_set_skip_tasklist(win, 1))
X(UNSET_SKIP_TASKLIST,     unset_skip_tasklist,     wnck_window_set_skip_tasklist(win, 0))
X(SET_FULLSCREEN,          set_fullscreen,          wnck_window_set_fullscreen(win, 1))
X(UNSET_FULLSCREEN,        unset_fullscreen,        wnck_window_set_fullscreen(win, 0))
X(PIN,                     pin,                     wnck_window_pin(win))
X(UNPIN,                   unpin,                   wnck_window_unpin(win))
X(ACTIVATE,                activate,                wnck_window_activate(win, 0))
X(ACTIVATE_TRANSIENT,      activate_transient,      wnck_window_activate_transient(win, 0))

#undef INCLUDE_STATEMENT_ACTIONS_ENUMS
#undef INCLUDE_STATEMENT_ACTIONS_EXECUTE_CODE
#undef INCLUDE_STATEMENT_ACTIONS_DUMP_CODE
#undef GENERATE_LEX_RULES
#undef X
