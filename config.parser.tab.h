/* A Bison parser, made by GNU Bison 3.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_CONFIG_PARSER_TAB_H_INCLUDED
# define YY_YY_CONFIG_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    STRING = 258,
    NUMBER = 259,
    WINDOW_STATE = 260,
    WINDOW_TYPE = 261,
    WINDOW_FIELD = 262,
    HOOK_TYPE = 263,
    ACTION_TYPE = 264,
    EQUAL = 265,
    UNEQUAL = 266,
    LESS = 267,
    LESS_EQUAL = 268,
    GREATER = 269,
    GREATER_EQUAL = 270,
    REGEX_EQUAL = 271,
    REGEX_UNEQUAL = 272,
    CONTAINS = 273,
    PRINT = 274,
    SYSTEM = 275,
    SUSPEND = 276,
    RETURN = 277,
    PROCESS = 278,
    HAS = 279,
    CHILDREN = 280,
    OPTION_SUSPEND_DELAY = 281,
    OPTION_REFRESH_DELAY = 282,
    OPTION_REFRESH_DURATION = 283,
    COND_TYPE = 284,
    COND_STACKPOSITION = 285,
    COND_STATE = 286,
    COND_HOOK = 287,
    COND_WORKSPACE_NUMBER = 288,
    NOT = 289,
    AND_AND = 290,
    OR_OR = 291
  };
#endif
/* Tokens.  */
#define STRING 258
#define NUMBER 259
#define WINDOW_STATE 260
#define WINDOW_TYPE 261
#define WINDOW_FIELD 262
#define HOOK_TYPE 263
#define ACTION_TYPE 264
#define EQUAL 265
#define UNEQUAL 266
#define LESS 267
#define LESS_EQUAL 268
#define GREATER 269
#define GREATER_EQUAL 270
#define REGEX_EQUAL 271
#define REGEX_UNEQUAL 272
#define CONTAINS 273
#define PRINT 274
#define SYSTEM 275
#define SUSPEND 276
#define RETURN 277
#define PROCESS 278
#define HAS 279
#define CHILDREN 280
#define OPTION_SUSPEND_DELAY 281
#define OPTION_REFRESH_DELAY 282
#define OPTION_REFRESH_DURATION 283
#define COND_TYPE 284
#define COND_STACKPOSITION 285
#define COND_STATE 286
#define COND_HOOK 287
#define COND_WORKSPACE_NUMBER 288
#define NOT 289
#define AND_AND 290
#define OR_OR 291

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 40 "config.parser.y"

  int number;
  char *string;
  WnckWindowType type;
  WnckWindowState state;
  window_string_id field;
  statement_type action_type;
  hook_type hook;
  comparison_type comparison;
  struct Statement *statement;
  struct Conditional *conditional;
  GSList *string_list;

#line 143 "config.parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_CONFIG_PARSER_TAB_H_INCLUDED  */
