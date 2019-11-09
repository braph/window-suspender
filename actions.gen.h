#if 0
  This file is used for generating the code for window actions (maximize, shade, ...)
  in statement.c and enums in statement.h.

  It may also be used for generating Lex rules:
    cpp -P -D GENERATE_LEX_RULES "actions.gen.h"

  Usage:
    #define <COMMAND>
    #include "actions.gen.h"
#endif

#if defined ( INCLUDE_STATEMENT_ACTIONS_ENUMS )
#define o(UPPER, ...) \
  STATEMENT_ ## UPPER,

#elif defined ( INCLUDE_STATEMENT_ACTIONS_EXECUTE_CODE )
#define o(UPPER, LOWER, CODE) \
  case STATEMENT_ ## UPPER: \
    CODE; \
  break;

#elif defined ( INCLUDE_STATEMENT_ACTIONS_DUMP_CODE )
#define o(UPPER, LOWER, ...) \
  case STATEMENT_ ## UPPER: \
    outf(#LOWER); \
  break;

#elif defined ( GENERATE_LEX_RULES )
#define o(UPPER, LOWER, ...) \
  LOWER return yylval.action_type=STATEMENT_ ## UPPER , ACTION_TYPE;

#else
#error "Invalid command specified"
#endif

/* UPPER                   LOWER                    CODE */
o(CLOSE,                   close,                   wnck_window_close(win, 0))
o(MINIMIZE,                minimize,                wnck_window_minimize(win))
o(UNMINIMIZE,              unminimize,              wnck_window_unminimize(win, 0))
o(MAXIMIZE,                maximize,                wnck_window_maximize(win))
o(UNMAXIMIZE,              unmaximize,              wnck_window_unmaximize(win))
o(MAXIMIZE_HORIZONTALLY,   maximize_horizontally,   wnck_window_maximize_horizontally(win))
o(UNMAXIMIZE_HORIZONTALLY, unmaximize_horizontally, wnck_window_unmaximize_horizontally(win))
o(MAXIMIZE_VERTICALLY,     maximize_vertically,     wnck_window_maximize_vertically(win))
o(UNMAXIMIZE_VERTICALLY,   unmaximize_vertically,   wnck_window_unmaximize_vertically(win))
o(SHADE,                   shade,                   wnck_window_shade(win))
o(UNSHADE,                 unshade,                 wnck_window_unshade(win))
o(MAKE_ABOVE,              make_above,              wnck_window_make_above(win))
o(UNMAKE_ABOVE,            unmake_above,            wnck_window_unmake_above(win))
o(MAKE_BELOW,              make_below,              wnck_window_make_below(win))
o(UNMAKE_BELOW,            unmake_below,            wnck_window_unmake_below(win))
o(STICK,                   stick,                   wnck_window_stick(win))
o(UNSTICK,                 unstick,                 wnck_window_unstick(win))
o(SET_SKIP_PAGER,          set_skip_pager,          wnck_window_set_skip_pager(win, 1))
o(UNSET_SKIP_PAGER,        unset_skip_pager,        wnck_window_set_skip_pager(win, 0))
o(SET_SKIP_TASKLIST,       set_skip_tasklist,       wnck_window_set_skip_tasklist(win, 1))
o(UNSET_SKIP_TASKLIST,     unset_skip_tasklist,     wnck_window_set_skip_tasklist(win, 0))
o(SET_FULLSCREEN,          set_fullscreen,          wnck_window_set_fullscreen(win, 1))
o(UNSET_FULLSCREEN,        unset_fullscreen,        wnck_window_set_fullscreen(win, 0))
o(PIN,                     pin,                     wnck_window_pin(win))
o(UNPIN,                   unpin,                   wnck_window_unpin(win))
o(ACTIVATE,                activate,                wnck_window_activate(win, 0))
o(ACTIVATE_TRANSIENT,      activate_transient,      wnck_window_activate_transient(win, 0))

#undef INCLUDE_STATEMENT_ACTIONS_ENUMS
#undef INCLUDE_STATEMENT_ACTIONS_EXECUTE_CODE
#undef INCLUDE_STATEMENT_ACTIONS_DUMP_CODE
#undef GENERATE_LEX_RULES
#undef o
