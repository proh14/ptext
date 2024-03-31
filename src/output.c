#include <cursor.h>
#include <highlighter.h>
#include <output.h>
#include <ptext.h>
#include <screen.h>
#include <stdafx.h>
#include <stdarg.h>
#include <stdio.h>

#define INIT_BUFF                                                              \
  { NULL, 0 }

void drawAll(struct screenBuffer *buff) {
  int y;
  int frow;
  for (y = 0; y < conf.height - 2; y++) {
    frow = conf.rowoff + y;
    if (frow >= conf.numrows || frow == -1) {
      screenAppend(buff, "~", 1);
    } else {
      int len = conf.rows[frow].renlen - conf.coloff;
      if (len < 0)
        len = 0;
      if (len > conf.width)
        len = conf.width;
      highlight(&conf.rows[frow].hl[conf.coloff],
                &conf.rows[frow].renchar[conf.coloff], buff, len);

      screenAppend(buff, "\x1b[m", 3);
    }
    screenAppend(buff, "\x1b[K", 3);
    screenAppend(buff, "\r\n", 2);
  }
}

void drawStatusBar(struct screenBuffer *buff) {
  screenAppend(buff, "\x1b[7m", 4);
  char status[80], rstatus[80];
  int len = snprintf(status, sizeof(status), "%.20s - %d lines %s",
                     conf.filename ? conf.filename : "[No Name]", conf.numrows,
                     conf.dirty ? "(modified)" : "");
  int rlen =
      snprintf(rstatus, sizeof(rstatus), "%d/%d", conf.cy + 1, conf.numrows);
  if (len > conf.width)
    len = conf.width;
  screenAppend(buff, status, len);
  while (len < conf.width) {
    if (conf.width - len == rlen) {
      screenAppend(buff, rstatus, rlen);
      break;
    } else {
      screenAppend(buff, " ", 1);
      len++;
    }
  }
  screenAppend(buff, "\x1b[m", 3);
  screenAppend(buff, "\r\n", 2);
}

void drawStatusMessage(struct screenBuffer *buff) {
  screenAppend(buff, "\x1b[K", 3);
  int msglen = strlen(conf.statusmsg);
  if (msglen > conf.width)
    msglen = conf.width;
  if (msglen && time(NULL) - conf.statusmsg_time < 5)
    screenAppend(buff, conf.statusmsg, msglen);
}

void refresh(void) {
  scroll();
  struct screenBuffer buff = INIT_BUFF;

  screenAppend(&buff, "\x1b[?25l", 6);
  screenAppend(&buff, "\x1b[H", 3);

  char s[32];
  drawAll(&buff);

  snprintf(s, sizeof(s), "\x1b[%d;%dH", (conf.cy - conf.rowoff) + 1,
           (conf.rx - conf.coloff) + 1);

  drawStatusBar(&buff);
  drawStatusMessage(&buff);

  screenAppend(&buff, s, strlen(s));
  screenAppend(&buff, "\x1b[?25h", 6);

#ifdef _WIN32
  DWORD written;
  WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), buff.chars, buff.len, &written,
                NULL);
#else
  write(1, buff.chars, buff.len);
#endif // _WIN32

  free(buff.chars);
}
