#pragma once
#ifndef _INPUT_H_
#define _INPUT_H_

enum editorKeys {
  BACKSPACE = 127,
  ARROW_UP = 500,
  ARROW_DOWN,
  ARROW_LEFT,
  ARROW_RIGHT,
  DEL_KEY,
};

void delChar(void);
void insertAChar(int c);
void insertNewLine(void);
int readKey(void);
void procKey(void);

#endif
