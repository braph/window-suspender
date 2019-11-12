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
    GRAVITY = 264,
    ACTION_TYPE = 265,
    EQUAL = 266,
    UNEQUAL = 267,
    LESS = 268,
    LESS_EQUAL = 269,
    GREATER = 270,
    GREATER_EQUAL = 271,
    SET_GEOMETRY = 272,
    REGEX_EQUAL = 273,
    REGEX_UNEQUAL = 274,
    CONTAINS = 275,
    PRINT = 276,
    SYSTEM = 277,
    SUSPEND = 278,
    RETURN = 279,
    PROCESS = 280,
    HAS = 281,
    CHILDREN = 282,
    OPTION_DELAY = 283,
    OPTION_REFRESH_AFTER = 284,
    OPTION_REFRESH_DURATION = 285,
    OPTION_X = 286,
    OPTION_Y = 287,
    OPTION_WIDTH = 288,
    OPTION_HEIGHT = 289,
    COND_TYPE = 290,
    COND_STACKPOSITION = 291,
    COND_STATE = 292,
    COND_HOOK = 293,
    COND_WORKSPACE_NUMBER = 294,
    ANY = 295,
    NOT = 296,
    AND_AND = 297,
    OR_OR = 298
  };
#endif
/* Tokens.  */
#define STRING 258
#define NUMBER 259
#define WINDOW_STATE 260
#define WINDOW_TYPE 261
#define WINDOW_FIELD 262
#define HOOK_TYPE 263
#define GRAVITY 264
#define ACTION_TYPE 265
#define EQUAL 266
#define UNEQUAL 267
#define LESS 268
#define LESS_EQUAL 269
#define GREATER 270
#define GREATER_EQUAL 271
#define SET_GEOMETRY 272
#define REGEX_EQUAL 273
#define REGEX_UNEQUAL 274
#define CONTAINS 275
#define PRINT 276
#define SYSTEM 277
#define SUSPEND 278
#define RETURN 279
#define PROCESS 280
#define HAS 281
#define CHILDREN 282
#define OPTION_DELAY 283
#define OPTION_REFRESH_AFTER 284
#define OPTION_REFRESH_DURATION 285
#define OPTION_X 286
#define OPTION_Y 287
#define OPTION_WIDTH 288
#define OPTION_HEIGHT 289
#define COND_TYPE 290
#define COND_STACKPOSITION 291
#define COND_STATE 292
#define COND_HOOK 293
#define COND_WORKSPACE_NUMBER 294
#define ANY 295
#define NOT 296
#define AND_AND 297
#define OR_OR 298

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 40 "config.parser.y"

  int number;
  char *string;
  WnckWindowType type;
  WnckWindowState state;
  WnckWindowGravity gravity;
  window_string_id field;
  statement_type action_type;
  hook_type hook;
  comparison_type comparison;
  struct Statement *statement;
  struct Conditional *conditional;
  GSList *string_list;

#line 158 "config.parser.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_CONFIG_PARSER_TAB_H_INCLUDED  */
