#include "system.h"
#include <sys/wait.h>
#include <stdlib.h>
#include <ctype.h>

int system_with_winenv(const char *string, WnckWindow *win) {
  const char *str;
  char buf[512];
  int states = wnck_window_get_state(win);

#define export(NAME, VALUE) \
  setenv(NAME, VALUE, 1)

#define export_safe(NAME, VALUE) \
  str = VALUE; export(NAME, (str ? str : ""))

  export_safe("WINDOW_TITLE", wnck_window_get_name(win));
  export_safe("WINDOW_CLASS", wnck_window_get_class_group_name(win));
  export_safe("WINDOW_GROUP", wnck_window_get_class_instance_name(win));
  export_safe("WINDOW_TYPE", window_type_to_str(wnck_window_get_window_type(win)));
  export_safe("WINDOW_STATE", window_states_to_str(states));
  sprintf(buf, "%d", wnck_window_get_pid(win));
  export("WINDOW_PID", buf);
  sprintf(buf, "%d", window_get_stackposition(win));
  export("WINDOW_STACKPOSITION", buf);

  for (unsigned int i = 0; i <= 16; ++i) {
    if ((str = window_state_to_str(1 << i))) {
      sprintf(buf, "WINDOW_IS_%s", str);
      for (char *c = buf + sizeof("WINDOW_IS"); *c; *c = toupper(*c), ++c);
      export(buf, (states & (1 << i) ? "1" : "0"));
    }
  }

  int child, retval;
  switch ((child = fork())) {
    case -1: return 0;
    case 0:  execl("/bin/sh", "sh", "-c", string, (char*) NULL);
    default:
             waitpid(child, &retval, WUNTRACED);
             if (WIFEXITED(retval))
               return ! WEXITSTATUS(retval);
             else
               return 0;
  }
}
