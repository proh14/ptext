#include <buff.h>
#include <highlighter.h>
#include <lexer.h>
#include <stdlib.h>

void highlight(Token *t, struct buff *buff) {
  int i = 0;
  while (1) {
    i++;
    switch (t[i - 1].kind) {
    case TOKEN_SYMBOL:
      buffAppend(buff, SYMBOL_COLOR, SYMBOL_LEN);
      buffAppend(buff, t[i - 1].text, t[i - 1].textlen);
      break;
    case TOKEN_KEYWORD:
      buffAppend(buff, KEYWORD_COLOR, KEYWORD_LEN);
      buffAppend(buff, t[i - 1].text, t[i - 1].textlen);
      break;
    case TOKEN_INVALID:
      buffAppend(buff, INVALID_COLOR, INVALID_LEN);
      buffAppend(buff, t[i - 1].text, t[i - 1].textlen);
      break;
    case TOKEN_PREPROC:
      buffAppend(buff, PREPROC_COLOR, PREPROC_LEN);
      buffAppend(buff, t[i - 1].text, t[i - 1].textlen);
      break;
    case TOKEN_DIGIT:
      buffAppend(buff, DIGIT_COLOR, DIGIT_LEN);
      buffAppend(buff, t[i - 1].text, t[i - 1].textlen);
      break;
    case TOKEN_COMMENT:
      buffAppend(buff, COMMENT_COLOR, COMMENT_LEN);
      buffAppend(buff, t[i - 1].text, t[i - 1].textlen);
      break;
    case TOKEN_END:
      return;
      break;
    default:
      buffAppend(buff, INVALID_COLOR, INVALID_LEN);
      buffAppend(buff, t[i - 1].text, t[i - 1].textlen);
      break;
    }
  }
}

Token *prehighlight(Token *tokens, Lexer *l) {
  int i = 0;
  while (1) {
    i++;
    tokens = realloc(tokens, sizeof(Token) * i);
    tokens[i - 1] = getNextToken(l);
    if (tokens[i - 1].kind == TOKEN_END)
      break;
  }
  return tokens;
}
