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
  static char *last_hl = NULL;

  if (last_hl) {
    row *row = &curbuf.rows[last_match];
    memcpy(row->hl, last_hl, row->renlen);
    free(last_hl);
    last_hl = NULL;
  }

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
  for (i = 0; i < curbuf.numrows; i++) {
    current += direction;
    if (current == -1)
      current = curbuf.numrows - 1;
    else if (current == curbuf.numrows)
      current = 0;
    row *row = &curbuf.rows[current];
    char *match = strstr(row->renchar, query);
    if (match) {
      last_match = current;
      curbuf.cy = current;
      curbuf.cx = rowRxToCx(row, match - row->renchar);
      curbuf.rowoff = curbuf.numrows;
      last_hl = malloc(row->renlen);
      memcpy(last_hl, row->hl, row->renlen);
      memset(&row->hl[match - row->renchar], TOKEN_MATCH, strlen(query));
      break;
    }
  }
}

void search(void) {
  int saved_cx = curbuf.cx;
  int saved_cy = curbuf.cy;
  int saved_rowoff = curbuf.rowoff;
  char *query = getPrompt("Search: %s", &searchCallBack);
  if (query) {
    free(query);
  } else {
    curbuf.cx = saved_cx;
    curbuf.cy = saved_cy;
    curbuf.rowoff = saved_rowoff;
  }
}

void replaceCallBack(char *query, int c) {
  static int direction = 1;
  static int last_mr = -1;
  static int last_match = -1;

  static char *last_hl = NULL;

  if (last_hl) {
    row *row = &curbuf.rows[last_match];
    memcpy(row->hl, last_hl, row->renlen);
    free(last_hl);
    last_hl = NULL;
  }

  row *row = NULL;
  switch (c) {
  case '\r':
  case '\x1b':
    row = &curbuf.rows[last_match];
    char *replace = getPrompt("Replace with: %s", NULL);
    if (replace == NULL) {
      free(replace);
      return;
    }
    row->chars = xrealloc(row->chars, row->len + strlen(replace) + 1);
    memcpy(&row->chars[last_mr], replace, strlen(replace));
    curbuf.dirty++;
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
  for (i = 0; i < curbuf.numrows; i++) {
    current += direction;
    if (current == -1)
      current = curbuf.numrows - 1;
    else if (current == curbuf.numrows)
      current = 0;
    row = &curbuf.rows[current];
    char *match = strstr(row->renchar, query);
    if (match) {
      last_match = current;
      curbuf.cy = current;
      curbuf.cx = rowRxToCx(row, match - row->renchar);
      curbuf.rowoff = curbuf.numrows;
      last_hl = malloc(row->renlen);
      memcpy(last_hl, row->hl, row->renlen);
      memset(&row->hl[match - row->renchar], TOKEN_MATCH, strlen(query));
      last_mr = match - row->renchar;
      break;
    }
  }
}

void replace(void) {
  int saved_cx = curbuf.cx;
  int saved_cy = curbuf.cy;
  int saved_rowoff = curbuf.rowoff;
  char *query = getPrompt("Search: %s", &replaceCallBack);
  if (query) {
    free(query);
  } else {
    curbuf.cx = saved_cx;
    curbuf.cy = saved_cy;
    curbuf.rowoff = saved_rowoff;
  }
}
