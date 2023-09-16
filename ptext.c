#define _DEFAULT_SOURCE
#define _BSD_SOURCE
#define _GNU_SOURCE

#include <ctype.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termio.h>
#include <unistd.h>

#define TABSTOP 4
#define CTRL_KEY(k) ((k)&0x1f)

typedef struct {
  char *chars;
  size_t len;
  char *renchar;
  size_t renlen;
} row;

struct {
  int cx, cy;
  struct termios orig_termios;
  row *rows;
  int numrows;
  size_t width;
  size_t hight;
} conf;

void die(const char *s) {
  perror(s);
  exit(1);
}

void disableRawMode(void) {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &conf.orig_termios) == -1)
    die("tcsetattr");
}

void rowAppend(char *s, size_t len) {
  conf.rows = realloc(conf.rows, sizeof(row) * (conf.numrows + 1));

  int at = conf.numrows;
  conf.rows[at].chars = malloc(len + 1);
  memcpy(conf.rows[at].chars, s, len);
  conf.rows[at].chars[len] = '\0';
  conf.rows[at].len = len;
  conf.rows[at].renlen = 0;
  conf.rows[at].renchar = NULL;
  conf.numrows++;
}

void fixTabs(void) {
  row *row = &conf.rows[conf.numrows - 1];
  int tabs = 0;
  int j;
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
}

void openFile(const char *s) {
  FILE *file = fopen(s, "r");
  if (!file) {
    die("fopen");
  }

  char *line = NULL;
  size_t cap = 0;
  int len;
  while ((len = getline(&line, &cap, file)) != -1) {
    while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
      len--;
    }
    rowAppend(line, len);
    fixTabs();
  }
  free(line);
  fclose(file);
}

void init(void) {
  conf.numrows = 0;
  conf.rows = NULL;
  conf.cx = 0;
  conf.cy = 0;
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  conf.width = w.ws_col;
  conf.hight w.ws_row;
}

void freeall(void) {
  if (conf.rows != NULL && conf.numrows != 0) {
    int i;
    for (i = 0; i < conf.numrows; i++) {
      free(conf.rows[i].chars);
      free(conf.rows[i].renchar);
    }
    free(conf.rows);
  }
}

void done(void) {
  disableRawMode();
  freeall();
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

int readKey(void) {
  int nr;
  int c = '\0';
  while ((nr = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nr == -1 && errno != EAGAIN)
      die("read");
  }
  return c;
}

void drawAll(void) {
  int y, x;
  for (y = 0; y < conf.hight; y++) {
    if (conf.numrows == 0 && y == conf.hight / 3) {
    }
  }
}

void refresh(void) { drawAll(); }

void procKey(void) {
  int c = readKey();
  switch (c) {
  case CTRL_KEY('q'):
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
    exit(0);
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
