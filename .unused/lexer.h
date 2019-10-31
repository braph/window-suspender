#ifndef _LEXER_H
#define _LEXER_H

#include "stdio.h"

typedef struct lexer_t {
  FILE   *fh;
  int     c;
  int     c_unget;
  int     line;
  int     line_pos;
  char   *error;

  int     number;      // holds number if token was a number
  char    token[4096]; // holds token string
  char   *t;
} lexer_t;

typedef enum TokenType {
  TOK_NUMBER      = 1 << 0,

  TOK_WORD        = 1 << 1,
  TOK_DQ_STRING   = 1 << 2,
  TOK_SQ_STRING   = 1 << 3,

  TOK_COND_BEG    = 1 << 4,
  TOK_COND_END    = 1 << 5,
  TOK_BLOCK_BEG   = 1 << 6,
  TOK_BLOCK_END   = 1 << 7,

  TOK_SEMICOLON   = 1 << 8,
  TOK_EQUAL       = 1 << 9,
  TOK_NOT_EQUAL   = 1 << 10,
  TOK_REGEX       = 1 << 11,
  TOK_NOT_REGEX   = 1 << 12,
  TOK_GREATER     = 1 << 13,
  TOK_GREATER_EQ  = 1 << 14,

  TOK_NOT         = 1 << 15,
  TOK_AND         = 1 << 16,
  TOK_OR          = 1 << 17,

  TOK_EOF         = 1 << 18 // EOF must be last token!
} TokenType;

typedef enum TokenWord {
  TOK_WORD_TITLE          = 0, // ARG_TITLE      (iargument.h)
  TOK_WORD_CLASSNAME      = 1, // ARG_CLASS_NAME (iargument.h)
  TOK_WORD_GROUPNAME      = 2, // ARG_GROUP_NAME (iargument.h)
  TOK_WORD_STATE          = 3,
  TOK_WORD_TYPE           = 4,
  TOK_WORD_STACKPOSITION  = 5,
  TOK_WORD_IGNORE         = 6,
  TOK_WORD_SUSPEND        = 7,
  TOK_WORD_PRINT          = 8,
  TOK_WORD_END            = 9
} TokenWord;

TokenType lex(lexer_t *);
void      lex_init(lexer_t *, FILE *);
void      lex_destroy(lexer_t *);
char*     lex_token(lexer_t *l);
int       lex_number(lexer_t *l);
char*     lex_token_to_string(TokenType);
int       lex_throw_error(lexer_t *, const char *, ...);

#endif
