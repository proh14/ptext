#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#include <buff.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <highlighter.h>
#include <lexer.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termio.h>
#include <time.h>
#include <unistd.h>

#define TABSTOP 4
#define CTRL_KEY(k) ((k)&0x1f)

typedef struct {
  char *chars;
  size_t len;
  char *renchar;
  size_t renlen;
  char *hl;
} row;

enum editorHighlight {
  HL_NORMAL = 0,
  HL_COMMENT,
  HL_MLCOMMENT,
  HL_KEYWORD1,
  HL_KEYWORD2,
  HL_STRING,
  HL_NUMBER,
  HL_MATCH,
};

enum editorKeys {
  BACKSPACE = 127,
  ARROW_UP = 500,
  ARROW_DOWN,
  ARROW_LEFT,
  ARROW_RIGHT,
  DEL_KEY,
};

struct config {
  int cx, cy;
  int rx;
  row *rows;
  int numrows;
  int rowoff;
  int coloff;
  int width;
  int height;
  int dirty;
  char *filename;
  int filenamelen;
  char statusmsg[80];
  time_t statusmsg_time;
  struct termios orig_termios;
};

struct config conf;

#define INIT_BUFF                                                              \
  { NULL, 0 }

void die(const char *s) {
  perror(s);
  exit(1);
}

void disableRawMode(void) {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &conf.orig_termios) == -1)
    die("tcsetattr");
}

void updateRow(row *row) {
  int tabs = 0;
  size_t j;
  for (j = 0; j < row->len; j++)
    if (row->chars[j] == '\t')
      tabs++;
  free(row->renchar);
  row->renchar = malloc(row->len + tabs * (TABSTOP - 1) + 1);
  int idx = 0;
  for (j = 0; j < row->len; j++) {
    if (row->chars[j] == '\t') {
      row->renchar[idx++] = ' ';
      while (idx % TABSTOP != 0)
        row->renchar[idx++] = ' ';
    } else {
      row->renchar[idx++] = row->chars[j];
    }
  }
  row->renchar[idx] = '\0';
  row->renlen = idx;
  Lexer l = {.content = row->renchar, .contentlen = row->renlen, .cursor = 0};
  row->hl = realloc(row->hl, row->renlen + 1);
  prehighlight(row->hl, &l);
}

void rowAppend(char *s, size_t len, int at) {
  if (at < 0 || at > conf.numrows)
    return;
  conf.rows = realloc(conf.rows, sizeof(row) * (conf.numrows + 1));
  memmove(&conf.rows[at + 1], &conf.rows[at],
          sizeof(row) * (conf.numrows - at));
  conf.rows[at].chars = malloc(len + 1);
  memcpy(conf.rows[at].chars, s, len);
  conf.rows[at].chars[len] = '\0';
  conf.rows[at].len = len;
  conf.rows[at].renlen = 0;
  conf.rows[at].renchar = NULL;
  conf.rows[at].hl = NULL;
  conf.numrows++;
  conf.dirty++;
  updateRow(&conf.rows[at]);
}

void rowAppendString(row *row, char *s, size_t len) {
  row->chars = realloc(row->chars, row->len + len + 1);
  memcpy(&row->chars[row->len], s, len);
  row->len += len;
  row->chars[row->len] = '\0';
  updateRow(row);
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

void freeRow(row *row) {
  free(row->renchar);
  free(row->chars);
  free(row->hl);
}
void delRow(int at) {
  if (at < 0 || at >= conf.numrows)
    return;
  freeRow(&conf.rows[at]);
  memmove(&conf.rows[at], &conf.rows[at + 1],
          sizeof(row) * (conf.numrows - at - 1));
  conf.numrows--;
  conf.dirty++;
}

void rowDelChar(row *row, int at) {
  if (at < 0 || at >= (int)row->len)
    return;
  memmove(&row->chars[at], &row->chars[at + 1], row->len - at);
  row->len--;
  updateRow(row);
  conf.dirty++;
}

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

void openFile(const char *s) {
  FILE *file = fopen(s, "r");
  if (!file) {
    die("fopen");
  }
  free(conf.filename);
  conf.filenamelen = strlen(s) + 1;
  conf.filename = malloc(conf.filenamelen);
  snprintf(conf.filename, conf.filenamelen, "%s", s);
  char *line = NULL;
  size_t cap = 0;
  int len;
  while ((len = getline(&line, &cap, file)) != -1) {
    while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
      len--;
    }
    rowAppend(line, len, conf.numrows);
  }
  free(line);
  (void)fclose(file);
  conf.dirty = 0;
}

void init(void) {
  conf.numrows = 0;
  conf.rows = NULL;
  conf.cx = 0;
  conf.cy = 0;
  conf.dirty = 0;
  conf.rowoff = 0;
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  conf.width = w.ws_col;
  conf.height = w.ws_row;
  conf.filename = NULL;
}

void freeall(void) {
  if (conf.rows != NULL && conf.numrows != 0) {
    int i;
    for (i = 0; i < conf.numrows; i++) {
      free(conf.rows[i].chars);
      free(conf.rows[i].renchar);
      free(conf.rows[i].hl);
    }
    free(conf.rows);
    conf.rows = NULL;
    free(conf.filename);
  }
}

void done(void) {
  freeall();
  disableRawMode();
}

void enableRawMode(void) {
  if (tcgetattr(STDIN_FILENO, &conf.orig_termios) == -1)
    die("tcgetattr");
  atexit(done);

  struct termios raw = conf.orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    die("tcsetattr");
}

void rowInsertChar(row *row, int at, int c) {
  if (at < 0 || at > (int)row->len)
    at = row->len;
  row->chars = realloc(row->chars, row->len + 2);
  memmove(&row->chars[at + 1], &row->chars[at], row->len - at + 1);
  row->len++;
  row->chars[at] = c;
  updateRow(row);
}

void insertAChar(int c) {
  if (conf.cy == conf.numrows) {
    rowAppend("", 0, 0);
  }
  rowInsertChar(&conf.rows[conf.cy], conf.cx, c);
  conf.cx++;
  conf.dirty++;
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

void drawAll(struct buff *buff) {
  int y;
  int frow;
  for (y = 0; y < conf.height - 2; y++) {
    frow = conf.rowoff + y;
    if (frow >= conf.numrows || frow == -1) {
      buffAppend(buff, "~", 1);
    } else {
      int len = conf.rows[frow].renlen - conf.coloff;
      if (len < 0)
        len = 0;
      if (len > conf.width)
        len = conf.width;
      highlight(&conf.rows[frow].hl[conf.coloff],
                &conf.rows[frow].renchar[conf.coloff], buff, len);
    }
    buffAppend(buff, "\x1b[K", 3);
    buffAppend(buff, "\r\n", 2);
    buffAppend(buff, "\x1b[0m", 4);
  }
}

void drawStatusBar(struct buff *buff) {
  buffAppend(buff, "\x1b[7m", 4);
  char status[80], rstatus[80];
  int len = snprintf(status, sizeof(status), "%.20s - %d lines %s",
                     conf.filename ? conf.filename : "[No Name]", conf.numrows,
                     conf.dirty ? "(modified)" : "");
  int rlen =
      snprintf(rstatus, sizeof(rstatus), "%d/%d", conf.cy + 1, conf.numrows);
  if (len > conf.width)
    len = conf.width;
  buffAppend(buff, status, len);
  while (len < conf.width) {
    if (conf.width - len == rlen) {
      buffAppend(buff, rstatus, rlen);
      break;
    } else {
      buffAppend(buff, " ", 1);
      len++;
    }
  }
  buffAppend(buff, "\x1b[m", 3);
  buffAppend(buff, "\r\n", 2);
}

void drawStatusMessage(struct buff *buff) {
  buffAppend(buff, "\x1b[K", 3);
  int msglen = strlen(conf.statusmsg);
  if (msglen > conf.width)
    msglen = conf.width;
  if (msglen && time(NULL) - conf.statusmsg_time < 5)
    buffAppend(buff, conf.statusmsg, msglen);
}

void setStatusMessage(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(conf.statusmsg, sizeof(conf.statusmsg), fmt, ap);
  va_end(ap);
  conf.statusmsg_time = time(NULL);
}

int rowCxToRx(row *row, int cx) {
  int rx = 0;
  int j;
  for (j = 0; j < cx; j++) {
    if (row->chars[j] == '\t')
      rx += (TABSTOP - 1) - (rx % TABSTOP);
    rx++;
  }
  return rx;
}

void scroll(void) {
  conf.rx = 0;
  if (conf.cy < conf.numrows) {
    conf.rx = rowCxToRx(&conf.rows[conf.cy], conf.cx);
  }
  conf.rx = conf.cx;
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

void refresh(void) {
  scroll();
  struct buff buff = INIT_BUFF;
  buffAppend(&buff, "\x1b[?25l", 6);
  buffAppend(&buff, "\x1b[H", 3);
  char s[32];
  drawAll(&buff);
  snprintf(s, sizeof(s), "\x1b[%d;%dH", (conf.cy - conf.rowoff) + 1,
           (conf.rx - conf.coloff) + 1);
  drawStatusBar(&buff);
  drawStatusMessage(&buff);
  buffAppend(&buff, s, strlen(s));
  buffAppend(&buff, "\x1b[?25h", 6);
  write(1, buff.chars, buff.len);
  free(buff.chars);
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

void save(void) {
  if (conf.filename == NULL)
    return;
  int len;
  char *buf = rowsToString(&len);
  int fd = open(conf.filename, O_RDWR | O_CREAT, 0644);
  if (fd != -1) {
    if (ftruncate(fd, len) != -1) {
      if (write(fd, buf, len) == len) {
        close(fd);
        free(buf);
        conf.dirty = 0;
        setStatusMessage("%d bytes written to disk", len);
        return;
      }
    }
    close(fd);
  }
  free(buf);
  setStatusMessage("Can't save! I/O error: %s", strerror(errno));
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
      conf.cx = conf.rows[conf.cy].len;
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
  int rowlen = row ? row->len : 0;
  if (conf.cx > rowlen) {
    conf.cx = rowlen;
  }
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
  case CTRL('h'):
    if (c == DEL_KEY)
      moveCursor(ARROW_RIGHT);
    delChar();
    break;

  case '\r':
    insertNewLine();
    break;

  case CTRL('s'):
    save();
    break;
  case CTRL('l'):
  case '\x1b':
    break;

  default:
    insertAChar(c);
    break;
  }
}

int main(int argc, char *argv[]) {
  enableRawMode();
  init();
  if (argc >= 2) {
    openFile(argv[1]);
  }
  while (1) {
    refresh();
    procKey();
  }
  return 0;
}
