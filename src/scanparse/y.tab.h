/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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
    BRACKET_L = 258,
    BRACKET_R = 259,
    COMMA = 260,
    SEMICOLON = 261,
    CURLY_BRACKET_L = 262,
    CURLY_BRACKET_R = 263,
    SQUARE_BRACKET_L = 264,
    SQUARE_BRACKET_R = 265,
    MINUS = 266,
    PLUS = 267,
    STAR = 268,
    SLASH = 269,
    MOD = 270,
    LE = 271,
    LT = 272,
    GE = 273,
    GT = 274,
    EQ = 275,
    NE = 276,
    OR = 277,
    AND = 278,
    NOT = 279,
    TRUEVAL = 280,
    FALSEVAL = 281,
    LET = 282,
    INT_TYPE = 283,
    FLOAT_TYPE = 284,
    BOOL_TYPE = 285,
    VOID_TYPE = 286,
    EXPORT = 287,
    RETURN = 288,
    IF = 289,
    ELSE = 290,
    WHILE = 291,
    DO = 292,
    FOR = 293,
    EXTERN = 294,
    NUM = 295,
    FLOAT = 296,
    ID = 297,
    UMINUS = 298,
    THEN = 299
  };
#endif
/* Tokens.  */
#define BRACKET_L 258
#define BRACKET_R 259
#define COMMA 260
#define SEMICOLON 261
#define CURLY_BRACKET_L 262
#define CURLY_BRACKET_R 263
#define SQUARE_BRACKET_L 264
#define SQUARE_BRACKET_R 265
#define MINUS 266
#define PLUS 267
#define STAR 268
#define SLASH 269
#define MOD 270
#define LE 271
#define LT 272
#define GE 273
#define GT 274
#define EQ 275
#define NE 276
#define OR 277
#define AND 278
#define NOT 279
#define TRUEVAL 280
#define FALSEVAL 281
#define LET 282
#define INT_TYPE 283
#define FLOAT_TYPE 284
#define BOOL_TYPE 285
#define VOID_TYPE 286
#define EXPORT 287
#define RETURN 288
#define IF 289
#define ELSE 290
#define WHILE 291
#define DO 292
#define FOR 293
#define EXTERN 294
#define NUM 295
#define FLOAT 296
#define ID 297
#define UMINUS 298
#define THEN 299

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 25 "parser.y"

 char               *id;
 int                 cint;
 float               cflt;
 enum Type           ctype;
 node_st             *node;

#line 153 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
