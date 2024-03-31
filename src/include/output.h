#pragma once
#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include <screen.h>

void drawAll(struct screenBuffer *buff);
void drawStatusBar(struct screenBuffer *buff);
void drawStatusMessage(struct screenBuffer *buff);
void refresh(void);

#endif
