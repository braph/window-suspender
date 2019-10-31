#include <libwnck/libwnck.h>
#include "xapplication.h"
#include "common.h"

xapplication* xapplication_new(int pid, WnckApplication *app) {
   xapplication* xapp = calloc(1, sizeof(xapplication));
   xapp->pid          = pid;
   xapp->app          = app;
   xapp->windows      = calloc(1, sizeof(void*));
   xapp->windows_size = 1;
   return xapp;
}

void xapplication_add_window(xapplication *xapp, WnckWindow *win) {
   for (int i = 0; i < xapp->windows_size; ++i) {
      if (xapp->windows[i] == 0) {
         xapp->windows[i] = win;
         return;
      }
   }

   xapp->windows = realloc(xapp->windows, ++xapp->windows_size * sizeof(void*));
   xapp->windows[xapp->windows_size - 1] = win;
}

void xapplication_remove_window(xapplication *xapp, WnckWindow *win) {
   for (int i = 0; i < xapp->windows_size; ++i) {
      if (xapp->windows[i] == win) {
         xapp->windows[i] = 0;
         return;
      }
   }
}

static void dump(xapplication *xapp) {
   debug("[ PID=%d ]", xapp->pid);

   for (int i = 0; i < xapp->windows_size; ++i) {
      if (xapp->windows[i] != NULL)
         debug("\n  %s", windump(xapp->windows[i]));
   }

   debug("\n");
}

// TODO!
void xapplication_apply_rules(xapplication *xapp) {
   dump(xapp);
}

   /*
   // global config
   debug("xapplication::apply_rules(): %s", this.to_string());
   if (xapp->windows_size == 0) {
      debug("-> xapplication::apply_rules(): no windows (yet)");
      return;
    }

    Array<IStatement> dry = null;
    XWindow? xwin0 = null;

    foreach (XWindow xwin in windows.get_values()) {
      dry = new Array<IStatement>();
      config.execute(new RuleArg(xwin, this), dry);
      if (dry.length == 0) {
        this.resume(0); // TODO: make configurable
        return;
      }
      xwin0 = xwin;
    }

    if (xwin0 != null)
      for (int i = 0; i < dry.length; ++i)
        dry.index(i).execute(new RuleArg(xwin0, this));
        */

/*
  public void cancelSuspend() {
    if (suspendJob != null)
      suspendJob.cancel();
    suspendJob = null;
  }

  public void cancelResume() {
    if (resumeJob != null)
      resumeJob.cancel();
    resumeJob = null;
  }

  public void resume(int resume_delay_millis) {
    cancelSuspend();
    if (resumeJob == null)
      resumeJob = new ResumeJob(this, resume_delay_millis);
  }

  public void suspend(int suspend_delay_seconds, int refresh_seconds) {
    cancelResume();
    if (suspendJob == null)
      suspendJob = new SuspendJob(this, suspend_delay_seconds, refresh_seconds);
  }
  */

/*
  public void sendSigStop() {
    debug("STOPPED: %s", to_string());
    //return;
    Posix.kill(pid, Posix.Signal.STOP);
  }

  public void sendSigCont() {
    debug("CONTINUED: %s", to_string());
    //return;
    Posix.kill(pid, Posix.Signal.CONT);
  }
  */

/*
  public string to_string() {
    string r = "[ PID=%d ]".printf(pid);
    if (windows.size() > 0) {
      foreach (XWindow win in windows.get_values()) {
        r += "\n ";
        r += new RuleArg(win, this).to_string();
      };
    }
    return r;
  }
}
*/
