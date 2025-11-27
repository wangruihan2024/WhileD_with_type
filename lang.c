#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lang.h"

struct Expr *new_Expr_ptr()
{
  struct Expr *res =
      (struct Expr *)malloc(sizeof(struct Expr));
  if (res == NULL)
  {
    printf("Failure in malloc.\n");
    exit(0);
  }
  return res;
}

struct Cmd *new_Cmd_ptr()
{
  struct Cmd *res = (struct Cmd *)malloc(sizeof(struct Cmd));
  if (res == NULL)
  {
    printf("Failure in malloc.\n");
    exit(0);
  }
  return res;
}

struct Expr *TConst(unsigned int value)
{
  struct Expr *res = new_Expr_ptr();
  res->t = T_CONST;
  res->d.CONST.value = value;
  return res;
}

struct Expr *TVar(char *name)
{
  struct Expr *res = new_Expr_ptr();
  res->t = T_VAR;
  res->d.VAR.name = name;
  return res;
}

struct Expr *TBinOp(enum BinOpType op,
                    struct Expr *left,
                    struct Expr *right)
{
  struct Expr *res = new_Expr_ptr();
  res->t = T_BINOP;
  res->d.BINOP.op = op;
  res->d.BINOP.left = left;
  res->d.BINOP.right = right;
  return res;
}

struct Expr *TUnOP(enum UnOpType op, struct Expr *right)
{
  struct Expr *res = new_Expr_ptr();
  res->t = T_UNOP;
  res->d.UNOP.op = op;
  res->d.UNOP.right = right;
  return res;
}

struct Expr *TDeref(struct Expr *right)
{
  struct Expr *res = new_Expr_ptr();
  res->t = T_DEREF;
  res->d.DEREF.right = right;
  return res;
}

struct Expr *TAddrof(struct Expr *right)
{
  struct Expr *res = new_Expr_ptr();
  res->t = T_ADDROF;
  res->d.ADDROF.right = right;
  return res;
}

struct Expr *TTypeConv(struct VarType *t, struct Expr *right)
{
  struct Expr *res = new_Expr_ptr();
  res->t = T_TYPECONV;
  res->d.TYPECONV.t = t; // 要转换的目标类型
  res->d.TYPECONV.right = right;
  return res;
}

struct Cmd *TAsgn(char *left, struct Expr *right)
{
  struct Cmd *res = new_Cmd_ptr();
  res->t = T_ASGN;
  res->d.ASGN.left = left;
  res->d.ASGN.right = right;
  return res;
}

struct Cmd *TAsgnDref(struct Expr *left, struct Expr *right) // 解引用赋值的特殊情况
{
  struct Cmd *res = new_Cmd_ptr();
  res->t = T_ASGNDREF;
  res->d.ASGNDREF.left = left;
  res->d.ASGNDREF.right = right;
  return res;
}

struct Cmd *TSeq(struct Cmd *left, struct Cmd *right)
{
  struct Cmd *res = new_Cmd_ptr();
  res->t = T_SEQ;
  res->d.SEQ.left = left;
  res->d.SEQ.right = right;
  return res;
}

struct Cmd *TIf(struct expr_bool *cond,
                struct Cmd *left,
                struct Cmd *right)
{
  struct Cmd *res = new_Cmd_ptr();
  res->t = T_IF;
  res->d.IF.cond = cond;
  res->d.IF.left = left;
  res->d.IF.right = right;
  return res;
}

struct Cmd *TWhile(struct expr_bool *inv,
                   struct expr_bool *cond,
                   struct Cmd *body)
{
  struct Cmd *res = new_Cmd_ptr();
  res->t = T_WHILE;
  res->d.WHILE.inv = inv;
  res->d.WHILE.cond = cond;
  res->d.WHILE.body = body;
  return res;
}

struct Cmd *TVarDeclare(struct VarType *t, char *var_name)
{
  struct Cmd *res = new_Cmd_ptr();
  res->t = T_VARDECLARE;
  res->d.VARDECLARE.t = t; // 声明的变量的类型
  res->d.VARDECLARE.var_name = var_name;
}