/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     _ERROR_ = 258,
     _SYMB_0 = 259,
     _SYMB_1 = 260,
     _SYMB_2 = 261,
     _SYMB_3 = 262,
     _SYMB_4 = 263,
     _SYMB_5 = 264,
     _SYMB_6 = 265,
     _SYMB_7 = 266,
     _SYMB_8 = 267,
     _SYMB_9 = 268,
     _SYMB_10 = 269,
     _SYMB_11 = 270,
     _SYMB_12 = 271,
     _SYMB_13 = 272,
     _SYMB_14 = 273,
     _SYMB_15 = 274,
     _SYMB_16 = 275,
     _SYMB_17 = 276,
     _SYMB_18 = 277,
     _SYMB_19 = 278,
     _SYMB_20 = 279,
     _SYMB_21 = 280,
     _SYMB_22 = 281,
     _SYMB_23 = 282,
     _SYMB_24 = 283,
     _SYMB_25 = 284,
     _SYMB_26 = 285,
     _SYMB_27 = 286,
     _SYMB_28 = 287,
     _SYMB_29 = 288,
     _SYMB_30 = 289,
     _SYMB_31 = 290,
     _SYMB_32 = 291,
     _SYMB_33 = 292,
     _SYMB_34 = 293,
     _SYMB_35 = 294,
     _SYMB_36 = 295,
     _SYMB_37 = 296,
     _SYMB_38 = 297,
     _SYMB_39 = 298,
     _STRING_ = 299,
     _INTEGER_ = 300,
     _IDENT_ = 301
   };
#endif
/* Tokens.  */
#define _ERROR_ 258
#define _SYMB_0 259
#define _SYMB_1 260
#define _SYMB_2 261
#define _SYMB_3 262
#define _SYMB_4 263
#define _SYMB_5 264
#define _SYMB_6 265
#define _SYMB_7 266
#define _SYMB_8 267
#define _SYMB_9 268
#define _SYMB_10 269
#define _SYMB_11 270
#define _SYMB_12 271
#define _SYMB_13 272
#define _SYMB_14 273
#define _SYMB_15 274
#define _SYMB_16 275
#define _SYMB_17 276
#define _SYMB_18 277
#define _SYMB_19 278
#define _SYMB_20 279
#define _SYMB_21 280
#define _SYMB_22 281
#define _SYMB_23 282
#define _SYMB_24 283
#define _SYMB_25 284
#define _SYMB_26 285
#define _SYMB_27 286
#define _SYMB_28 287
#define _SYMB_29 288
#define _SYMB_30 289
#define _SYMB_31 290
#define _SYMB_32 291
#define _SYMB_33 292
#define _SYMB_34 293
#define _SYMB_35 294
#define _SYMB_36 295
#define _SYMB_37 296
#define _SYMB_38 297
#define _SYMB_39 298
#define _STRING_ 299
#define _INTEGER_ 300
#define _IDENT_ 301




/* Copy the first part of user declarations.  */
#line 2 "Latte.y"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include "Absyn.H"
typedef struct yy_buffer_state *YY_BUFFER_STATE;
int yyparse(void);
int yylex(void);
YY_BUFFER_STATE yy_scan_string(const char *str);
void yy_delete_buffer(YY_BUFFER_STATE buf);
int yy_mylinenumber;
int initialize_lexer(FILE * inp);
int yywrap(void)
{
  return 1;
}
void yyerror(const char *str)
{
  extern char *yytext;
  fprintf(stderr,"error: line %d: %s at %s\n", 
    yy_mylinenumber, str, yytext);
}



static Program* YY_RESULT_Program_ = 0;
Program* pProgram(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Program_;
  }
}
Program* pProgram(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Program_;
  }
}

static TopDef* YY_RESULT_TopDef_ = 0;
TopDef* pTopDef(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_TopDef_;
  }
}
TopDef* pTopDef(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_TopDef_;
  }
}

static ListTopDef* YY_RESULT_ListTopDef_ = 0;
ListTopDef* pListTopDef(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListTopDef_;
  }
}
ListTopDef* pListTopDef(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListTopDef_;
  }
}

static Arg* YY_RESULT_Arg_ = 0;
Arg* pArg(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Arg_;
  }
}
Arg* pArg(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Arg_;
  }
}

static ListArg* YY_RESULT_ListArg_ = 0;
ListArg* pListArg(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListArg_;
  }
}
ListArg* pListArg(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListArg_;
  }
}

static Block* YY_RESULT_Block_ = 0;
Block* pBlock(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Block_;
  }
}
Block* pBlock(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Block_;
  }
}

static ListStmt* YY_RESULT_ListStmt_ = 0;
ListStmt* pListStmt(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListStmt_;
  }
}
ListStmt* pListStmt(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListStmt_;
  }
}

static Stmt* YY_RESULT_Stmt_ = 0;
Stmt* pStmt(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Stmt_;
  }
}
Stmt* pStmt(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Stmt_;
  }
}

static Item* YY_RESULT_Item_ = 0;
Item* pItem(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Item_;
  }
}
Item* pItem(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Item_;
  }
}

static ItemArr* YY_RESULT_ItemArr_ = 0;
ItemArr* pItemArr(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ItemArr_;
  }
}
ItemArr* pItemArr(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ItemArr_;
  }
}

static ListItem* YY_RESULT_ListItem_ = 0;
ListItem* pListItem(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListItem_;
  }
}
ListItem* pListItem(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListItem_;
  }
}

static SimpleType* YY_RESULT_SimpleType_ = 0;
SimpleType* pSimpleType(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_SimpleType_;
  }
}
SimpleType* pSimpleType(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_SimpleType_;
  }
}

static ArrType* YY_RESULT_ArrType_ = 0;
ArrType* pArrType(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ArrType_;
  }
}
ArrType* pArrType(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ArrType_;
  }
}

static Type* YY_RESULT_Type_ = 0;
Type* pType(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Type_;
  }
}
Type* pType(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Type_;
  }
}

static ListSimpleType* YY_RESULT_ListSimpleType_ = 0;
ListSimpleType* pListSimpleType(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListSimpleType_;
  }
}
ListSimpleType* pListSimpleType(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListSimpleType_;
  }
}

static ListArrType* YY_RESULT_ListArrType_ = 0;
ListArrType* pListArrType(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListArrType_;
  }
}
ListArrType* pListArrType(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListArrType_;
  }
}

static ListType* YY_RESULT_ListType_ = 0;
ListType* pListType(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListType_;
  }
}
ListType* pListType(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListType_;
  }
}

static Expr* YY_RESULT_Expr_ = 0;
Expr* pExpr(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Expr_;
  }
}
Expr* pExpr(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Expr_;
  }
}

static ListExpr* YY_RESULT_ListExpr_ = 0;
ListExpr* pListExpr(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListExpr_;
  }
}
ListExpr* pListExpr(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_ListExpr_;
  }
}

static AddOp* YY_RESULT_AddOp_ = 0;
AddOp* pAddOp(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_AddOp_;
  }
}
AddOp* pAddOp(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_AddOp_;
  }
}

static MulOp* YY_RESULT_MulOp_ = 0;
MulOp* pMulOp(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_MulOp_;
  }
}
MulOp* pMulOp(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_MulOp_;
  }
}

static RelOp* YY_RESULT_RelOp_ = 0;
RelOp* pRelOp(FILE *inp)
{
  yy_mylinenumber = 1;
  initialize_lexer(inp);
  if (yyparse())
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_RelOp_;
  }
}
RelOp* pRelOp(const char *str)
{
  YY_BUFFER_STATE buf;
  int result;
  yy_mylinenumber = 1;
  initialize_lexer(0);
  buf = yy_scan_string(str);
  result = yyparse();
  yy_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_RelOp_;
  }
}





/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 759 "Latte.y"
{
  int int_;
  char char_;
  double double_;
  char* string_;
  Program* program_;
  TopDef* topdef_;
  ListTopDef* listtopdef_;
  Arg* arg_;
  ListArg* listarg_;
  Block* block_;
  ListStmt* liststmt_;
  Stmt* stmt_;
  Item* item_;
  ItemArr* itemarr_;
  ListItem* listitem_;
  SimpleType* simpletype_;
  ArrType* arrtype_;
  Type* type_;
  ListSimpleType* listsimpletype_;
  ListArrType* listarrtype_;
  ListType* listtype_;
  Expr* expr_;
  ListExpr* listexpr_;
  AddOp* addop_;
  MulOp* mulop_;
  RelOp* relop_;
}
/* Line 193 of yacc.c.  */
#line 973 "Parser.C"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 986 "Parser.C"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   198

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  47
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  26
/* YYNRULES -- Number of rules.  */
#define YYNRULES  80
/* YYNRULES -- Number of states.  */
#define YYNSTATES  151

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   301

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,    12,    14,    17,    20,    21,    23,
      27,    31,    32,    35,    37,    39,    43,    47,    56,    61,
      69,    73,    80,    84,    91,    95,    98,   104,   112,   118,
     127,   130,   132,   136,   138,   146,   150,   152,   156,   158,
     160,   162,   164,   168,   170,   172,   174,   179,   183,   185,
     187,   189,   194,   196,   200,   203,   206,   208,   212,   214,
     218,   220,   224,   226,   230,   232,   236,   238,   239,   241,
     245,   247,   249,   251,   253,   255,   257,   259,   261,   263,
     265
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      48,     0,    -1,    50,    -1,    61,    46,     4,    52,     5,
      53,    -1,    49,    -1,    49,    50,    -1,    61,    46,    -1,
      -1,    51,    -1,    51,     6,    52,    -1,     7,    54,     8,
      -1,    -1,    54,    55,    -1,     9,    -1,    53,    -1,    59,
      58,     9,    -1,    60,    57,     9,    -1,    46,    10,    38,
      59,    11,    68,    12,     9,    -1,    46,    10,    68,     9,
      -1,    46,    11,    68,    12,    10,    68,     9,    -1,    46,
      13,     9,    -1,    46,    11,    68,    12,    13,     9,    -1,
      46,    14,     9,    -1,    46,    11,    68,    12,    14,     9,
      -1,    39,    68,     9,    -1,    39,     9,    -1,    35,     4,
      68,     5,    55,    -1,    35,     4,    68,     5,    55,    32,
      55,    -1,    43,     4,    68,     5,    55,    -1,    34,     4,
      59,    46,    15,    46,     5,    55,    -1,    68,     9,    -1,
      46,    -1,    46,    10,    68,    -1,    46,    -1,    46,    10,
      38,    59,    11,    68,    12,    -1,    46,    10,    68,    -1,
      56,    -1,    56,     6,    58,    -1,    36,    -1,    40,    -1,
      31,    -1,    42,    -1,    59,    11,    12,    -1,    60,    -1,
      59,    -1,    46,    -1,    46,    11,    68,    12,    -1,    46,
      16,    37,    -1,    45,    -1,    41,    -1,    33,    -1,    46,
       4,    69,     5,    -1,    44,    -1,     4,    68,     5,    -1,
      17,    62,    -1,    18,    62,    -1,    62,    -1,    64,    71,
      63,    -1,    63,    -1,    65,    70,    64,    -1,    64,    -1,
      66,    72,    65,    -1,    65,    -1,    66,    19,    67,    -1,
      66,    -1,    67,    20,    68,    -1,    67,    -1,    -1,    68,
      -1,    68,     6,    69,    -1,    21,    -1,    17,    -1,    22,
      -1,    23,    -1,    24,    -1,    25,    -1,    26,    -1,    27,
      -1,    28,    -1,    29,    -1,    30,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   865,   865,   867,   869,   870,   872,   874,   875,   876,
     878,   880,   881,   883,   884,   885,   886,   887,   888,   889,
     890,   891,   892,   893,   894,   895,   896,   897,   898,   899,
     900,   902,   903,   905,   906,   907,   909,   910,   912,   913,
     914,   915,   917,   919,   920,   934,   935,   936,   937,   938,
     939,   940,   941,   942,   944,   945,   946,   948,   949,   951,
     952,   954,   955,   957,   958,   960,   961,   963,   964,   965,
     967,   968,   970,   971,   972,   974,   975,   976,   977,   978,
     979
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "_ERROR_", "_SYMB_0", "_SYMB_1",
  "_SYMB_2", "_SYMB_3", "_SYMB_4", "_SYMB_5", "_SYMB_6", "_SYMB_7",
  "_SYMB_8", "_SYMB_9", "_SYMB_10", "_SYMB_11", "_SYMB_12", "_SYMB_13",
  "_SYMB_14", "_SYMB_15", "_SYMB_16", "_SYMB_17", "_SYMB_18", "_SYMB_19",
  "_SYMB_20", "_SYMB_21", "_SYMB_22", "_SYMB_23", "_SYMB_24", "_SYMB_25",
  "_SYMB_26", "_SYMB_27", "_SYMB_28", "_SYMB_29", "_SYMB_30", "_SYMB_31",
  "_SYMB_32", "_SYMB_33", "_SYMB_34", "_SYMB_35", "_SYMB_36", "_SYMB_37",
  "_SYMB_38", "_SYMB_39", "_STRING_", "_INTEGER_", "_IDENT_", "$accept",
  "Program", "TopDef", "ListTopDef", "Arg", "ListArg", "Block", "ListStmt",
  "Stmt", "Item", "ItemArr", "ListItem", "SimpleType", "ArrType", "Type",
  "Expr6", "Expr5", "Expr4", "Expr3", "Expr2", "Expr1", "Expr", "ListExpr",
  "AddOp", "MulOp", "RelOp", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    47,    48,    49,    50,    50,    51,    52,    52,    52,
      53,    54,    54,    55,    55,    55,    55,    55,    55,    55,
      55,    55,    55,    55,    55,    55,    55,    55,    55,    55,
      55,    56,    56,    57,    57,    57,    58,    58,    59,    59,
      59,    59,    60,    61,    61,    62,    62,    62,    62,    62,
      62,    62,    62,    62,    63,    63,    63,    64,    64,    65,
      65,    66,    66,    67,    67,    68,    68,    69,    69,    69,
      70,    70,    71,    71,    71,    72,    72,    72,    72,    72,
      72
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     6,     1,     2,     2,     0,     1,     3,
       3,     0,     2,     1,     1,     3,     3,     8,     4,     7,
       3,     6,     3,     6,     3,     2,     5,     7,     5,     8,
       2,     1,     3,     1,     7,     3,     1,     3,     1,     1,
       1,     1,     3,     1,     1,     1,     4,     3,     1,     1,
       1,     4,     1,     3,     2,     2,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     1,     0,     1,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    40,    38,    39,    41,     0,     4,     2,    44,    43,
       0,     1,     5,     0,     0,    42,     7,     8,     0,     0,
       7,     0,     6,     9,    11,     3,     0,     0,    10,    13,
       0,     0,    50,     0,     0,     0,    49,     0,    52,    48,
      45,    14,    12,     0,     0,    56,    58,    60,    62,    64,
      66,     0,    45,     0,    54,    55,     0,     0,    25,     0,
       0,    67,     0,     0,     0,     0,     0,    31,    36,     0,
      33,     0,    72,    73,    74,     0,    71,    70,     0,     0,
      75,    76,    77,    78,    79,    80,     0,     0,    30,     0,
      53,     0,     0,    24,     0,    68,     0,     0,     0,     0,
      20,    22,    47,     0,     0,    15,     0,    16,    57,    59,
      63,    61,    65,     0,     0,     0,     0,    67,    51,     0,
      18,    46,    32,    37,     0,    35,    46,     0,    26,    28,
      69,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      21,    23,     0,     0,    27,     0,    19,     0,    29,    17,
      34
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     5,     6,     7,    17,    18,    41,    26,    42,    68,
      71,    69,    43,    44,    10,    45,    46,    47,    48,    49,
      50,    51,    96,    78,    75,    86
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -98
static const yytype_int16 yypact[] =
{
     103,   -98,   -98,   -98,   -98,     2,   103,   -98,    13,   -98,
     -41,   -98,   -98,    -1,    26,   -98,   103,    39,    53,    14,
     103,    59,   -98,   -98,   -98,   -98,    86,   136,   -98,   -98,
      24,    24,   -98,    67,    70,    55,   -98,    72,   -98,   -98,
      71,   -98,   -98,    -8,    32,   -98,   -98,   126,   -13,    83,
      63,    77,    51,    74,   -98,   -98,   103,   136,   -98,    80,
     136,   136,   119,   136,    89,    98,    78,    82,   108,   109,
     125,   132,   -98,   -98,   -98,   136,   -98,   -98,   136,   136,
     -98,   -98,   -98,   -98,   -98,   -98,   136,   136,   -98,   136,
     -98,    92,   137,   -98,   139,   145,   150,   103,   147,   146,
     -98,   -98,   -98,   136,   113,   -98,   129,   -98,   -98,   126,
     -98,   -13,   -98,   149,   151,     8,     8,   136,   -98,   157,
     -98,    19,   -98,   -98,   103,   -98,   -98,   130,   140,   -98,
     -98,   136,   136,   162,   169,   168,   178,     8,   172,   176,
     -98,   -98,   136,     8,   -98,   177,   -98,   175,   -98,   -98,
     -98
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -98,   -98,   -98,   182,   -98,   170,   171,   -98,   -97,   -98,
     -98,    85,     0,     7,    -6,    -9,   116,   115,   110,   -98,
     118,   -26,    81,   -98,   -98,   -98
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
       8,    53,    11,    13,    76,    14,     8,     9,    77,    59,
      19,    15,    27,     9,    19,    24,     8,    29,   128,   129,
       8,    54,    55,     9,    13,    30,    31,     9,    27,   132,
      16,    92,   133,   134,    94,    95,    98,    99,    67,     1,
     144,    32,    33,    34,     2,    20,   148,    35,     3,    36,
       4,    37,    38,    39,    40,    61,    91,    32,    21,    27,
      22,   112,    89,   113,    58,    36,    24,    66,    38,    39,
      52,    56,    30,    31,    57,    61,    60,   122,    70,    90,
     125,    62,    63,    87,    64,    65,    88,    66,    32,    93,
      27,    95,   103,    24,    28,    29,    36,   119,   100,    38,
      39,    52,    79,    30,    31,   138,   139,   101,    80,    81,
      82,    83,    84,    85,   104,   102,   147,     1,   105,    32,
      33,    34,     2,    27,   135,    35,     3,    36,     4,    37,
      38,    39,    40,    27,     1,   106,    30,    31,   114,     2,
      27,   107,   115,     3,   116,     4,    30,    31,    72,    73,
      74,   117,    32,    30,    31,   118,   120,    97,   121,    67,
      36,   126,    32,    38,    39,    52,   127,   124,   131,    32,
      36,   140,   137,    38,    39,    52,   136,    36,   141,   142,
      38,    39,    52,   143,   145,   146,   149,   150,    12,   123,
      23,   108,    25,   109,     0,     0,   111,   110,   130
};

static const yytype_int16 yycheck[] =
{
       0,    27,     0,    11,    17,    46,     6,     0,    21,    35,
      16,    12,     4,     6,    20,     7,    16,     9,   115,   116,
      20,    30,    31,    16,    11,    17,    18,    20,     4,    10,
       4,    57,    13,    14,    60,    61,    62,    63,    46,    31,
     137,    33,    34,    35,    36,     6,   143,    39,    40,    41,
      42,    43,    44,    45,    46,     4,    56,    33,     5,     4,
      46,    87,    11,    89,     9,    41,     7,    16,    44,    45,
      46,     4,    17,    18,     4,     4,     4,   103,    46,     5,
     106,    10,    11,    20,    13,    14,     9,    16,    33,     9,
       4,   117,    10,     7,     8,     9,    41,    97,     9,    44,
      45,    46,    19,    17,    18,   131,   132,     9,    25,    26,
      27,    28,    29,    30,     6,    37,   142,    31,     9,    33,
      34,    35,    36,     4,   124,    39,    40,    41,    42,    43,
      44,    45,    46,     4,    31,    10,    17,    18,    46,    36,
       4,     9,     5,    40,     5,    42,    17,    18,    22,    23,
      24,     6,    33,    17,    18,     5,     9,    38,    12,    46,
      41,    12,    33,    44,    45,    46,    15,    38,    11,    33,
      41,     9,    32,    44,    45,    46,    46,    41,     9,    11,
      44,    45,    46,     5,    12,     9,     9,    12,     6,   104,
      20,    75,    21,    78,    -1,    -1,    86,    79,   117
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    31,    36,    40,    42,    48,    49,    50,    59,    60,
      61,     0,    50,    11,    46,    12,     4,    51,    52,    61,
       6,     5,    46,    52,     7,    53,    54,     4,     8,     9,
      17,    18,    33,    34,    35,    39,    41,    43,    44,    45,
      46,    53,    55,    59,    60,    62,    63,    64,    65,    66,
      67,    68,    46,    68,    62,    62,     4,     4,     9,    68,
       4,     4,    10,    11,    13,    14,    16,    46,    56,    58,
      46,    57,    22,    23,    24,    71,    17,    21,    70,    19,
      25,    26,    27,    28,    29,    30,    72,    20,     9,    11,
       5,    59,    68,     9,    68,    68,    69,    38,    68,    68,
       9,     9,    37,    10,     6,     9,    10,     9,    63,    64,
      67,    65,    68,    68,    46,     5,     5,     6,     5,    59,
       9,    12,    68,    58,    38,    68,    12,    15,    55,    55,
      69,    11,    10,    13,    14,    59,    46,    32,    68,    68,
       9,     9,    11,     5,    55,    12,     9,    68,    55,     9,
      12
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 865 "Latte.y"
    {  std::reverse((yyvsp[(1) - (1)].listtopdef_)->begin(),(yyvsp[(1) - (1)].listtopdef_)->end()) ;(yyval.program_) = new Prog((yyvsp[(1) - (1)].listtopdef_)); (yyval.program_)->line_number = yy_mylinenumber; YY_RESULT_Program_= (yyval.program_); ;}
    break;

  case 3:
#line 867 "Latte.y"
    {  std::reverse((yyvsp[(4) - (6)].listarg_)->begin(),(yyvsp[(4) - (6)].listarg_)->end()) ;(yyval.topdef_) = new FnDef((yyvsp[(1) - (6)].type_), (yyvsp[(2) - (6)].string_), (yyvsp[(4) - (6)].listarg_), (yyvsp[(6) - (6)].block_)); (yyval.topdef_)->line_number = yy_mylinenumber;  ;}
    break;

  case 4:
#line 869 "Latte.y"
    {  (yyval.listtopdef_) = new ListTopDef() ; (yyval.listtopdef_)->push_back((yyvsp[(1) - (1)].topdef_));  ;}
    break;

  case 5:
#line 870 "Latte.y"
    {  (yyvsp[(2) - (2)].listtopdef_)->push_back((yyvsp[(1) - (2)].topdef_)) ; (yyval.listtopdef_) = (yyvsp[(2) - (2)].listtopdef_) ;  ;}
    break;

  case 6:
#line 872 "Latte.y"
    {  (yyval.arg_) = new Ar((yyvsp[(1) - (2)].type_), (yyvsp[(2) - (2)].string_)); (yyval.arg_)->line_number = yy_mylinenumber;  ;}
    break;

  case 7:
#line 874 "Latte.y"
    {  (yyval.listarg_) = new ListArg();  ;}
    break;

  case 8:
#line 875 "Latte.y"
    {  (yyval.listarg_) = new ListArg() ; (yyval.listarg_)->push_back((yyvsp[(1) - (1)].arg_));  ;}
    break;

  case 9:
#line 876 "Latte.y"
    {  (yyvsp[(3) - (3)].listarg_)->push_back((yyvsp[(1) - (3)].arg_)) ; (yyval.listarg_) = (yyvsp[(3) - (3)].listarg_) ;  ;}
    break;

  case 10:
#line 878 "Latte.y"
    {  (yyval.block_) = new Blk((yyvsp[(2) - (3)].liststmt_)); (yyval.block_)->line_number = yy_mylinenumber;  ;}
    break;

  case 11:
#line 880 "Latte.y"
    {  (yyval.liststmt_) = new ListStmt();  ;}
    break;

  case 12:
#line 881 "Latte.y"
    {  (yyvsp[(1) - (2)].liststmt_)->push_back((yyvsp[(2) - (2)].stmt_)) ; (yyval.liststmt_) = (yyvsp[(1) - (2)].liststmt_) ;  ;}
    break;

  case 13:
#line 883 "Latte.y"
    {  (yyval.stmt_) = new Empty(); (yyval.stmt_)->line_number = yy_mylinenumber;  ;}
    break;

  case 14:
#line 884 "Latte.y"
    {  (yyval.stmt_) = new BStmt((yyvsp[(1) - (1)].block_)); (yyval.stmt_)->line_number = yy_mylinenumber;  ;}
    break;

  case 15:
#line 885 "Latte.y"
    {  std::reverse((yyvsp[(2) - (3)].listitem_)->begin(),(yyvsp[(2) - (3)].listitem_)->end()) ;(yyval.stmt_) = new Decl((yyvsp[(1) - (3)].simpletype_), (yyvsp[(2) - (3)].listitem_)); (yyval.stmt_)->line_number = yy_mylinenumber;  ;}
    break;

  case 16:
#line 886 "Latte.y"
    {  (yyval.stmt_) = new DeclArr((yyvsp[(1) - (3)].arrtype_), (yyvsp[(2) - (3)].itemarr_)); (yyval.stmt_)->line_number = yy_mylinenumber;  ;}
    break;

  case 17:
#line 887 "Latte.y"
    {  (yyval.stmt_) = new InitArrSt((yyvsp[(1) - (8)].string_), (yyvsp[(4) - (8)].simpletype_), (yyvsp[(6) - (8)].expr_)); (yyval.stmt_)->line_number = yy_mylinenumber;  ;}
    break;

  case 18:
#line 888 "Latte.y"
    {  (yyval.stmt_) = new Ass((yyvsp[(1) - (4)].string_), (yyvsp[(3) - (4)].expr_)); (yyval.stmt_)->line_number = yy_mylinenumber;  ;}
    break;

  case 19:
#line 889 "Latte.y"
    {  (yyval.stmt_) = new AssArr((yyvsp[(1) - (7)].string_), (yyvsp[(3) - (7)].expr_), (yyvsp[(6) - (7)].expr_)); (yyval.stmt_)->line_number = yy_mylinenumber;  ;}
    break;

  case 20:
#line 890 "Latte.y"
    {  (yyval.stmt_) = new Incr((yyvsp[(1) - (3)].string_)); (yyval.stmt_)->line_number = yy_mylinenumber;  ;}
    break;

  case 21:
#line 891 "Latte.y"
    {  (yyval.stmt_) = new IncrArr((yyvsp[(1) - (6)].string_), (yyvsp[(3) - (6)].expr_)); (yyval.stmt_)->line_number = yy_mylinenumber;  ;}
    break;

  case 22:
#line 892 "Latte.y"
    {  (yyval.stmt_) = new Decr((yyvsp[(1) - (3)].string_)); (yyval.stmt_)->line_number = yy_mylinenumber;  ;}
    break;

  case 23:
#line 893 "Latte.y"
    {  (yyval.stmt_) = new DecrArr((yyvsp[(1) - (6)].string_), (yyvsp[(3) - (6)].expr_)); (yyval.stmt_)->line_number = yy_mylinenumber;  ;}
    break;

  case 24:
#line 894 "Latte.y"
    {  (yyval.stmt_) = new Ret((yyvsp[(2) - (3)].expr_)); (yyval.stmt_)->line_number = yy_mylinenumber;  ;}
    break;

  case 25:
#line 895 "Latte.y"
    {  (yyval.stmt_) = new VRet(); (yyval.stmt_)->line_number = yy_mylinenumber;  ;}
    break;

  case 26:
#line 896 "Latte.y"
    {  (yyval.stmt_) = new Cond((yyvsp[(3) - (5)].expr_), (yyvsp[(5) - (5)].stmt_)); (yyval.stmt_)->line_number = yy_mylinenumber;  ;}
    break;

  case 27:
#line 897 "Latte.y"
    {  (yyval.stmt_) = new CondElse((yyvsp[(3) - (7)].expr_), (yyvsp[(5) - (7)].stmt_), (yyvsp[(7) - (7)].stmt_)); (yyval.stmt_)->line_number = yy_mylinenumber;  ;}
    break;

  case 28:
#line 898 "Latte.y"
    {  (yyval.stmt_) = new While((yyvsp[(3) - (5)].expr_), (yyvsp[(5) - (5)].stmt_)); (yyval.stmt_)->line_number = yy_mylinenumber;  ;}
    break;

  case 29:
#line 899 "Latte.y"
    {  (yyval.stmt_) = new ForEach((yyvsp[(3) - (8)].simpletype_), (yyvsp[(4) - (8)].string_), (yyvsp[(6) - (8)].string_), (yyvsp[(8) - (8)].stmt_)); (yyval.stmt_)->line_number = yy_mylinenumber;  ;}
    break;

  case 30:
#line 900 "Latte.y"
    {  (yyval.stmt_) = new SExp((yyvsp[(1) - (2)].expr_)); (yyval.stmt_)->line_number = yy_mylinenumber;  ;}
    break;

  case 31:
#line 902 "Latte.y"
    {  (yyval.item_) = new NoInit((yyvsp[(1) - (1)].string_)); (yyval.item_)->line_number = yy_mylinenumber;  ;}
    break;

  case 32:
#line 903 "Latte.y"
    {  (yyval.item_) = new Init((yyvsp[(1) - (3)].string_), (yyvsp[(3) - (3)].expr_)); (yyval.item_)->line_number = yy_mylinenumber;  ;}
    break;

  case 33:
#line 905 "Latte.y"
    {  (yyval.itemarr_) = new NoInitArr((yyvsp[(1) - (1)].string_)); (yyval.itemarr_)->line_number = yy_mylinenumber;  ;}
    break;

  case 34:
#line 906 "Latte.y"
    {  (yyval.itemarr_) = new InitArr((yyvsp[(1) - (7)].string_), (yyvsp[(4) - (7)].simpletype_), (yyvsp[(6) - (7)].expr_)); (yyval.itemarr_)->line_number = yy_mylinenumber;  ;}
    break;

  case 35:
#line 907 "Latte.y"
    {  (yyval.itemarr_) = new InitArrE((yyvsp[(1) - (3)].string_), (yyvsp[(3) - (3)].expr_)); (yyval.itemarr_)->line_number = yy_mylinenumber;  ;}
    break;

  case 36:
#line 909 "Latte.y"
    {  (yyval.listitem_) = new ListItem() ; (yyval.listitem_)->push_back((yyvsp[(1) - (1)].item_));  ;}
    break;

  case 37:
#line 910 "Latte.y"
    {  (yyvsp[(3) - (3)].listitem_)->push_back((yyvsp[(1) - (3)].item_)) ; (yyval.listitem_) = (yyvsp[(3) - (3)].listitem_) ;  ;}
    break;

  case 38:
#line 912 "Latte.y"
    {  (yyval.simpletype_) = new Int(); (yyval.simpletype_)->line_number = yy_mylinenumber;  ;}
    break;

  case 39:
#line 913 "Latte.y"
    {  (yyval.simpletype_) = new Str(); (yyval.simpletype_)->line_number = yy_mylinenumber;  ;}
    break;

  case 40:
#line 914 "Latte.y"
    {  (yyval.simpletype_) = new Bool(); (yyval.simpletype_)->line_number = yy_mylinenumber;  ;}
    break;

  case 41:
#line 915 "Latte.y"
    {  (yyval.simpletype_) = new Void(); (yyval.simpletype_)->line_number = yy_mylinenumber;  ;}
    break;

  case 42:
#line 917 "Latte.y"
    {  (yyval.arrtype_) = new Arr((yyvsp[(1) - (3)].simpletype_)); (yyval.arrtype_)->line_number = yy_mylinenumber;  ;}
    break;

  case 43:
#line 919 "Latte.y"
    {  (yyval.type_) = new AbsAT((yyvsp[(1) - (1)].arrtype_)); (yyval.type_)->line_number = yy_mylinenumber;  ;}
    break;

  case 44:
#line 920 "Latte.y"
    {  (yyval.type_) = new AbsST((yyvsp[(1) - (1)].simpletype_)); (yyval.type_)->line_number = yy_mylinenumber;  ;}
    break;

  case 45:
#line 934 "Latte.y"
    {  (yyval.expr_) = new EVar((yyvsp[(1) - (1)].string_)); (yyval.expr_)->line_number = yy_mylinenumber;  ;}
    break;

  case 46:
#line 935 "Latte.y"
    {  (yyval.expr_) = new EVarArr((yyvsp[(1) - (4)].string_), (yyvsp[(3) - (4)].expr_)); (yyval.expr_)->line_number = yy_mylinenumber;  ;}
    break;

  case 47:
#line 936 "Latte.y"
    {  (yyval.expr_) = new EAtr((yyvsp[(1) - (3)].string_)); (yyval.expr_)->line_number = yy_mylinenumber;  ;}
    break;

  case 48:
#line 937 "Latte.y"
    {  (yyval.expr_) = new ELitInt((yyvsp[(1) - (1)].int_)); (yyval.expr_)->line_number = yy_mylinenumber;  ;}
    break;

  case 49:
#line 938 "Latte.y"
    {  (yyval.expr_) = new ELitTrue(); (yyval.expr_)->line_number = yy_mylinenumber;  ;}
    break;

  case 50:
#line 939 "Latte.y"
    {  (yyval.expr_) = new ELitFalse(); (yyval.expr_)->line_number = yy_mylinenumber;  ;}
    break;

  case 51:
#line 940 "Latte.y"
    {  std::reverse((yyvsp[(3) - (4)].listexpr_)->begin(),(yyvsp[(3) - (4)].listexpr_)->end()) ;(yyval.expr_) = new EApp((yyvsp[(1) - (4)].string_), (yyvsp[(3) - (4)].listexpr_)); (yyval.expr_)->line_number = yy_mylinenumber;  ;}
    break;

  case 52:
#line 941 "Latte.y"
    {  (yyval.expr_) = new EString((yyvsp[(1) - (1)].string_)); (yyval.expr_)->line_number = yy_mylinenumber;  ;}
    break;

  case 53:
#line 942 "Latte.y"
    {  (yyval.expr_) = (yyvsp[(2) - (3)].expr_);  ;}
    break;

  case 54:
#line 944 "Latte.y"
    {  (yyval.expr_) = new Neg((yyvsp[(2) - (2)].expr_)); (yyval.expr_)->line_number = yy_mylinenumber;  ;}
    break;

  case 55:
#line 945 "Latte.y"
    {  (yyval.expr_) = new Not((yyvsp[(2) - (2)].expr_)); (yyval.expr_)->line_number = yy_mylinenumber;  ;}
    break;

  case 56:
#line 946 "Latte.y"
    {  (yyval.expr_) = (yyvsp[(1) - (1)].expr_);  ;}
    break;

  case 57:
#line 948 "Latte.y"
    {  (yyval.expr_) = new EMul((yyvsp[(1) - (3)].expr_), (yyvsp[(2) - (3)].mulop_), (yyvsp[(3) - (3)].expr_)); (yyval.expr_)->line_number = yy_mylinenumber;  ;}
    break;

  case 58:
#line 949 "Latte.y"
    {  (yyval.expr_) = (yyvsp[(1) - (1)].expr_);  ;}
    break;

  case 59:
#line 951 "Latte.y"
    {  (yyval.expr_) = new EAdd((yyvsp[(1) - (3)].expr_), (yyvsp[(2) - (3)].addop_), (yyvsp[(3) - (3)].expr_)); (yyval.expr_)->line_number = yy_mylinenumber;  ;}
    break;

  case 60:
#line 952 "Latte.y"
    {  (yyval.expr_) = (yyvsp[(1) - (1)].expr_);  ;}
    break;

  case 61:
#line 954 "Latte.y"
    {  (yyval.expr_) = new ERel((yyvsp[(1) - (3)].expr_), (yyvsp[(2) - (3)].relop_), (yyvsp[(3) - (3)].expr_)); (yyval.expr_)->line_number = yy_mylinenumber;  ;}
    break;

  case 62:
#line 955 "Latte.y"
    {  (yyval.expr_) = (yyvsp[(1) - (1)].expr_);  ;}
    break;

  case 63:
#line 957 "Latte.y"
    {  (yyval.expr_) = new EAnd((yyvsp[(1) - (3)].expr_), (yyvsp[(3) - (3)].expr_)); (yyval.expr_)->line_number = yy_mylinenumber;  ;}
    break;

  case 64:
#line 958 "Latte.y"
    {  (yyval.expr_) = (yyvsp[(1) - (1)].expr_);  ;}
    break;

  case 65:
#line 960 "Latte.y"
    {  (yyval.expr_) = new EOr((yyvsp[(1) - (3)].expr_), (yyvsp[(3) - (3)].expr_)); (yyval.expr_)->line_number = yy_mylinenumber;  ;}
    break;

  case 66:
#line 961 "Latte.y"
    {  (yyval.expr_) = (yyvsp[(1) - (1)].expr_);  ;}
    break;

  case 67:
#line 963 "Latte.y"
    {  (yyval.listexpr_) = new ListExpr();  ;}
    break;

  case 68:
#line 964 "Latte.y"
    {  (yyval.listexpr_) = new ListExpr() ; (yyval.listexpr_)->push_back((yyvsp[(1) - (1)].expr_));  ;}
    break;

  case 69:
#line 965 "Latte.y"
    {  (yyvsp[(3) - (3)].listexpr_)->push_back((yyvsp[(1) - (3)].expr_)) ; (yyval.listexpr_) = (yyvsp[(3) - (3)].listexpr_) ;  ;}
    break;

  case 70:
#line 967 "Latte.y"
    {  (yyval.addop_) = new Plus(); (yyval.addop_)->line_number = yy_mylinenumber;  ;}
    break;

  case 71:
#line 968 "Latte.y"
    {  (yyval.addop_) = new Minus(); (yyval.addop_)->line_number = yy_mylinenumber;  ;}
    break;

  case 72:
#line 970 "Latte.y"
    {  (yyval.mulop_) = new Times(); (yyval.mulop_)->line_number = yy_mylinenumber;  ;}
    break;

  case 73:
#line 971 "Latte.y"
    {  (yyval.mulop_) = new Div(); (yyval.mulop_)->line_number = yy_mylinenumber;  ;}
    break;

  case 74:
#line 972 "Latte.y"
    {  (yyval.mulop_) = new Mod(); (yyval.mulop_)->line_number = yy_mylinenumber;  ;}
    break;

  case 75:
#line 974 "Latte.y"
    {  (yyval.relop_) = new LTH(); (yyval.relop_)->line_number = yy_mylinenumber;  ;}
    break;

  case 76:
#line 975 "Latte.y"
    {  (yyval.relop_) = new LE(); (yyval.relop_)->line_number = yy_mylinenumber;  ;}
    break;

  case 77:
#line 976 "Latte.y"
    {  (yyval.relop_) = new GTH(); (yyval.relop_)->line_number = yy_mylinenumber;  ;}
    break;

  case 78:
#line 977 "Latte.y"
    {  (yyval.relop_) = new GE(); (yyval.relop_)->line_number = yy_mylinenumber;  ;}
    break;

  case 79:
#line 978 "Latte.y"
    {  (yyval.relop_) = new EQU(); (yyval.relop_)->line_number = yy_mylinenumber;  ;}
    break;

  case 80:
#line 979 "Latte.y"
    {  (yyval.relop_) = new NE(); (yyval.relop_)->line_number = yy_mylinenumber;  ;}
    break;


/* Line 1267 of yacc.c.  */
#line 2724 "Parser.C"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



