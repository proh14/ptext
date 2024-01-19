#pragma once
#ifndef _PTEXT_H_
#define _PTEXT_H_

#include <rows.h>
#include <stdafx.h>

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
  char statusmsg[80];
  time_t statusmsg_time;

#ifdef _WIN32
  DWORD originalConsoleMode;
#else
  struct termios orig_termios;
#endif  // _WIN32
};

extern struct config conf;

void die(const char *s);

#endif
