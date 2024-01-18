#pragma once
#ifndef _BUFF_H_
#define _BUFF_H_

#include <stdafx.h>

struct buff {
  char *chars;
  size_t len;
};
void buffAppend(struct buff *buff, const char *s, size_t len);

#endif // _BUFF_H_
