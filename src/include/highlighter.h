#ifndef _HIGHLIGHT_H_
#define _HIGHLIGHT_H_

#include <buff.h>
#include <lexer.h>
#include <stddef.h>

#define SYMBOL_COLOR "\x1b[0m"
#define SYMBOL_LEN 4
#define PREPROC_COLOR "\x1b[97m"
#define PREPROC_LEN 5
#define KEYWORD_COLOR "\x1b[33m"
#define KEYWORD_LEN 5
#define DIGIT_COLOR "\x1b[31m"
#define DIGIT_LEN 5
#define COMMENT_COLOR "\x1b[36m"
#define COMMENT_LEN 5
#define INVALID_COLOR "\x1b[0m"
#define INVALID_LEN 4

void highlight(Lexer *l, struct buff *buff);
#endif // _HIGHLIGHT_H_
