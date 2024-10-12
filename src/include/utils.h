#pragma once
#ifndef _UTILS_H_
#define _UTILS_H_

void *xmalloc(size_t size);
void *xrealloc(void *ptr, size_t size);

char *rowsToString(int *buflen);
void setStatusMessage(const char *fmt, ...);
char *getPrompt(char *promt, void (*callback)(char *, int));
char *rtrim(char *s);
char *token(char *command, int *len);

#endif
