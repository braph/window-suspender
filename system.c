#include "system.h"
#include <sys/wait.h>
#include <stdlib.h>

#define WINDOW_IS_(STATE) \
  [GET_BIT_POSITION_OF_FLAG(WNCK_WINDOW_STATE_ ## STATE)-1] = "WINDOW_IS_" #STATE

static const char* const WINDOW_IS[WNCK_WINDOW_STATE_HIGHEST_BIT] = {
  WINDOW_IS_(MINIMIZED),
  WINDOW_IS_(MAXIMIZED_HORIZONTALLY),
  WINDOW_IS_(MAXIMIZED_VERTICALLY),
  WINDOW_IS_(SHADED),
  WINDOW_IS_(SKIP_PAGER),
  WINDOW_IS_(SKIP_TASKLIST),
  WINDOW_IS_(STICKY),
  WINDOW_IS_(HIDDEN),
  WINDOW_IS_(FULLSCREEN),
  WINDOW_IS_(DEMANDS_ATTENTION),
  WINDOW_IS_(URGENT),
  WINDOW_IS_(ABOVE),
  WINDOW_IS_(BELOW),
  WINDOW_IS_(INACTIVE_WORKSPACE)
};

static void export_window_variables(WnckWindow *win) {
  const char *str;
  char buf[32];

#define export(NAME, VALUE) \
  setenv(NAME, VALUE, 1)

#define export_safe(NAME, VALUE) \
  str = VALUE; export(NAME, (str ? str : ""))

#define export_int(NAME, VALUE) \
  sprintf(buf, "%d", VALUE); export(NAME, buf)

#define export_long(NAME, VALUE) \
  sprintf(buf, "%lu", VALUE); export(NAME, buf)

  unsigned int states = window_get_state(win);
  for (unsigned int i = 0; i < WNCK_WINDOW_STATE_HIGHEST_BIT; ++i)
    if (WINDOW_IS[i])
      export(WINDOW_IS[i], (states & (1 << i) ? "1" : "0"));

  int x, y, width, height;
  wnck_window_get_geometry(win, &x, &y, &width, &height);
  export_int("WINDOW_X", x);
  export_int("WINDOW_Y", y);
  export_int("WINDOW_WIDTH", width);
  export_int("WINDOW_HEIGHT", height);
  export_int("WINDOW_STACKPOSITION", window_get_stackposition(win));
  export_int("WINDOW_WORKSPACE_NUMBER", window_get_workspace_number(win));
  export_int("WINDOW_PID", wnck_window_get_pid(win));
  export_long("WINDOW_XID", wnck_window_get_xid(win));
  export_safe("WINDOW_TITLE", wnck_window_get_name(win));
  export_safe("WINDOW_CLASS", wnck_window_get_class_group_name(win));
  export_safe("WINDOW_GROUP", wnck_window_get_class_instance_name(win));
  export_safe("WINDOW_ICON_NAME", wnck_window_get_icon_name(win));
  export_safe("WINDOW_ROLE", wnck_window_get_role(win));
  export("WINDOW_TYPE", window_type_to_str(wnck_window_get_window_type(win)));
  export("WINDOW_STATE", window_states_to_str(states));
  export("WINDOW_WORKSPACE", window_get_workspace_name(win));
}

int system_with_winenv(const char *string, WnckWindow *win) {
  export_window_variables(win);
  int child, retval;
  switch ((child = fork())) {
    case -1:
      return 0;
    case 0:
      execl("/bin/sh", "sh", "-c", string, (char*) NULL);
      exit(1);
    default:
      waitpid(child, &retval, WUNTRACED);
      if (WIFEXITED(retval))
        return ! WEXITSTATUS(retval);
      else
        return 0;
  }
}
