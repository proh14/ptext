#pragma once
#ifndef _CURSOR_H_
#define _CURSOR_H_

#include <rows.h>

int rowCxToRx(row *row, int cx);
int rowRxToCx(row *row, int cx);
void scroll(void);
void moveCursor(int key);

#endif
