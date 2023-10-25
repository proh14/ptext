#include <buff.h>
#include <highlighter.h>
#include <lexer.h>

void highlight(Lexer *l, struct buff *buff) {
  Token t;
  while (1) {
    t = getNextToken(l);
    switch (t.kind) {
    case TOKEN_SYMBOL:
      buffAppend(buff, SYMBOL_COLOR, SYMBOL_LEN);
      buffAppend(buff, t.text, t.textlen);
      break;
    case TOKEN_KEYWORD:
      buffAppend(buff, KEYWORD_COLOR, KEYWORD_LEN);
      buffAppend(buff, t.text, t.textlen);
      break;
    case TOKEN_INVALID:
      buffAppend(buff, INVALID_COLOR, INVALID_LEN);
      buffAppend(buff, t.text, t.textlen);
      break;
    case TOKEN_PREPROC:
      buffAppend(buff, PREPROC_COLOR, PREPROC_LEN);
      buffAppend(buff, t.text, t.textlen);
      break;
    case TOKEN_END:
      return;
      break;
    default:
      buffAppend(buff , INVALID_COLOR, INVALID_LEN);
      buffAppend(buff, t.text, t.textlen);
      break;
    }
  }
}
