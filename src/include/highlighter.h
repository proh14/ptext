#ifndef _HIGHLIGHT_H_
#define _HIGHLIGHT_H_

#include <buff.h>
#include <lexer.h>
#include <stddef.h>

#define SYMBOL_COLOR "\x1b[3;32m"
#define SYMBOL_LEN 7
#define PREPROC_COLOR "\x1b[3;97m"
#define PREPROC_LEN 7
#define KEYWORD_COLOR "\x1b[3;31m"
#define KEYWORD_LEN 7
#define INVALID_COLOR "\x1b[3;97m"
#define INVALID_LEN 7

void highlight(Lexer *l, struct buff *buff);
#endif // _HIGHLIGHT_H_
