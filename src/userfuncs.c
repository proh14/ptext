#include <files.h>
#include <options.h>
#include <output.h>
#include <ptext.h>
#include <stdafx.h>
#include <userfuncs.h>
#include <utils.h>

char *args = NULL;

char *nextarg(void) {
  int len;
  char *arg = token(args, &len);
  args += len + 1;
  return arg;
}

// Function from https://github.com/Swoorup/mysh
void strip_quotes(char *src, char *dest) {
  int n = strlen(src);
  if (n <= 1) {
    dest = src;
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

void user_set(void) {
  char *arg1 = nextarg();
  char *arg2 = nextarg();
  if (arg1 == NULL || arg2 == NULL) {
    setStatusMessage("arguments are not enough for \"%s\"", "set");
    return;
  }
  strip_quotes(arg1, arg1);
  strip_quotes(arg2, arg2);

  int val = atoi(arg2);
  if (val == 0 && strcmp(arg2, "0") != 0) {
    setStatusMessage("argument 2 is not a number");
    return;
  }

  int ret = setOption_char(arg1, val);

  if (!ret) {
    setStatusMessage("option \"%s\" not found", arg1);
  }
}

void user_gotoBuffer(void) {
  char *arg1 = nextarg();
  if (arg1 == NULL) {
    setStatusMessage("arguments are not enough for \"%s\"", "goto-buffer");
    return;
  }
  strip_quotes(arg1, arg1);
  int val = atoi(arg1);
  if (val == 0 && strcmp(arg1, "0") != 0) {
    setStatusMessage("argument 1 is not a number");
    return;
  }
  if (val < 0 || val >= conf.num_buffers) {
    setStatusMessage("buffer index out of range");
    return;
  }
  conf.current_buffer = val;
}

void user_edit(void) {
  char *arg1 = nextarg();
  if (arg1 == NULL) {
    setStatusMessage("arguments are not enough for \"%s\"", "edit");
    return;
  }
  strip_quotes(arg1, arg1);
  if (access(arg1, F_OK) == -1) {
    setStatusMessage("file \"%s\" not found", arg1);
    return;
  }
  openFile(arg1);
  setStatusMessage("Oppend File %s", arg1);
  refresh();
}
