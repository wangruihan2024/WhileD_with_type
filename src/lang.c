#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lang.h"

struct VarType new_VarType_BASIC(enum BasicVarType t)
{
  struct VarType res;
  res.tag = T_BASIC;
  res.tbasic = t;
  return res;
}

struct VarType new_VarType_PTR(struct VarType t)
{
  struct VarType res;
  res.tag = T_PTR;
  res.tptr.pointt = t;
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

struct Expr *TConst(unsigned long long value)
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

struct Expr *TTypeConv(struct VarType t, struct Expr *right)
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

struct Cmd *TVarDeclare(struct VarType t, char *var_name)
{
  struct Cmd *res = new_Cmd_ptr();
  res->t = T_VARDECLARE;
  res->d.VARDECLARE.t = t; // 声明的变量的类型
  res->d.VARDECLARE.var_name = var_name;
  return res;
}

unsigned long long build_nat(const char *c, int len)
{
  unsigned long long s = 0;
  int i = 0;
  // 2^63 = 9223372036854775808
  // 阈值 = 922337203685477580
  unsigned long long limit = 922337203685477580ULL;

  for (i = 0; i < len; ++i)
  {
    if (s > limit)
    {
      printf("[ERROR] We cannot handle natural numbers greater than 2^{63}.\n");
      exit(0);
    }

    if (s == limit && c[i] > '8')
    {
      printf("[ERROR] We cannot handle natural numbers greater than 2^{63}.\n");
      exit(0);
    }

    s = s * 10 + (c[i] - '0');
  }
  return s;
}

char *new_str(const char *str, int len)
{
  char *res = (char *)malloc(sizeof(char) * (len + 1));
  if (res == NULL)
  {
    printf("Failure in malloc.\n");
    exit(0);
  }
  memcpy(res, str, len);
  res[len] = '\0';
  return res;
}

void print_binop(enum BinOpType op)
{
  switch (op)
  {
  case T_PLUS:
    printf("ADD");
    break;
  case T_MINUS:
    printf("SUB");
    break;
  case T_MUL:
    printf("MUL");
    break;
  case T_DIV:
    printf("DIV");
    break;
  case T_MOD:
    printf("MOD");
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

void print_unop(enum UnOpType op)
{
  switch (op)
  {
  case T_NOT:
    printf("NOT");
    break;
  case T_NEG:
    printf("NEG");
    break;
  }
}

void print_type(struct VarType t)
{
  if (t.tag == T_BASIC)
  {
    switch (t.tbasic)
    {
    case T_INT:
      printf("INT");
      break;
    case T_SHORT:
      printf("SHORT");
      break;
    case T_LONG:
      printf("LONG");
      break;
    case T_BOOL:
      printf("BOOL");
      break;
    case T_LONGLONG:
      printf("LONGLONG");
      break;
    }
  }
  else if (t.tag == T_PTR)
  {
    print_type(t.tptr.pointt);
    printf("*");
  }
}

void print_expr(struct Expr *e)
{
  switch (e->t)
  {
  case T_CONST:
    printf("CONST(%d)", e->d.CONST.value);
    break;
  case T_VAR:
    printf("VAR(%s)", e->d.VAR.name);
    break;
  case T_BINOP:
    print_binop(e->d.BINOP.op);
    printf("(");
    print_expr(e->d.BINOP.left);
    printf(",");
    print_expr(e->d.BINOP.right);
    printf(")");
    break;
  case T_UNOP:
    print_unop(e->d.UNOP.op);
    printf("(");
    print_expr(e->d.UNOP.right);
    printf(")");
    break;
  case T_DEREF:
    printf("DEREF(");
    print_expr(e->d.DEREF.right);
    printf(")");
    break;
  case T_ADDROF:
    printf("ADDROF(");
    print_expr(e->d.ADDROF.right);
    printf(")");
    break;
  case T_TYPECONV:
    printf("TYPECONV(");
    print_type(e->d.TYPECONV.t);
    printf(",");
    print_expr(e->d.TYPECONV.right);
    printf(")");
    break;
  }
}

void print_cmd(struct Cmd *c)
{
  switch (c->t)
  {
  case T_ASGN:
    printf("ASGN(");
    printf("%s", c->d.ASGN.left);
    printf(",");
    print_expr(c->d.ASGN.right);
    printf(")");
    break;
  case T_ASGNDREF:
    printf("ASGNDREF(");
    print_expr(c->d.ASGNDREF.left);
    printf(",");
    print_expr(c->d.ASGNDREF.right);
    printf(")");
    break;
  case T_SKIP:
    printf("SKIP");
    break;
  case T_VARDECLARE:
    printf("VARDECLARE(%s)", c->d.VARDECLARE.var_name);
    break;
  case T_SEQ:
    printf("SEQ(");
    print_cmd(c->d.SEQ.left);
    printf(",");
    print_cmd(c->d.SEQ.right);
    printf(")");
    break;
  case T_IF:
    printf("IF(");
    print_expr(c->d.IF.cond);
    printf(",");
    print_cmd(c->d.IF.left);
    printf(",");
    print_cmd(c->d.IF.right);
    printf(")");
    break;
  case T_WHILE:
    printf("WHILE(");
    print_expr(c->d.WHILE.cond);
    printf(",");
    print_cmd(c->d.WHILE.body);
    printf(")");
    break;
  }
}

int VarTypeCmp(struct VarType left, struct VarType right)
{
  if (left.tag == right.tag) // 首先是标签相等
  {
    switch (left.tag)
    {
    case T_BASIC:
      if (left.tbasic == right.tbasic)
        return 1; // 类型一致
      else
        return 0;
      break;
    case T_PTR: // 递归下去检查
      return VarTypeCmp(left.tptr.pointt, right.tptr.pointt);
      break;
    default:
      printf("[Error]: unkown tag");
      exit(0);
    }
  }
  else
  {
    return 0;
  }
}