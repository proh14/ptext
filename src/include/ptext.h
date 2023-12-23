#ifndef _PTEXT_H_
#define _PTEXT_H_

#define TABSTOP 5

#include <rows.h>
#include <termios.h>
#include <time.h>

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

#endif
