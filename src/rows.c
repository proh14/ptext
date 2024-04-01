#include <highlighter.h>
#include <options.h>
#include <ptext.h>
#include <rows.h>
#include <stdafx.h>
#include <utils.h>

void updateRow(row *row) {
  int tabs = 0;
  int tabstop = getOption(O_TABSTOP);
  size_t j;
  for (j = 0; j < row->len; j++)
    if (row->chars[j] == '\t')
      tabs++;
  free(row->renchar);
  row->renchar = xmalloc(row->len + tabs * (tabstop - 1) + 1);
  int idx = 0;
  for (j = 0; j < row->len; j++) {
    if (row->chars[j] == '\t') {
      row->renchar[idx++] = ' ';
      while (idx % tabstop != 0)
        row->renchar[idx++] = ' ';
    } else {
      row->renchar[idx++] = row->chars[j];
    }
  }
  row->renchar[idx] = '\0';
  row->renlen = idx;
  Lexer l = {.content = row->renchar,
             .contentlen = row->renlen,
             .cursor = 0,
             .idx = row->idx};
  row->hl = xrealloc(row->hl, row->renlen + 1);
  prehighlight(row->hl, &l);
}

void rowAppend(char *s, size_t len, int at) {
  if (at < 0 || at > curbuf.numrows)
    return;
  curbuf.rows = xrealloc(curbuf.rows, sizeof(row) * (curbuf.numrows + 1));
  memmove(&curbuf.rows[at + 1], &curbuf.rows[at],
          sizeof(row) * (curbuf.numrows - at));

  for (int j = at + 1; j <= curbuf.numrows; j++)
    curbuf.rows[j].idx++;
  curbuf.rows[at].chars = xmalloc(len + 1);
  memcpy(curbuf.rows[at].chars, s, len);
  curbuf.rows[at].chars[len] = '\0';
  curbuf.rows[at].idx = at;
  curbuf.rows[at].len = len;
  curbuf.rows[at].renlen = 0;
  curbuf.rows[at].renchar = NULL;
  curbuf.rows[at].hl = NULL;
  curbuf.rows[at].in_comment = 0;
  curbuf.numrows++;
  curbuf.dirty++;
  updateRow(&curbuf.rows[at]);
}

void rowAppendString(row *row, char *s, size_t len) {
  row->chars = xrealloc(row->chars, row->len + len + 1);
  memcpy(&row->chars[row->len], s, len);
  row->len += len;
  row->chars[row->len] = '\0';
  updateRow(row);
  curbuf.dirty++;
}

void freeRow(row *row) {
  free(row->renchar);
  free(row->chars);
  free(row->hl);
}

void delRow(int at) {
  if (at < 0 || at >= curbuf.numrows) {
    return;
  }
  freeRow(&curbuf.rows[at]);
  memmove(&curbuf.rows[at], &curbuf.rows[at + 1],
          sizeof(row) * (curbuf.numrows - at - 1));
  for (int i = at; i < curbuf.numrows - 1; i++) {
    curbuf.rows[i].idx--;
  }
  curbuf.numrows--;
  curbuf.dirty++;
}

void rowDelChar(row *row, int at) {
  if (at < 0 || at >= (int)row->len)
    return;
  memmove(&row->chars[at], &row->chars[at + 1], row->len - at);
  row->len--;
  updateRow(row);
  curbuf.dirty++;
}

void rowInsertChar(row *row, int at, int c) {
  if (at < 0 || at > (int)row->len)
    at = row->len;
  row->chars = xrealloc(row->chars, row->len + 2);
  memmove(&row->chars[at + 1], &row->chars[at], row->len - at + 1);
  row->len++;
  row->chars[at] = c;
  updateRow(row);
}
