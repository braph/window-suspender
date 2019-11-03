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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
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
    EQUAL = 263,
    UNEQUAL = 264,
    LESS = 265,
    LESS_EQUAL = 266,
    GREATER = 267,
    GREATER_EQUAL = 268,
    REGEX_EQUAL = 269,
    REGEX_UNEQUAL = 270,
    CONTAINS = 271,
    PRINT = 272,
    SYSTEM = 273,
    SUSPEND = 274,
    RETURN = 275,
    PROCESS = 276,
    HAS = 277,
    CHILDREN = 278,
    OPTION_SUSPEND_DELAY = 279,
    OPTION_REFRESH_DELAY = 280,
    OPTION_REFRESH_DURATION = 281,
    COND_TYPE = 282,
    COND_STACKPOSITION = 283,
    COND_STATE = 284,
    COND_WORKSPACE_NUMBER = 285,
    NOT = 286,
    AND_AND = 287,
    OR_OR = 288
  };
#endif
/* Tokens.  */
#define STRING 258
#define NUMBER 259
#define WINDOW_STATE 260
#define WINDOW_TYPE 261
#define WINDOW_FIELD 262
#define EQUAL 263
#define UNEQUAL 264
#define LESS 265
#define LESS_EQUAL 266
#define GREATER 267
#define GREATER_EQUAL 268
#define REGEX_EQUAL 269
#define REGEX_UNEQUAL 270
#define CONTAINS 271
#define PRINT 272
#define SYSTEM 273
#define SUSPEND 274
#define RETURN 275
#define PROCESS 276
#define HAS 277
#define CHILDREN 278
#define OPTION_SUSPEND_DELAY 279
#define OPTION_REFRESH_DELAY 280
#define OPTION_REFRESH_DURATION 281
#define COND_TYPE 282
#define COND_STACKPOSITION 283
#define COND_STATE 284
#define COND_WORKSPACE_NUMBER 285
#define NOT 286
#define AND_AND 287
#define OR_OR 288

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 47 "config.y"

  int number;
  char *string;
  WnckWindowType type;
  WnckWindowState state;
  WSWindowString field;
  comparison_type comparison;
  struct Statement *statement;
  struct Conditional *conditional;
  GSList *string_list;

#line 135 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
