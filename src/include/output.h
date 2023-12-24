#pragma once
#ifndef _OUTPUT_H_
#define _OUTPUT_H_

#include <buff.h>

void drawAll(struct buff *buff);
void drawStatusBar(struct buff *buff);
void drawStatusMessage(struct buff *buff);
void refresh(void);

#endif
