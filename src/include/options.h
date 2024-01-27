#pragma once
#ifndef _OPTIONS_H_
#define _OPTIONS_H_

typedef enum optionIndex { O_TABSTOP = 0, O_QUITCONF } optionIndex;

typedef struct {
  const char *name;
  int value;
} option;

int setOption_char(char *name, int value);
int getOption_char(char *name);
int getOption(optionIndex idx);
int setOption(optionIndex idx, int value);

#endif
