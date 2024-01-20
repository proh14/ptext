#pragma once
#ifndef _LEXER_H_
#define _LEXER_H_
#include <stddef.h>
typedef enum {
  TOKEN_END = 0,
  TOKEN_INVALID,
  TOKEN_SYMBOL,
  TOKEN_KEYWORD,
  TOKEN_PREPROC,
  TOKEN_COMMENT,
  TOKEN_OPEN_PAREN,
  TOKEN_CLOSE_PAREN,
  TOKEN_MATCH,
  TOKEN_OPEN_CURLY,
  TOKEN_CLOSE_CURLY,
  TOKEN_SEMICOLON,
  TOKEN_STRING,
  TOKEN_DIGIT,
} Token_kind;

typedef struct {
  Token_kind kind;
  size_t textlen;
  char *text;
} Token;

typedef struct {
  char *content;
  size_t cursor;
  size_t contentlen;
  int idx;
} Lexer;

typedef struct {
  char *text;
  Token_kind kind;
} Single_Tokens;

Token getNextToken(Lexer *);

#endif // _LEXER_H_
