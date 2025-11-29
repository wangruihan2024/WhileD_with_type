%{
	// this part is copied to the beginning of the parser 
	#include <stdio.h>
	#include "lang.h"
	#include "lexer.h"
	void yyerror(char *);
	int yylex(void);
  struct Cmd * root;
%}

%union {
unsigned int n;
char * i;
struct Expr * e;
struct Cmd * c;
struct VarType * t;
void * none;
}

// Terminals
%token <n> TM_NAT
%token <i> TM_IDENT
%token <none> TM_LEFT_BRACE TM_RIGHT_BRACE
%token <none> TM_LEFT_PAREN TM_RIGHT_PAREN
%token <none> TM_SEMICOL
%token <none> TM_MALLOC TM_RI TM_RC TM_WI TM_WC
%token <none> TM_VAR TM_IF TM_THEN TM_ELSE TM_WHILE TM_DO TM_SKIP
%token <none> TM_INT TM_SHORT TM_LONG TM_LONGLONG TM_BOOL
%token <none> TM_ASGNOP
%token <none> TM_OR
%token <none> TM_AND
%token <none> TM_NOT
%token <none> TM_LT TM_LE TM_GT TM_GE TM_EQ TM_NE
%token <none> TM_PLUS TM_MINUS
%token <none> TM_MUL TM_DIV TM_MOD
%token <none> TM_BAND

// Nonterminals
%type <c> NT_WHOLE
%type <c> NT_CMD
%type <e> NT_EXPR
%type <t> NT_TYPE

// Priority
%nonassoc TM_ASGNOP
%left TM_OR
%left TM_AND
%left TM_LT TM_LE TM_GT TM_GE TM_EQ TM_NE
%left TM_PLUS TM_MINUS
%left TM_MUL TM_DIV TM_MOD
%left TM_NOT TM_BAND UMINUS  // 设置一个虚拟的 token UMINUS
%right TM_SEMICOL

%%

NT_WHOLE:
  NT_CMD
  {
    $$ = ($1);
    root = $$;
  }
;

NT_CMD:
  TM_SKIP
  {
    $$ = TSkip();
  }
| NT_TYPE TM_IDENT
  {
    $$ = TVarDeclare($1, $2);
  }
| TM_IDENT TM_ASGNOP NT_EXPR
  {
    $$ = TAsgn($1, $3);
  }
| TM_MUL NT_EXPR TM_ASGNOP NT_EXPR
  {
    $$ = TAsgnDref($2, $4);
  }
| NT_CMD TM_SEMICOL NT_CMD
  {
    $$ = TSeq($1, $3);
  }
| TM_IF TM_LEFT_PAREN NT_EXPR TM_RIGHT_PAREN TM_LEFT_BRACE NT_CMD TM_RIGHT_BRACE TM_ELSE TM_LEFT_BRACE NT_CMD TM_RIGHT_BRACE
  {
    $$ = TIf($3, $6, $10);
  }
| TM_WHILE TM_LEFT_PAREN NT_EXPR TM_RIGHT_PAREN TM_LEFT_BRACE NT_CMD TM_RIGHT_BRACE
  {
    $$ = TWhile($3, $6);
  }
;

NT_TYPE:
  TM_INT
  {
    $$ = new_VarType_BASIC(T_INT);
  }
| TM_SHORT
  {
    $$ = new_VarType_BASIC(T_SHORT);
  }
| TM_LONG
  {
    $$ = new_VarType_BASIC(T_LONG);
  }
| TM_BOOL
  {
    $$ = new_VarType_BASIC(T_BOOL);
  }
| TM_LONGLONG
  {
    $$ = new_VarType_BASIC(T_LONGLONG);
  }
| NT_TYPE TM_MUL
  {
    $$ = new_VarType_PTR($1);
  }
;

NT_EXPR:
  TM_NAT
  {
    $$ = TConst($1);
  }
| TM_IDENT
  {
    $$ = TVar($1);
  }
| TM_LEFT_PAREN NT_EXPR TM_RIGHT_PAREN
  {
    $$ = $2;
  }
| TM_NOT NT_EXPR
  {
    $$ = TUnOP(T_NOT, $2);
  }
| TM_MINUS NT_EXPR %prec UMINUS
  {
    $$ = TUnOP(T_NEG, $2);
  }
| TM_MUL NT_EXPR
  {
    $$ = TDeref($2);
  }
| TM_BAND NT_EXPR
  {
    $$ = TAddrof($2);
  }
| TM_LEFT_PAREN NT_TYPE TM_RIGHT_PAREN NT_EXPR
  {
    $$ = TTypeConv($2, $4);
  }
| NT_EXPR TM_PLUS NT_EXPR
  {
    $$ = TBinOp(T_PLUS, $1, $3);
  }
| NT_EXPR TM_MINUS NT_EXPR
  {
    $$ = TBinOp(T_MINUS, $1, $3);
  }
| NT_EXPR TM_MUL NT_EXPR
  {
    $$ = TBinOp(T_MUL, $1, $3);
  }
| NT_EXPR TM_DIV NT_EXPR
  {
    $$ = TBinOp(T_DIV, $1, $3);
  }
| NT_EXPR TM_MOD NT_EXPR
  {
    $$ = TBinOp(T_MOD, $1, $3);
  }
| NT_EXPR TM_AND NT_EXPR
  {
    $$ = TBinOp(T_AND, $1, $3);
  }
| NT_EXPR TM_OR NT_EXPR
  {
    $$ = TBinOp(T_OR, $1, $3);
  }
| NT_EXPR TM_LT NT_EXPR
  {
    $$ = TBinOp(T_LT, $1, $3);
  }
| NT_EXPR TM_GT NT_EXPR
  {
    $$ = TBinOp(T_GT, $1, $3);
  }
| NT_EXPR TM_LE NT_EXPR
  {
    $$ = TBinOp(T_LE, $1, $3);
  }
| NT_EXPR TM_GE NT_EXPR
  {
    $$ = TBinOp(T_GE, $1, $3);
  }
| NT_EXPR TM_EQ NT_EXPR
  {
    $$ = TBinOp(T_EQ, $1, $3);
  }
| NT_EXPR TM_NE NT_EXPR
  {
    $$ = TBinOp(T_NE, $1, $3);
  }
;


%%

void yyerror(char* s)
{
    fprintf(stderr , "%s\n",s);
}
