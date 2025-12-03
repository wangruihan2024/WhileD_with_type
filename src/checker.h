#ifndef CHECKER_H_INCLUDED
#define CHECKER_H_INCLUDED
#include <string>
#include <map>
#include <stdint.h>
#include "lang.h"

struct VarTypeEnv
{
    std::map<std::string, VarType> vartypes;
    struct VarTypeEnv *parent; // 父作用域，全局作用域则为空
};

VarType checkexpr(struct Expr *e, struct VarTypeEnv *env);

// 无隐式转换版本
void checkcmd(struct Cmd *, struct VarTypeEnv *);

// 隐式转换版本
void checkcmd_conv(struct Cmd *, struct VarTypeEnv *);

#endif