#ifndef _BUFF_H_
#define _BUFF_H_

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
struct buff {
  char *chars;
  size_t len;
};
void buffAppend(struct buff *buff, const char *s, size_t len);

#endif // _BUFF_H_
