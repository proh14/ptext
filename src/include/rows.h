#pragma once
#ifndef _ROWS_H_
#define _ROWS_H_
#include <stddef.h>

typedef struct row {
  char *chars;
  size_t len;
  char *renchar;
  size_t renlen;
  char *hl;
  int in_comment;
  int idx;
  int redraw;
} row;

void updateRow(row *row);
void rowAppend(char *s, size_t len, int at);
void rowAppendString(row *row, char *s, size_t len);
void freeRow(row *row);
void delRow(int at);
void rowDelChar(row *row, int at);
void rowInsertChar(row *row, int at, int c);
#endif
