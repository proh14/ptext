#pragma once
#ifndef _COMMANDS_H_
#define _COMMANDS_H_

typedef struct {
  char *name;
  void (*func)(void);
} commandBind;

extern commandBind commands[];

void execCommand(void);
void doLine(char *line);
void doFile(char *filename);

#endif
