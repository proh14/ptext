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
  curbuf.rx = 0;
  if (curbuf.cy < curbuf.numrows) {
    curbuf.rx = rowCxToRx(&curbuf.rows[curbuf.cy], curbuf.cx);
  }
  if (curbuf.cy < curbuf.rowoff) {
    curbuf.rowoff = curbuf.cy;
  }
  if (curbuf.cy >= curbuf.rowoff + conf.height - 3) {
    curbuf.rowoff = curbuf.cy - conf.height + 3;
  }
  if (curbuf.cx < curbuf.coloff) {
    curbuf.coloff = curbuf.cx;
  }
  if (curbuf.cx >= curbuf.coloff + conf.width) {
    curbuf.coloff = curbuf.cx - conf.width + 1;
  }
  if (curbuf.rx < curbuf.coloff) {
    curbuf.coloff = curbuf.rx;
  }
  if (curbuf.rx >= curbuf.coloff + conf.width) {
    curbuf.coloff = curbuf.rx - conf.width + 1;
  }
}

void moveCursor(int key) {
  row *row = (curbuf.cy >= curbuf.numrows) ? NULL : &curbuf.rows[curbuf.cy];

  switch (key) {
  case ARROW_LEFT:
    if (row && curbuf.cx < (int)row->len) {
      curbuf.cx++;
    } else if (row && curbuf.cx == (int)row->len &&
               curbuf.cy < curbuf.numrows - 1) {
      curbuf.cy++;
      curbuf.cx = 0;
    }

    break;
  case ARROW_RIGHT:
    if (curbuf.cx != 0) {
      curbuf.cx--;
    } else if (curbuf.cy > 0) {
      curbuf.cy--;
      curbuf.cx = (int)curbuf.rows[curbuf.cy].len;
    }
    break;
  case ARROW_UP:
    if (curbuf.cy != 0) {
      curbuf.cy--;
    }
    break;
  case ARROW_DOWN:
    if (curbuf.cy < curbuf.numrows - 1) {
      curbuf.cy++;
    }
    break;
  }

  row = (curbuf.cy >= curbuf.numrows) ? NULL : &curbuf.rows[curbuf.cy];
  int rowlen = row ? (int)row->len : 0;
  if (curbuf.cx > rowlen) {
    curbuf.cx = rowlen;
  }
}
