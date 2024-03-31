#pragma once
#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <stdafx.h>

struct screenBuffer {
  char *chars;
  size_t len;
};

void screenAppend(struct screenBuffer *buff, const char *s, size_t len);

#endif // _SCREEN_H_
