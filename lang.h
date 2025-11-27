#ifndef LANG_H_INCLUDED
#define LANG_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

enum BasicVarType
{
  T_INT,
  T_SHORT,
  T_LONG,
  T_LONGLONG,
  T_BOOL
};

struct VarType
{
  union
  {
    enum BasicVarType t;
    struct
    {
      struct VarType *pointt; // 指向的类型
    } t;
  };
};

enum BinOpType
{
  T_PLUS,
  T_MINUS,
  T_MUL,

  T_AND,
  T_OR,

  T_LT,
  T_GT,
  T_LE,
  T_GE,
  T_EQ,
  T_NE
};

enum UnOpType
{
  T_NEG,
  T_NOT
};

enum ExprType
{
  T_CONST = 0,
  T_VAR,
  T_BINOP,
  T_UNOP,
  T_DEREF,
  T_ADDROF,
  T_TYPECONV
};

enum CmdType
{
  T_SKIP = 0,
  T_ASGN,
  T_ASGNDREF,
  T_SEQ,
  T_IF,
  T_WHILE,
  T_VARDECLARE
};

struct Expr
{
  enum ExprType t;
  union
  {
    struct
    {
      unsigned int value;
    } CONST;
    struct
    {
      char *name;
    } VAR;
    struct
    {
      enum BinOpType op;
      struct Expr *left;
      struct Expr *right;
    } BINOP;
    struct
    {
      enum UnOpType op;
      struct Expr *right;
    } UNOP;
    struct
    {
      struct Expr *right;
    } DEREF;
    struct
    {
      struct Expr *right;
    } ADDROF;
    struct
    {
      struct VarType *t;
      struct Expr *right;
    } TYPECONV;
  } d;
};

struct Cmd
{
  enum CmdType t;
  union
  {
    struct
    {
      char *left;
      struct Expr *right;
    } ASGN;
    struct
    {
      struct Expr *left;
      struct Expr *right;
    } ASGNDREF;
    struct
    {
      void *__;
    } SKIP;
    struct
    {
      struct Cmd *left;
      struct Cmd *right;
    } SEQ;
    struct
    {
      struct Expr *cond;
      struct Cmd *left;
      struct Cmd *right;
    } IF;
    struct
    {
      struct Expr *inv;
      struct Expr *cond;
      struct Cmd *body;
    } WHILE;
    struct
    {
      struct VarType *t;
      struct Expr *expr;
    } VARDECLARE;
  } d;
};

// 这里在声明构造函数呀，笑死了刚才写注释用的 (* ? *)
struct Expr *TConst(unsigned int);
struct Expr *TVar(char *);
struct Expr *TBinOp(enum BinOpType,
                    struct Expr *,
                    struct Expr *);
struct Expr *TUnOP(enum UnOpType, struct Expr *);
struct Expr *TDeref(struct Expr *);
struct Expr *TAddrof(struct Expr *);
struct Expr *TTypeConv(struct VarType *, struct Expr *);

struct Cmd *TAsgn(char *left, struct Expr *right);
struct Cmd *TAsgnDref(struct Expr *left, struct Expr *right); // 解引用赋值的特殊情况
struct Cmd *TSkip();
struct Cmd *TSeq(struct Cmd *, struct Cmd *);
struct Cmd *TIf(struct Expr *, struct Cmd *, struct Cmd *);
struct Cmd *TWhile(struct Expr *, struct Cmd *);
struct Cmd *TVarDeclare(struct VarType *, struct Expr *);

#endif // LANG_H_INCLUDED
