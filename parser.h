/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TM_NAT = 258,
     TM_IDENT = 259,
     TM_LEFT_BRACE = 260,
     TM_RIGHT_BRACE = 261,
     TM_LEFT_PAREN = 262,
     TM_RIGHT_PAREN = 263,
     TM_SEMICOL = 264,
     TM_MALLOC = 265,
     TM_RI = 266,
     TM_RC = 267,
     TM_WI = 268,
     TM_WC = 269,
     TM_VAR = 270,
     TM_IF = 271,
     TM_THEN = 272,
     TM_ELSE = 273,
     TM_WHILE = 274,
     TM_DO = 275,
     TM_SKIP = 276,
     TM_INT = 277,
     TM_SHORT = 278,
     TM_LONG = 279,
     TM_BOOL = 280,
     TM_ASGNOP = 281,
     TM_OR = 282,
     TM_AND = 283,
     TM_NOT = 284,
     TM_LT = 285,
     TM_LE = 286,
     TM_GT = 287,
     TM_GE = 288,
     TM_EQ = 289,
     TM_NE = 290,
     TM_PLUS = 291,
     TM_MINUS = 292,
     TM_MUL = 293,
     TM_DIV = 294,
     TM_MOD = 295,
     TM_BAND = 296
   };
#endif
/* Tokens.  */
#define TM_NAT 258
#define TM_IDENT 259
#define TM_LEFT_BRACE 260
#define TM_RIGHT_BRACE 261
#define TM_LEFT_PAREN 262
#define TM_RIGHT_PAREN 263
#define TM_SEMICOL 264
#define TM_MALLOC 265
#define TM_RI 266
#define TM_RC 267
#define TM_WI 268
#define TM_WC 269
#define TM_VAR 270
#define TM_IF 271
#define TM_THEN 272
#define TM_ELSE 273
#define TM_WHILE 274
#define TM_DO 275
#define TM_SKIP 276
#define TM_INT 277
#define TM_SHORT 278
#define TM_LONG 279
#define TM_BOOL 280
#define TM_ASGNOP 281
#define TM_OR 282
#define TM_AND 283
#define TM_NOT 284
#define TM_LT 285
#define TM_LE 286
#define TM_GT 287
#define TM_GE 288
#define TM_EQ 289
#define TM_NE 290
#define TM_PLUS 291
#define TM_MINUS 292
#define TM_MUL 293
#define TM_DIV 294
#define TM_MOD 295
#define TM_BAND 296




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 11 "lang.y"
{
unsigned int n;
char * i;
struct Expr * e;
struct Cmd * c;
struct VarType * t;
void * none;
}
/* Line 1529 of yacc.c.  */
#line 140 "parser.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

