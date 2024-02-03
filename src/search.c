#include <cursor.h>
#include <input.h>
#include <lexer.h>
#include <ptext.h>
#include <rows.h>
#include <search.h>
#include <stdafx.h>
#include <utils.h>

void searchCallBack(char *query, int c) {
  static int direction = 1;
  static int last_match = -1;

  switch (c) {
  case '\r':
  case '\x1b':
    direction = 1;
    last_match = 0;
    return;
    break;
  case ARROW_UP:
    direction = -1;
    break;
  case ARROW_DOWN:
    direction = 1;
    break;
  }
  if (last_match == -1)
    direction = 1;
  int current = last_match;
  int i;
  for (i = 0; i < conf.numrows; i++) {
    current += direction;
    if (current == -1)
      current = conf.numrows - 1;
    else if (current == conf.numrows)
      current = 0;
    row *row = &conf.rows[current];
    char *match = strstr(row->renchar, query);
    if (match) {
      last_match = current;
      conf.cy = current;
      conf.cx = rowRxToCx(row, match - row->renchar);
      conf.rowoff = conf.numrows;
      memset(&row->hl[match - row->renchar], TOKEN_MATCH, strlen(query));
      break;
    }
  }
}

void search(void) {
  int saved_cx = conf.cx;
  int saved_cy = conf.cy;
  int saved_rowoff = conf.rowoff;
  char *query = getPrompt("Search: %s", &searchCallBack);
  if (query) {
    free(query);
  } else {
    conf.cx = saved_cx;
    conf.cy = saved_cy;
    conf.rowoff = saved_rowoff;
  }
}

void replaceCallBack(char *query, int c) {
  static int direction = 1;
  static int last_mr = -1;
  static int last_match = -1;
  row *row = NULL;
  switch (c) {
  case '\r':
  case '\x1b':
    row = &conf.rows[last_match];
    char *replace = getPrompt("Replace with: %s", NULL);
    row->chars = xrealloc(row->chars, row->len + strlen(replace) + 1);
    memcpy(&row->chars[last_mr], replace, strlen(replace));
    conf.dirty++;
    updateRow(row);
    free(replace);
    direction = 1;
    last_match = 0;
    return;
    break;
  case ARROW_UP:
    direction = -1;
    break;
  case ARROW_DOWN:
    direction = 1;
    break;
  }
  if (last_match == -1)
    direction = 1;
  int current = last_match;
  int i;
  for (i = 0; i < conf.numrows; i++) {
    current += direction;
    if (current == -1)
      current = conf.numrows - 1;
    else if (current == conf.numrows)
      current = 0;
    row = &conf.rows[current];
    char *match = strstr(row->renchar, query);
    if (match) {
      last_match = current;
      conf.cy = current;
      conf.cx = rowRxToCx(row, match - row->renchar);
      conf.rowoff = conf.numrows;
      last_mr = match - row->renchar;
      break;
    }
  }
}

void replace(void) {
  int saved_cx = conf.cx;
  int saved_cy = conf.cy;
  int saved_rowoff = conf.rowoff;
  char *query = getPrompt("Search: %s", &replaceCallBack);
  if (query) {
    free(query);
  } else {
    conf.cx = saved_cx;
    conf.cy = saved_cy;
    conf.rowoff = saved_rowoff;
  }
}
