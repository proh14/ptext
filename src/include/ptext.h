#pragma once
#ifndef _PTEXT_H_
#define _PTEXT_H_

#include <rows.h>
#include <termios.h>
#include <time.h>

#define TABSTOP 5

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

extern struct config conf;

void die(const char *s);

#endif
