#include <cursor.h>
#include <input.h>
#include <options.h>
#include <ptext.h>
#include <rows.h>

int rowCxToRx(row *row, int cx) {
  int tabstop = getOption(O_TABSTOP);
  int rx = 0;
  int j;
  for (j = 0; j < cx; j++) {
    if (row->chars[j] == '\t') {
      rx += (tabstop - 1) - (rx % tabstop);
    }
    rx++;
  }
  return rx;
}

int rowRxToCx(row *row, int rx) {
  int tabstop = getOption(O_TABSTOP);
  int cur_rx = 0;
  int cx;
  for (cx = 0; cx < (int)row->len; cx++) {
    if (row->chars[cx] == '\t') {
      cur_rx += (tabstop - 1) - (cur_rx % tabstop);
    }
    cur_rx++;
    if (cur_rx > rx) {
      return cx;
    }
  }
  return cx;
}

void scroll(void) {
  conf.rx = 0;
  if (conf.cy < conf.numrows) {
    conf.rx = rowCxToRx(&conf.rows[conf.cy], conf.cx);
  }
  if (conf.cy < conf.rowoff) {
    conf.rowoff = conf.cy;
  }
  if (conf.cy >= conf.rowoff + conf.height - 3) {
    conf.rowoff = conf.cy - conf.height + 3;
  }
  if (conf.cx < conf.coloff) {
    conf.coloff = conf.cx;
  }
  if (conf.cx >= conf.coloff + conf.width) {
    conf.coloff = conf.cx - conf.width + 1;
  }
  if (conf.rx < conf.coloff) {
    conf.coloff = conf.rx;
  }
  if (conf.rx >= conf.coloff + conf.width) {
    conf.coloff = conf.rx - conf.width + 1;
  }
}

void moveCursor(int key) {
  row *row = (conf.cy >= conf.numrows) ? NULL : &conf.rows[conf.cy];

  switch (key) {
  case ARROW_LEFT:
    if (row && conf.cx < (int)row->len) {
      conf.cx++;
    } else if (row && conf.cx == (int)row->len && conf.cy < conf.numrows - 1) {
      conf.cy++;
      conf.cx = 0;
    }

    break;
  case ARROW_RIGHT:
    if (conf.cx != 0) {
      conf.cx--;
    } else if (conf.cy > 0) {
      conf.cy--;
      conf.cx = (int)conf.rows[conf.cy].len;
    }
    break;
  case ARROW_UP:
    if (conf.cy != 0) {
      conf.cy--;
    }
    break;
  case ARROW_DOWN:
    if (conf.cy < conf.numrows - 1) {
      conf.cy++;
    }
    break;
  }

  row = (conf.cy >= conf.numrows) ? NULL : &conf.rows[conf.cy];
  int rowlen = row ? (int)row->len : 0;
  if (conf.cx > rowlen) {
    conf.cx = rowlen;
  }
}
