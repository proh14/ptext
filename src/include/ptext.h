#pragma once
#ifndef _PTEXT_H_
#define _PTEXT_H_

#define NAME "ptext"
#define VERSION "1.0.0"
#define MAINTAINER "proh14"
#define LICENSE "BSD-2-Clause"

#define RCFILE "/.ptextrc"

#define curbuf conf.buffers[conf.current_buffer]

#include <buffer.h>
#include <config.h>
#include <rows.h>
#include <stdafx.h>

struct config {
  buffer buffers[MAX_BUFFERS];
  int current_buffer;
  int num_buffers;
  int width;
  int height;
  char statusmsg[80];
  time_t statusmsg_time;

#ifdef _WIN32
  DWORD originalConsoleMode;
#else
  struct termios orig_termios;
#endif // _WIN32
};

extern struct config conf;

void die(const char *s);

#endif
