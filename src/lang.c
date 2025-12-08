#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lang.h"

struct VarType *new_VarType_BASIC(enum BasicVarType t)
{
  struct VarType *res =
      (struct VarType *)malloc(sizeof(struct VarType));
  res->tag = T_BASIC;
  res->tbasic = t;
  return res;
}

struct VarType *new_VarType_PTR(struct VarType *t)
{
  struct VarType *res =
      (struct VarType *)malloc(sizeof(struct VarType));
  res->tag = T_PTR;
  res->tptr.pointt = t;
  return res;
}

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

struct Cmd *TSkip()
{
  struct Cmd *res = new_Cmd_ptr();
  res->t = T_SKIP;
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

struct Cmd *TIf(struct Expr *cond,
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

struct Cmd *TWhile(struct Expr *cond,
                   struct Cmd *body)
{
  struct Cmd *res = new_Cmd_ptr();
  res->t = T_WHILE;
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
  return res;
}

unsigned int build_nat(const char *c, int len) {
  int s = 0, i = 0;
  for (i = 0; i < len; ++i) {
    if (s > 429496729) {
      printf("We cannot handle natural numbers greater than 4294967295.\n");
      exit(0);
    }
    if (s == 429496729 && c[i] > '5') {
      printf("We cannot handle natural numbers greater than 4294967295.\n");
      exit(0);
    }
    s = s * 10 + (c[i] - '0');
  }
  return s;
}

char *new_str(const char *str, int len) {
  char *res = (char *)malloc(sizeof(char) * (len + 1));
  if (res == NULL) {
    printf("Failure in malloc.\n");
    exit(0);
  }
  memcpy(res, str, len);
  res[len] = '\0';
  return res;
}

void print_binop(enum BinOpType op) {
  switch (op) {
  case T_PLUS:
    printf("PLUS");
    break;
  case T_MINUS:
    printf("MINUS");
    break;
  case T_MUL:
    printf("MUL");
    break;
  case T_LT:
    printf("LT");
    break;
  case T_GT:
    printf("GT");
    break;
  case T_LE:
    printf("LE");
    break;
  case T_GE:
    printf("GE");
    break;
  case T_EQ:
    printf("EQ");
    break;
  case T_NE:
    printf("NE");
    break;
  case T_AND:
    printf("AND");
    break;
  case T_OR:
    printf("OR");
    break;
  }
}

void print_unop(enum UnOpType op) {
  switch (op) {
  case T_NOT:
    printf("NOT");
    break;
  case T_NEG:
    printf("NEG");
    break;
  }
}


void print_expr(struct Expr * e) {
  switch (e -> t) {
  case T_CONST:
    printf("CONST(%d)", e -> d.CONST.value);
    break;
  case T_VAR:
    printf("VAR(%s)", e -> d.VAR.name);
    break;
  case T_BINOP:
    print_binop(e -> d.BINOP.op);
    printf("(");
    print_expr(e -> d.BINOP.left);
    printf(",");
    print_expr(e -> d.BINOP.right);
    printf(")");
    break;
  case T_UNOP:
    print_unop(e -> d.UNOP.op);
    printf("(");
    print_expr(e -> d.UNOP.right);
    printf(")");
    break;
  case T_DEREF:
    printf("DEREF(");
    print_expr(e -> d.DEREF.right);
    printf(")");
    break;
  case T_ADDROF:
    printf("ADDROF(");
    print_expr(e -> d.ADDROF.right);
    printf(")");
    break;
  case T_TYPECONV:
    printf("TYPECONV(");
    // print_type(e -> d.TYPECONV.t); // 如果有打印类型的函数
    print_expr(e -> d.TYPECONV.right);
    printf(")");
    break;
  }
}

void print_cmd(struct Cmd * c) {
  switch (c -> t) {
  case T_ASGN:
    printf("ASGN(");
    printf("%s", c -> d.ASGN.left);
    printf(",");
    print_expr(c -> d.ASGN.right);
    printf(")");
    break;
  case T_ASGNDREF:
    printf("ASGNDREF(");
    print_expr(c -> d.ASGNDREF.left);
    printf(",");
    print_expr(c -> d.ASGNDREF.right);
    printf(")");
    break;
  case T_SKIP:
    printf("SKIP");
    break;
  case T_VARDECLARE:
    printf("VARDECLARE(%s)", c -> d.VARDECLARE.var_name);
    break;
  case T_SEQ:
    printf("SEQ(");
    print_cmd(c -> d.SEQ.left);
    printf(",");
    print_cmd(c -> d.SEQ.right);
    printf(")");
    break;
  case T_IF:
    printf("IF(");
    print_expr(c -> d.IF.cond);
    printf(",");
    print_cmd(c -> d.IF.left);
    printf(",");
    print_cmd(c -> d.IF.right);
    printf(")");
    break;
  case T_WHILE:
    printf("WHILE(");
    print_expr(c -> d.WHILE.cond);
    printf(",");
    print_cmd(c -> d.WHILE.body);
    printf(")");
    break;
  }
}