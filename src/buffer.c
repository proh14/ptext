#include <buffer.h>
#include <ptext.h>

buffer createBuffer(buffer *b, int editable) {
  b->cx = 0;
  b->cy = 0;
  b->rowoff = 0;
  b->coloff = 0;
  b->dirty = 0;
  b->numrows = 0;
  b->rows = NULL;
  b->editable = editable;
  b->filename = NULL;
  b->rx = 0;
  return *b;
}

void freeBuffer(buffer *b) {
  if (b->rows == NULL && curbuf.numrows == 0) {
    return;
  }
  for (int i = 0; i < b->numrows; i++) {
    free(b->rows[i].chars);
    free(b->rows[i].renchar);
    free(b->rows[i].hl);
  }
  free(b->rows);
  b->rows = NULL;

  free(b->filename);
}
