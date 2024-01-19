#pragma once
#ifndef _INPUT_H_
#define _INPUT_H_
#include <stdafx.h>

enum editorKeys {
#ifdef _WIN32

#define KEY_Q 0x51
#define KEY_S 0x53
#define KEY_H 0x48
#define KEY_D 0x44
#define KEY_A 0x41
#define KEY_I 0x49
#define KEY_U 0x55
#define KEY_E 0x45
#define KEY_F 0x46
#define KEY_R 0x52

  BACKSPACE = VK_BACK,
  ARROW_UP = VK_UP,
  ARROW_DOWN = VK_DOWN,
  ARROW_LEFT = VK_RIGHT,
  ARROW_RIGHT = VK_LEFT,  // swap left and right ?
  DEL_KEY = VK_DELETE,
  ESC_KEY = VK_ESCAPE,
#else
#define CTRL_KEY(k) ((k) & 0x1f)

  BACKSPACE = 127,
  ARROW_UP = 500,
  ARROW_DOWN,
  ARROW_LEFT,
  ARROW_RIGHT,
  DEL_KEY,
  ESC_KEY = 27,
#endif  // _WIN32
};

typedef struct keyPair {
#ifdef _WIN32
  WORD key;
#else
  int key;
#endif  // _WIN32
  void (*func)(void);
} keysPair;

void delChar(void);
void insertAChar(int c);
void insertNewLine(void);

#ifdef _WIN32
KEY_EVENT_RECORD readKey(void);
#else
int readKey(void);
#endif  // _WIN32

void procKey(void);

// return 1 if key is handled, 0 otherwise
int handleCustomKeys(int c);

#endif
