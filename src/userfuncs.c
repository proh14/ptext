#include <stdafx.h>
#include <userfuncs.h>
#include <utils.h>

char *args = NULL;

char *nextarg(void) {
  char *arg = token(args);
  return arg;
}

void user_setStatus(void) {
  char *arg1 = nextarg();
  if (arg1 == NULL) {
    setStatusMessage("arguments are not enough for \"%s\"", "set-status");
    return;
  }
  setStatusMessage("%s", arg1);
}
