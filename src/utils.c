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
