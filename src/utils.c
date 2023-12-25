#include <ctype.h>
#include <input.h>
#include <output.h>
#include <ptext.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utils.h>

void setStatusMessage(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(conf.statusmsg, sizeof(conf.statusmsg), fmt, ap);
  va_end(ap);
  conf.statusmsg_time = time(NULL);
}

char *rowsToString(int *buflen) {
  int totlen = 0;
  int j;
  for (j = 0; j < conf.numrows; j++)
    totlen += conf.rows[j].len + 1;
  *buflen = totlen;
  char *buf = malloc(totlen);
  char *p = buf;
  for (j = 0; j < conf.numrows; j++) {
    memcpy(p, conf.rows[j].chars, conf.rows[j].len);
    p += conf.rows[j].len;
    *p = '\n';
    p++;
  }
  return buf;
}

char *getPrompt(char *promt) {
  size_t bufcap = 100;
  size_t bufsize = 0;
  const int ADD_SIZE = 16;
  char *buf = malloc(100);
  buf[0] = '\0';

  while (1) {
    setStatusMessage(promt, buf);
    refresh();

    int c = readKey();
    if (c == '\r') {
      if (bufsize != 0) {
        setStatusMessage("");
        return buf;
      }
    } else if (c == DEL_KEY || c == CTRL_KEY('h') || c == BACKSPACE) {
      if (bufsize != 0) {
        buf[bufsize - 1] = '\0';
      }
    } else if (c == ESC_KEY) {
      setStatusMessage("");
      free(buf);
      return NULL;
    } else if (!iscntrl(c) && c < 128) {
      if (bufcap == bufsize - 1) {
        bufcap += ADD_SIZE;
        buf = realloc(buf, bufsize);
        if (buf == NULL) {
          die("realloc");
        }
      }
      buf[bufsize + 1] = '\0';
      buf[bufsize] = c;
      bufsize++;
    }
  }
}
