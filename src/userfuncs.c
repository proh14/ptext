#include <stdafx.h>
#include <userfuncs.h>
#include <utils.h>

char *args = NULL;

char *nextarg(void) {
  char *arg = token(args);
  return arg;
}

// Function from https://github.com/Swoorup/mysh
void strip_quotes(char *src, char *dest) {
  int n = strlen(src);
  if (n <= 1) {
    strcpy(dest, src);
    return;
  }

  int i;

  char lastquote = 0;
  int j = 0;

  for (i = 0; i < n; i++) {
    char c = src[i];
    if ((c == '\'' || c == '\"') && lastquote == 0) {
      lastquote = c;
    } else if (c == lastquote) {
      lastquote = 0;
    } else {
      dest[j++] = c;
    }
  }

  dest[j] = 0;
}

void user_setStatus(void) {
  char *arg1 = nextarg();
  if (arg1 == NULL) {
    setStatusMessage("arguments are not enough for \"%s\"", "set-status");
    return;
  }
  strip_quotes(arg1, arg1);
  setStatusMessage("%s", arg1);
}
