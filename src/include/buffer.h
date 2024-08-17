#pragma once
#ifndef _BUFFER_H
#define _BUFFER_H

#include <rows.h>

typedef struct buffer {
  row *rows;
  int numrows;
  int cx, cy;
  int rowoff;
  int coloff;
  int rx;
  int editable;
  int dirty;
  char *filename;
  int redraw;
} buffer;

buffer createBuffer(buffer *b, int editable);
void freeBuffer(buffer *b);

#endif
