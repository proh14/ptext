#include <buff.h>
#include <highlighter.h>
#include <lexer.h>

void highlight(char *hl, char *content, struct buff *buff, int len) {
  int i = 0;
  if (hl == NULL) {
    return;
  }
  while (1) {
    switch (hl[i]) {
    case TOKEN_SYMBOL:
      buffAppend(buff, SYMBOL_COLOR, SYMBOL_LEN);
      break;
    case TOKEN_KEYWORD:
      buffAppend(buff, KEYWORD_COLOR, KEYWORD_LEN);
      break;
    case TOKEN_PREPROC:
      buffAppend(buff, PREPROC_COLOR, PREPROC_LEN);
      break;
    case TOKEN_DIGIT:
      buffAppend(buff, DIGIT_COLOR, DIGIT_LEN);
      break;
    case TOKEN_COMMENT:
      buffAppend(buff, COMMENT_COLOR, COMMENT_LEN);
      break;
    case TOKEN_MATCH:
      buffAppend(buff, MATCH_COLOR, MATCH_LEN);
      break;
    case TOKEN_END:
      return;
      break;
    default:
      buffAppend(buff, INVALID_COLOR, INVALID_LEN);
      break;
    }
    if (i >= len)
      return;
    buffAppend(buff, &content[i], 1);
    i++;
  }
}

void prehighlight(char *hl, Lexer *l) {
  Token t = {0};
  size_t hllen = 0;
  t.kind = TOKEN_SYMBOL;
  while (t.kind != TOKEN_END) {
    t = getNextToken(l);
    for (size_t i = 0; i < t.textlen; i++) {
      hl[i + hllen] = t.kind;
    }
    hllen += t.textlen;
  }
  hl[hllen] = TOKEN_END;
}
