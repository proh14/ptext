#include <cursor.h>
#include <errno.h>
#include <input.h>
#include <ptext.h>
#include <rows.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#define CTRL_KEY(k) ((k)&0x1f)

void delChar(void) {
  if (conf.cy == conf.numrows)
    return;
  if (conf.cx == 0 && conf.cy == 0)
    return;
  row *row = &conf.rows[conf.cy];
  if (conf.cx > 0) {
    rowDelChar(row, conf.cx - 1);
    conf.cx--;
  } else {
    conf.cx = conf.rows[conf.cy - 1].len;
    rowAppendString(&conf.rows[conf.cy - 1], row->chars, row->len);
    delRow(conf.cy);
    conf.cy--;
  }
}

void insertAChar(int c) {
  if (conf.cy == conf.numrows) {
    rowAppend("", 0, 0);
  }
  rowInsertChar(&conf.rows[conf.cy], conf.cx, c);
  conf.cx++;
  conf.dirty++;
}

void insertNewLine(void) {
  if (conf.cx == 0) {
    rowAppend("", 0, conf.cy);
  } else {
    row *row = &conf.rows[conf.cy];
    rowAppend(&row->chars[conf.cx], row->len - conf.cx, conf.cy + 1);
    row = &conf.rows[conf.cy];
    row->len = conf.cx;
    row->chars[row->len] = '\0';
    updateRow(row);
  }
  conf.cy++;
  conf.cx = 0;
}

int readKey(void) {
  int nr;
  int c = '\0';
  while ((nr = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nr == -1 && errno != EAGAIN)
      die("read");
  }
  if (c == '\x1b') {
    char ecode[2];
    if (read(0, &ecode[0], 1) != 1) {
      return '\x1b';
    }
    if (read(0, &ecode[1], 1) != 1) {
      return '\x1b';
    }
    if (ecode[0] == '[') {
      switch (ecode[1]) {
      case 'A':
        return ARROW_UP;
        break;
      case 'B':
        return ARROW_DOWN;
        break;
      case 'C':
        return ARROW_LEFT;
        break;
      case 'D':
        return ARROW_RIGHT;
        break;
      case 3:
        return DEL_KEY;
        break;
      }
    }
  }
  return c;
}

void procKey(void) {
  int c = readKey();
  switch (c) {
  case CTRL_KEY('q'):
    write(1, "\x1b[2J", 4);
    write(1, "\x1b[H", 3);
    exit(0);
    break;
  case ARROW_DOWN:
  case ARROW_UP:
  case ARROW_LEFT:
  case ARROW_RIGHT:
    moveCursor(c);
    break;
  case BACKSPACE:
  case DEL_KEY:
  case CTRL_KEY('h'):
    if (c == DEL_KEY)
      moveCursor(ARROW_RIGHT);
    delChar();
    break;

  case '\r':
    insertNewLine();
    break;

  case CTRL_KEY('s'):
    save();
    break;

  case CTRL_KEY('l'):
  case '\x1b':
    break;

  default:
    insertAChar(c);
    break;
  }
}
