#include <buff.h>
#include <utils.h>

void buffAppend(struct buff *buff, const char *s, size_t len) {
  char *new = xrealloc(buff->chars, buff->len + len);
  if (new == NULL) {
    return;
  }
  memcpy(&new[buff->len], s, len);
  buff->chars = new;
  buff->len += len;
}
