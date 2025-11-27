#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <map>
#include "lang.h"

// checker 即类型检查器，基础是类型表（支持作用域）

struct VarTypeEnv
{
    std::map<char *, VarType> vartypes;
    struct VarTypeEnv *parent; // 父作用域，全局作用域则为空
};

// 直觉理解上，第一步要边建立基础环境，要同时进行类型分析
// 递归执行语句的类型检查
void checkcmd(struct Cmd *c, struct VarTypeEnv *env);

VarType *check_binop(struct Expr *e, struct VarTypeEnv *env)
{
    switch (e->d.BINOP.op)
    {
    case T_PLUS:
        VarType *left = checkexpr(e->d.BINOP.left, env);
        VarType *right = checkexpr(e->d.BINOP.right, env);
        return new_VarType_BASIC(T_LONGLONG); // TODO: 根据结果和求和范围，给定
        break;
    case T_MINUS:
        VarType *left = checkexpr(e->d.BINOP.left, env);
        VarType *right = checkexpr(e->d.BINOP.right, env);
        return new_VarType_BASIC(T_LONGLONG); // TODO: 根据结果和求和范围，给定
        break;
    case T_MUL:
        VarType *left = checkexpr(e->d.BINOP.left, env);
        VarType *right = checkexpr(e->d.BINOP.right, env);
        return new_VarType_BASIC(T_LONGLONG); // TODO: 根据结果和求和范围，给定
        break;
    // TODO: 处理Bool相关的运算符还有8个，数据类型以零为假
    default:
        std::cerr << "[UNKOWN]: check_binop" << std::endl;
        break;
    }
}
VarType *check_unop(struct Expr *e, struct VarTypeEnv *env)
{
    VarType *expr_type = checkexpr(e->d.UNOP.right, env);
    if (expr_type->tag == T_PTR)
    {
        std::cerr << "[Error]: Pointer Type cannot be applied unary operators" << std::endl;
    }
    else
    {
        switch (e->d.UNOP.op)
        {
        case T_NEG:
            if (expr_type->tbasic == T_BOOL)
                std::cerr << "[Error]: Bool Type cannot get negative" << std::endl;
            else // TODO: 进一步添加范围检查
                return expr_type;
        case T_NOT:
            if (expr_type->tbasic == T_BOOL)
                return expr_type;
            else
                std::cerr << "[Error]: Number Type cannot get not" << std::endl;
        default:
            std::cerr << "[UNKOWN]: check_unop" << std::endl;
            break;
        }
    }
    // 这里还有一些默认的return的结果欸🤔我不会抛异常感觉好麻烦🤔
}

// 先定义表达式的类型检查叭
VarType *checkexpr(struct Expr *e, struct VarTypeEnv *env)
{
    switch (e->t)
    {
    case T_CONST:
        // 默认落在INT范围内
        // if (INT32_MIN <= e->d.CONST.value <= INT32_MAX) 笑死这里也是Coq写多了
        // 我认为后续可能还得改一改风格，这种超长链条的内存访问ww
        if (INT32_MIN <= e->d.CONST.value && e->d.CONST.value <= INT32_MAX)
            return new_VarType_BASIC(T_INT);
        else
            return new_VarType_BASIC(T_LONGLONG);
        break;

    case T_VAR:
        auto it = env->vartypes.find(e->d.VAR.name); // TODO: 实现一个lookup，能够支持像父域查找ww
        if (it != env->vartypes.end())
            return &(it->second);
        else
            std::cerr << "[Error]: using variables undefined." << std::endl;
        break;

    case T_BINOP:
        return check_binop(e, env);

    case T_UNOP:
        return check_unop(e, env);

    case T_DEREF:
    {
        VarType *t = checkexpr(e->d.DEREF.right, env);
        switch (t->tag) // 根据tag来跟踪union中到底是谁有效
        {
        case T_PTR:
            return t->tptr.pointt; // 交出指针包裹的类型
        case T_BASIC:
            std::cerr << "cannot dereference non-pointer" << std::endl;
            break;
        default:
            std::cerr << "[UNKOWN]: dereference" << std::endl;
            break;
        }
    }

    case T_ADDROF:
        VarType *t = checkexpr(e->d.ADDROF.right, env);
        return new_VarType_PTR(t);

    case T_TYPECONV:
        checkexpr(e->d.TYPECONV.right, env); // type validity check
        // TODO: CONVERSION Rules
        return e->d.TYPECONV.t; // conversion result type

    default:
        std::cerr << "[UNKOWN]: checkexpr" << std::endl;
    }
}