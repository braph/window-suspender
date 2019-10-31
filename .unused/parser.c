#include "lexer.h"
#include "parser.h"

void parser_init(parser_t *p, FILE *fh) {
  p->lex        = lex_init(fh);
  p->type       = 0;
  p->_last_type = 0;
}

void parser_destroy(parser_t *p) {
  lex_destroy(p->lex);
}

const char* parser_token(parser_t *p) {
  return lex_token(p->lex);
}

int parser_number(parser_t *p) {
  return lex_number(p->lex);
}

TokenType peek(parser_t *p) {
  if (p->type == 0)
    p->type = lex(p->lex);

  p->_last_type = p->type;
  return p->type;
}

TokenType next(parser_t *p) {
  TokenType tmp;

  if (p->type != 0) {
    tmp = p->type;
    p->type = 0;
  }
  else {
    tmp = lex(p->lex);
  }

  p->_last_type = tmp;
  return tmp;
}

TokenType next_in(parser_t *p, TokenType wanted) {
  TokenType tmp = next(p);
  if (! (wanted & tmp))
    return parser_throw_error(p, "expected: %s", token_to_string(wanted));
  return tmp;
}

IConditional* read_conditional(parser_t *p) {
  TokenType   ttmp;
  TokenWord   tword;
  const char  *str;

  switch (peek(p)) {
    case TOK_COND_BEG:
      return read_conditionals(p);

    case TOK_NOT:
      next(p);
      return not_conditional_new(read_conditional(p)); // new handles error

    case TOK_WORD:
      next(p);
      switch ((tword = word_to_tokenword(parser_token(p)))) {
        case TOK_WORD_TITLE:
        case TOK_WORD_CLASSNAME:
        case TOK_WORD_GROUPNAME:
          if (! (ttmp = next_in(p, TOK_EQUAL|TOK_NOT_EQUAL|TOK_REGEX|TOK_NOT_REGEX)))
            return 0;
          if (! next_in(p, TOK_WORD))
            return 0;
          return string_conditional_new(tword, ttmp, lex_token(p)); // TODO regex error

        case TOK_WORD_STATE:
          if (! (ttmp = next_in(p, TOK_EQUAL|TOK_NOT_EQUAL)))
            return 0;
          if (! next_in(p, TOK_WORD))
            return 0;
          return state_conditional_new((ttmp == TOK_EQUAL), lex_token(p));

        case TOK_WORD_TYPE:
          if (! (ttmp = next_in(p, TOK_EQUAL|TOK_NOT_EQUAL)))
            return 0;
          if (! next_in(p, TOK_WORD))
            return 0;
          return type_conditional_new((ttmp == TOK_EQUAL), lex_token(p));

        case TOK_WORD_STACKPOSITION:
          if (! (ttmp = next(p, TOK_GREATER|TOK_GREATER_EQ)))
            return 0;
          if (! next_in(p, TOK_NUMBER))
            return 0;

          return state_conditional(ttmp, lex_number(p));

        default:
          return throw_error(p, "expected title, classname, groupname, state, type, stackposition");
      }

    default:
      return throw_error(p, "expected !, (, title, classname, groupname, state, type stackposition");
  }
}

IConditional read_conditionals(parser_t *p) {
  next(p, TOK_COND_BEG);

  IConditional *cond_next;
  IConditional *cond = read_conditional(p);
  if (! cond)
    return 0;

  while (true) {
    switch (peek(l)) {
      case TOK_AND:
        NEXT_OR_ERROR(p, TOK_AND);
        if (! (cond_next = read_conditional(p)))
          goto ERROR;
        cond = and_conditional_new(cond, cond_next);
        break;

      case TOK_OR:
        NEXT_OR_ERROR(p, TOK_AND);
        if (! (cond_next = read_conditional(p)))
          goto ERROR;
        cond = or_conditional_new(cond, cond_next);
        break;

      case TOK_COND_END:
        NEXT_OR_ERROR(p, TOK_COND_END);
        return cond;

      case EOF:
        goto ERROR;
    }
  }
  //assert_not_reached();
ERROR:
  cond->free(cond);
}

IStatement* read_statement(parser_t *p) {
  IConditional *cond;
  IStatement   *stmts;

  switch (peek(p)) {
    case TOK_BLOCK_BEG:
      return read_statements(p);

    case TOK_WORD:
      next(p);
      switch (word_to_tokenword(token(p))) {
        case TOK_WORD_IGNORE:   return ignore_stmt_new(p);
        case TOK_WORD_SUSPEND:  return suspend_stmt_new(p);
        case TOK_WORD_PRINT:
          if (! next_in(p, TOK_WORD))
            return 0; // TODO errmsg
          return print_statement_new(token());
        default:
          return parser_throw_error(p, "expected %s|%s|%s"
            token_words[TOK_WORD_IGNORE],
            token_words[TOK_WORD_SUSPEND],
            token_words[TOK_WORD_PRINT]);
      }

    case TOK_COND_BEG:
      if (! (cond = read_conditionals(p)))
        return 0;
      if (! (stmts = read_statements(p))) {
        cond->free(cond);
        return 0;
      }

      return conditional_statement_new(cond, stmts);

    default:
      return parser_throw_error(p, "expected (, { or COMMAND");
  }
}

IStatement* read_statements(parser_t *p) {
  IStatement     *stmt;
  statement_list *stmts = statement_list_new();

  next(p, TOK_BLOCK_BEG);
  while (peek(p) != TOK_BLOCK_END) {
    if ((stmt = readStatement(p)))
      statement_list_add(stmts, stmt);
    else {
      statement_list_free(stmts);
      return NULL;
    }
  }
  next(p, TOK_BLOCK_END);

  return stmts;
}

IStatement* parser_parse(parser_t *p) {
  IStatement     *stmt;
  statement_list *stmts = statement_list_new();

  while (peek(p) != TOK_EOF) {
    if ((stmt = readStatement(p)))
      statement_list_add(stmts, stmt);
    else {
      statement_list_free(stmts);
      return NULL;
    }
  }

  // TODO:
  window_state_to_str(WNCK_WINDOW_STATE_ABOVE);
  lex_token_to_string(TOK_EOF); // clear the buffer

  return stmts;
}

parser_throw_error(parser_t *p, const char *fmt, ...) {
  return lex.error(m);
}
