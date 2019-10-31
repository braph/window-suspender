#include "lexer.h"

#include "ctype.h"
#include "stdarg.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"

static const char* const token_words[TOK_WORD_END] = {
  [TOK_WORD_TITLE]           "title",
  [TOK_WORD_CLASSNAME]       "classname",
  [TOK_WORD_GROUPNAME]       "groupname",
  [TOK_WORD_STATE]           "state",
  [TOK_WORD_TYPE]            "type",
  [TOK_WORD_STACKPOSITION]   "stackposition",
  [TOK_WORD_IGNORE]          "ignore",
  [TOK_WORD_SUSPEND]         "suspend",
  [TOK_WORD_PRINT]           "print"
};

TokenWord word_to_tokenword(const char *str) {
  for (int i = 0; i < TOK_WORD_END; ++i)
    if (streq(str, token_words[i]))
      return i;
  return TOK_WORD_END;
}

static const char* _token_to_string(TokenType token) {
  switch (token) {
    case TOK_NUMBER:      return "<NUMBER>";
    case TOK_WORD:        return "<WORD>";
    case TOK_DQ_STRING:   return "<DOUBLE QUOTED STRING>";
    case TOK_SQ_STRING:   return "<SINGLE QUOTED STRING>";
    case TOK_COND_BEG:    return "(";
    case TOK_COND_END:    return ")";
    case TOK_BLOCK_BEG:   return "{";
    case TOK_BLOCK_END:   return "}";
    case TOK_SEMICOLON:   return ";";
    case TOK_EQUAL:       return "==";
    case TOK_NOT_EQUAL:   return "!=";
    case TOK_REGEX:       return "=~";
    case TOK_NOT_REGEX:   return "!~";
    case TOK_GREATER:     return ">";
    case TOK_GREATER_EQ:  return ">=";
    case TOK_NOT:         return "!";
    case TOK_AND:         return "&&";
    case TOK_OR:          return "||";
    case TOK_EOF:         return "EOF";
    default:              return NULL;
  }
}

char* lex_token_to_string(TokenType token) {
  static char *str = 0;
  free(str);
  str = 0;

  const char *s = _token_to_string(token);
  if (s) return s;

  str = calloc(1, 1024);
  for (int i = 0; i <= TOK_EOF; ++i) {
    if (token & (1 << i)) {
      if (str[0])
        strcat(str, ",");
      strcat(str, _token_to_string(1 << i));
    }
  }

  return str;
}

void lex_init(lexer_t *l, FILE *fh) {
  l->fh = fh;
  l->error    = 0;
  l->c_unget  = 0;
  l->line     = 1;
  l->line_pos = 1;
}

void lex_ungetc(lexer_t *l, int c) {
  l->c_unget = c;
}

char* lex_token(lexer_t *l) {
  return l->token;
}

int lex_number(lexer_t *l) {
  return l->number;
}

int lex_getc(lexer_t *l) {
  if (l->c_unget > 0) {
    l->c = l->c_unget;
    l->c_unget = 0;
  }
  else {
    l->c = fgetc(l->fh);

    if (l->c <= 0)
      return (l->c = EOF);
    else if (l->c == '\n') {
      l->line++;
      l->line_pos = 1;
    } else
      l->line_pos++;
  }

  return l->c;
}

static bool issyntax(int c) {
  return strchr("(){}\"'&|=!;~<>", c);
}

static void inline clear_token(lexer_t *l) {
  l->t = l->token;
  *(l->t) = 0;
}

static void inline token_append(lexer_t *l, int c) {
  *(l->t) = c;
  l->t++;
}

int lex_throw_error(lexer_t *l, const char *fmt, ...) {
  char errmsg[4096];
  va_list va;
  va_start(va, fmt);
  int s = vsprintf(errmsg, fmt, va);
  va_end(va);

  l->error = realloc(l->error, s + 30);
  sprintf(l->error, "on line %d:%d: %s", l->line, l->line_pos, errmsg);
  return EOF;
}

static inline int token_finalize(lexer_t *l, int ret) {
  l->t = 0;
  return ret;
}

static TokenType read_single_quote(lexer_t *l) {
  int c;
  clear_token(l);

  while ((c = lex_getc(l)) != EOF) {
    if (c == '\'')
      return token_finalize(l, TOK_WORD); // Treat as word for now
    else if (c == '\\') {
      if ((c = lex_getc(l)) != '\'')
        token_append(l, c);
    }
    else
      token_append(l, c);
  }
  return lex_throw_error(l, "unterminated single quote");
}

static TokenType read_double_quote(lexer_t *l) {
  int c;
  clear_token(l);
  while ((c = lex_getc(l)) > 0) {
    if (c == '"')
      return token_finalize(l, TOK_WORD); // Treat as word for now
    else if (c == '\\') {
      if ((c = lex_getc(l)) != '"')
        token_append(l, c);
    }
    else
      token_append(l, c);
  }
  return lex_throw_error(l, "unterminated double quote");
}

static TokenType read_word(lexer_t *l, char *prefix, int len) {
  int c; // TODO: prefix/len repalce wich single char
  clear_token(l);
  if (len)
    l->t += snprintf(l->token, len, "%s", prefix);

  while ((c = lex_getc(l)) > 0) {
    if (isspace(c) || issyntax(c)) {
      lex_ungetc(l, c);
      return token_finalize(l, TOK_WORD);
    }
    else
      token_append(l, c);
  }
}

static TokenType read_eq(lexer_t *l) {
  switch (lex_getc(l)) {
    case '=':   return TOK_EQUAL;
    case '~':   return TOK_REGEX;
    default:    return lex_throw_error(l, "expected == or =~");
  }
}

static TokenType read_not(lexer_t *l) {
  int c = lex_getc(l);
  if (c == '=')
    return TOK_NOT_EQUAL;
  else {
    lex_ungetc(l, c);
    return TOK_NOT;
  }
}

static TokenType read_greater(lexer_t *l) {
  int c = lex_getc(l);
  if (c == '=')
    return TOK_GREATER_EQ;
  else {
    lex_ungetc(l, c);
    return TOK_GREATER;
  }
}

static TokenType read_number(lexer_t *l, int start) {
  int c;
  l->number = start - '0';
  while (isdigit((c = lex_getc(l))))
    l->number = l->number * 10 + c - '0';
  lex_ungetc(l, c);
  return TOK_NUMBER;
}

static void read_comment(lexer_t *l) {
  int c;
  while ((c = lex_getc(l)) != '\n' && c != EOF);
}

TokenType lex(lexer_t *l) {
  #define GETC()   lex_getc(l)
  #define PERR(M)  lex_throw_error(l, M);

  int c;
  switch ((c = lex_getc(l))) {
    case '\'': return read_single_quote(l);
    case '"':  return read_double_quote(l);
    case '(':  return TOK_COND_BEG;
    case ')':  return TOK_COND_END;
    case '{':  return TOK_BLOCK_BEG;
    case '}':  return TOK_BLOCK_END;
    case ';':  return TOK_SEMICOLON;
    case '=':  return read_eq(l);
    case '!':  return read_not(l);
    case '>':  return read_greater(l);
    case '|':  return (GETC() == '|' ? TOK_OR  : PERR("expected ||"));
    case '&':  return (GETC() == '&' ? TOK_AND : PERR("expected &&"));
    case '0':  case '2': case '4': case '6': case '8':
    case '1':  case '3': case '5': case '7': case '9': return read_number(l, c);
    case '#':  read_comment(l); /* fall through */
    case ' ':
    case '\n':
    case '\r':
    case '\t':
    case '\f':
    case '\v': return lex(l);
    case EOF:  return EOF;
    default:   return read_word(l, (char*) &c, 1);
  }
}
