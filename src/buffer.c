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
