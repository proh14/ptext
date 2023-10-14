#include <ctype.h>
#include <lexer.h>
#include <stdio.h>
#include <string.h>

const char *keywords[] = {
    "auto",          "break",
    "case",          "char",
    "const",         "continue",
    "default",       "do",
    "double",        "else",
    "enum",          "extern",
    "float",         "for",
    "goto",          "if",
    "int",           "long",
    "register",      "return",
    "short",         "signed",
    "sizeof",        "static",
    "struct",        "switch",
    "typedef",       "union",
    "unsigned",      "void",
    "volatile",      "while",
    "alignas",       "alignof",
    "and",           "and_eq",
    "asm",           "atomic_cancel",
    "atomic_commit", "atomic_noexcept",
    "bitand",        "bitor",
    "bool",          "catch",
    "char16_t",      "char32_t",
    "char8_t",       "class",
    "co_await",      "co_return",
    "co_yield",      "compl",
    "concept",       "const_cast",
    "consteval",     "constexpr",
    "constinit",     "decltype",
    "delete",        "dynamic_cast",
    "explicit",      "export",
    "false",         "friend",
    "inline",        "mutable",
    "namespace",     "new",
    "noexcept",      "not",
    "not_eq",        "nullptr",
    "operator",      "or",
    "or_eq",         "private",
    "protected",     "public",
    "reflexpr",      "reinterpret_cast",
    "requires",      "static_assert",
    "static_cast",   "synchronized",
    "template",      "this",
    "thread_local",  "throw",
    "true",          "try",
    "typeid",        "typename",
    "using",         "virtual",
    "wchar_t",       "xor",
    "xor_eq",
};

#define KEYWORDS_COUNT (sizeof(keywords) / sizeof(keywords[0]))

int isSymbol(char x) { return isalnum(x) || x == '_'; }
int isSymbolStart(char x) { return isalpha(x) || x == '_'; }

void trimLeft(Lexer *l) {
  while (isspace(l->content[l->cursor])) {
    l->cursor++;
  }
}

Token getNextToken(Lexer *l) {
  Token t = {0};

  t.kind = TOKEN_END;
  trimLeft(l);
  t.text = &l->content[l->cursor];

  if (l->content[l->cursor] == '#') {
    t.kind = TOKEN_PREPROC;
    while (l->content[l->cursor] != '\n' && l->content[l->cursor] != '\\') {
      l->cursor++;
      t.textlen++;
    }
    return t;
  }
  if (isSymbolStart(l->content[l->cursor])) {
    t.kind = TOKEN_SYMBOL;
    t.textlen++;
    l->cursor++;
    while (isSymbol(l->content[l->cursor]) && l->cursor <= l->contentlen) {
      t.textlen++;
      l->content++;
    }
    for (size_t i = 0; i < KEYWORDS_COUNT; ++i) {
      size_t keyword_len = strlen(keywords[i]);
      if (keyword_len == t.textlen &&
          memcmp(keywords[i], t.text, keyword_len) == 0) {
        t.kind = TOKEN_KEYWORD;
        break;
      }
    }
  }
  return t;
}
