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
    LOGIC_OPERATOR = 263,
    EQUAL = 264,
    UNEQUAL = 265,
    LESS = 266,
    LESS_EQUAL = 267,
    GREATER = 268,
    GREATER_EQUAL = 269,
    REGEX_EQUAL = 270,
    REGEX_UNEQUAL = 271,
    CONTAINS = 272,
    PRINT = 273,
    SYSTEM = 274,
    SUSPEND = 275,
    RETURN = 276,
    PROCESS = 277,
    HAS = 278,
    CHILDREN = 279,
    OPTION_SUSPEND_DELAY = 280,
    OPTION_REFRESH_DELAY = 281,
    OPTION_REFRESH_DURATION = 282,
    COND_TYPE = 283,
    COND_STACKPOSITION = 284,
    COND_STATE = 285,
    COND_WORKSPACE_NUMBER = 286,
    NOT = 287,
    AND_AND = 288,
    OR_OR = 289
  };
#endif
/* Tokens.  */
#define STRING 258
#define NUMBER 259
#define WINDOW_STATE 260
#define WINDOW_TYPE 261
#define WINDOW_FIELD 262
#define LOGIC_OPERATOR 263
#define EQUAL 264
#define UNEQUAL 265
#define LESS 266
#define LESS_EQUAL 267
#define GREATER 268
#define GREATER_EQUAL 269
#define REGEX_EQUAL 270
#define REGEX_UNEQUAL 271
#define CONTAINS 272
#define PRINT 273
#define SYSTEM 274
#define SUSPEND 275
#define RETURN 276
#define PROCESS 277
#define HAS 278
#define CHILDREN 279
#define OPTION_SUSPEND_DELAY 280
#define OPTION_REFRESH_DELAY 281
#define OPTION_REFRESH_DURATION 282
#define COND_TYPE 283
#define COND_STACKPOSITION 284
#define COND_STATE 285
#define COND_WORKSPACE_NUMBER 286
#define NOT 287
#define AND_AND 288
#define OR_OR 289

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 52 "config.y"

  int number;
  char *string;
  WnckWindowType type;
  WnckWindowState state;
  WSWindowString field;
  conditional_type logic;
  comparison_type comparison;
  struct Statement *statement;
  struct Conditional *conditional;
  struct ProcessRule *process_rule;
  GSList *string_list;

#line 139 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
