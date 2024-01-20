#ifndef _HIGHLIGHT_H_
#define _HIGHLIGHT_H_

#include <buff.h>
#include <lexer.h>

#define SYMBOL_COLOR "\x1b[m"
#define SYMBOL_LEN 3
#define PREPROC_COLOR "\x1b[97m"
#define PREPROC_LEN 5
#define KEYWORD_COLOR "\x1b[33m"
#define KEYWORD_LEN 5
#define DIGIT_COLOR "\x1b[31m"
#define DIGIT_LEN 5
#define COMMENT_COLOR "\x1b[36m"
#define COMMENT_LEN 5
#define INVALID_COLOR "\x1b[31m"
#define INVALID_LEN 5
#define MATCH_COLOR "\x1b[34m"
#define MATCH_LEN 5
#define STRING_COLOR "\x1b[31m"
#define STRING_LEN 5

void highlight(char *hl, char *content, struct buff *buff, int len);
void prehighlight(char *hl, Lexer *l);
#endif // _HIGHLIGHT_H_
