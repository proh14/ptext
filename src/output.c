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

void drawLineNumber(struct screenBuffer *buff, int frow) {
  char linenum[6];
  int size = snprintf(linenum, sizeof(linenum), "%-5d", frow + 1);
  screenAppend(buff, "\x1b[1;39m", 7);
  screenAppend(buff, linenum, size);
  screenAppend(buff, "\x1b[m", 3);
}

void drawTilde(struct screenBuffer *buff) {
  screenAppend(buff, "~", 1);
  screenAppend(buff, "\x1b[K", 3);
}

void drawHighlighted(struct screenBuffer *buff, int frow) {
  int len = curbuf.rows[frow].renlen - curbuf.coloff;
  if (len < 0) {
    len = 0;
  }
  if (len > conf.width) {
    len = conf.width;
  }
  highlight(&curbuf.rows[frow].hl[curbuf.coloff],
            &curbuf.rows[frow].renchar[curbuf.coloff], buff, len);
  screenAppend(buff, "\x1b[m", 3);
  screenAppend(buff, "\x1b[K", 3);
}

void drawLine(struct screenBuffer *buff, int y) {
  int frow = curbuf.rowoff + y;

  if (frow >= curbuf.numrows || frow == -1) {
    drawTilde(buff);
    return;
  }

  if (!curbuf.rows[frow].redraw && !curbuf.redraw) {
    return;
  }

  drawLineNumber(buff, frow);
  drawHighlighted(buff, frow);

  curbuf.rows[frow].redraw = 0;
}

void drawWelcomeMessage(struct screenBuffer *buff, int y) {
  if (y != conf.height / 3) {
    drawTilde(buff);
    return;
  }
  char welcome[80];
  int welcomelen = snprintf(welcome, sizeof(welcome), WELCOME_MSG);
  int padding = (conf.width - welcomelen) / 2;
  if (padding) {
    screenAppend(buff, "~", 1);
    padding--;
  }
  while (padding--) {
    screenAppend(buff, " ", 1);
  }
  screenAppend(buff, welcome, welcomelen);
  screenAppend(buff, "\x1b[K", 3);
}

void drawAll(struct screenBuffer *buff) {
  for (int y = 0; y < conf.height - 2; y++) {
    if (curbuf.numrows == 0) {
      drawWelcomeMessage(buff, y);
    } else {
      drawLine(buff, y);
    }
    screenAppend(buff, "\r\n", 2);
  }
  curbuf.redraw = 0;
}
void drawStatusBar(struct screenBuffer *buff) {
  screenAppend(buff, "\x1b[7m", 4);
  char status[80], rstatus[80];
  int len = snprintf(status, sizeof(status), "%.20s - Buffer %d - %d lines %s",
                     curbuf.filename ? curbuf.filename : "[No Name]",
                     conf.current_buffer, curbuf.numrows,
                     curbuf.dirty ? "(modified)" : "");
  int rlen = snprintf(rstatus, sizeof(rstatus), "%d/%d", curbuf.cy + 1,
                      curbuf.numrows);
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

  conf.width -= 5;
  char s[32];
  drawAll(&buff);

  snprintf(s, sizeof(s), "\x1b[%d;%dH", (curbuf.cy - curbuf.rowoff) + 1,
           (curbuf.rx - curbuf.coloff) + 1 + 5);
  conf.width += 5;

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
