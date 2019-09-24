/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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
    ID = 258,
    INT = 259,
    FLOAT = 260,
    SCIENTIFIC = 261,
    TYPE = 262,
    READ = 263,
    WHILE = 264,
    DO = 265,
    IF = 266,
    TRUE = 267,
    FALSE = 268,
    FOR = 269,
    CONST = 270,
    PRINT = 271,
    CONTINUE = 272,
    BREAK = 273,
    RETURN = 274,
    STRING = 275,
    VOID = 276,
    AND = 277,
    OR = 278,
    LE = 279,
    NE = 280,
    GE = 281,
    EQ = 282,
    ELSE = 283,
    PARENTHESES = 284,
    UNARY = 285,
    NOT = 286
  };
#endif
/* Tokens.  */
#define ID 258
#define INT 259
#define FLOAT 260
#define SCIENTIFIC 261
#define TYPE 262
#define READ 263
#define WHILE 264
#define DO 265
#define IF 266
#define TRUE 267
#define FALSE 268
#define FOR 269
#define CONST 270
#define PRINT 271
#define CONTINUE 272
#define BREAK 273
#define RETURN 274
#define STRING 275
#define VOID 276
#define AND 277
#define OR 278
#define LE 279
#define NE 280
#define GE 281
#define EQ 282
#define ELSE 283
#define PARENTHESES 284
#define UNARY 285
#define NOT 286

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
