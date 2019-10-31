#ifndef _PARSER_H
#define _PARSER_H

#include "lexer.h"
#include "statements.h"

typedef struct parser_t {
    lexer_t     *lex;
    TokenType   type;
    TokenType   _last_type;
} parser_t;

void        parser_init(parser_t *, FILE *);
IStatement* parser_parse(parser_t *);
void        parser_destroy(parser_t *p);

#endif
