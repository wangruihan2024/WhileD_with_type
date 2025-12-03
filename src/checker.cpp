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

// 全部修改为直接传递VarType
VarType lookup_vartype(struct VarTypeEnv *env, char *name)
{
    auto it = env->vartypes.find(name);
    if (it != env->vartypes.end())
    {
        switch (it->second.tag)
        {
        case T_BASIC:
            return new_VarType_BASIC(it->second.tbasic);
        case T_PTR:
            return new_VarType_PTR(it->second.tptr.pointt);
        }
    }
    else
    {
        if (env->parent) // 如果父作用域存在，则递归回去寻找
        {
            return lookup_vartype(env->parent, name);
        }
        else // 找到全局作用域还没有获取到类型，则不存在
        {
            std ::cerr << "[Error]: 使用了未声明的变量" << std::endl;
            exit(0);
        }
    }
}

// 直觉理解上，第一步要边建立基础环境，要同时进行类型分析
// 递归执行语句的类型检查
void checkcmd(struct Cmd *c, struct VarTypeEnv *env);

VarType check_binop(struct Expr *e, struct VarTypeEnv *env)
{
    VarType left = checkexpr(e->d.BINOP.left, env);
    VarType right = checkexpr(e->d.BINOP.right, env);
    switch (e->d.BINOP.op)
    {
    case T_PLUS:
    case T_MINUS:
    case T_MUL:
        if (VarTypeCmp(left, right))
        {
            switch (left.tag)
            {
            case T_BASIC:
                return left;
                break;
            case T_PTR:
                std ::cerr << "[Error]: 指针类型不能相加/减/乘" << std::endl;
                exit(0);
            }
        }
        else
        {
            std ::cerr << "[Error]: 操作数类型不一致（没有隐式转换的版本）" << std::endl;
            exit(0);
        }
        break;
    case T_AND:
    case T_OR:
        if (VarTypeCmp(left, right))
        {
            switch (left.tag)
            {
            case T_BASIC:
                if (left.tbasic == T_BOOL)
                    return left;
                else
                {
                    std ::cerr << "[Error]: 整数类型不能与，非（没有隐式转换的版本）" << std::endl;
                    exit(0);
                }
            case T_PTR:
                std ::cerr << "[Error]: 指针类型不能相与，非" << std::endl;
                exit(0);
            }
        }
        else
        {
            std ::cerr << "[Error]: 操作数类型不一致（没有隐式转换的版本）" << std::endl;
            exit(0);
        }
        break;
    case T_LT:
    case T_GT:
    case T_LE:
    case T_GE:
    case T_EQ:
    case T_NE:
        if (VarTypeCmp(left, right))
        {
            switch (left.tag)
            {
            case T_BASIC:
                return new_VarType_BASIC(T_BOOL);
                break;
            case T_PTR:
                std ::cerr << "[Error]: 指针类型不能比较" << std::endl;
                exit(0);
            }
        }
        else
        {
            std ::cerr << "[Error]: 操作数类型不一致（没有隐式转换的版本）" << std::endl;
            exit(0);
        }
        break;
    default:
        std::cerr << "[UNKOWN]: check_binop" << std::endl;
        exit(0);
    }
}
VarType check_unop(struct Expr *e, struct VarTypeEnv *env)
{
    VarType expr_type = checkexpr(e->d.UNOP.right, env);
    if (expr_type.tag == T_PTR)
    {
        std::cerr << "[Error]: 指针类型不支持一元运算符" << std::endl;
        exit(0);
    }
    else
    {
        switch (e->d.UNOP.op)
        {
        case T_NEG:
            if (expr_type.tbasic == T_BOOL)
                std::cerr << "[Error]: 布尔类型不能取负数（无隐式类型转化）" << std::endl;
            else
                return expr_type;
            break;
        case T_NOT:
            if (expr_type.tbasic == T_BOOL)
                return expr_type;
            else
                std::cerr << "[Error]: 整数类型不能取反？（无隐式类型转化）" << std::endl;
        default:
            std::cerr << "[UNKOWN]: check_unop" << std::endl;
            exit(0);
        }
    }
}

// 先定义表达式的类型检查叭
VarType checkexpr(struct Expr *e, struct VarTypeEnv *env)
{
    switch (e->t)
    {
    case T_CONST:
        // 默认落在INT范围内
        // 后续可能还得改一改风格，这种超长链条的内存访问ww
        if (INT32_MIN <= e->d.CONST.value && e->d.CONST.value <= INT32_MAX)
            return new_VarType_BASIC(T_INT);
        else
            return new_VarType_BASIC(T_LONGLONG);
        break;
    case T_VAR:
        return lookup_vartype(env, e->d.VAR.name); // 支持父域查找
    case T_BINOP:
        return check_binop(e, env);
        break;
    case T_UNOP:
        return check_unop(e, env);
        break;
    case T_DEREF:
    {
        VarType t = checkexpr(e->d.DEREF.right, env);
        switch (t.tag) // 根据tag来跟踪union中到底是谁有效
        {
        case T_PTR:
            return t.tptr.pointt; // 交出指针包裹的类型
        case T_BASIC:
            std::cerr << "cannot dereference non-pointer" << std::endl;
            break;
        default:
            std::cerr << "[UNKOWN]: dereference" << std::endl;
            break;
        }
    }

    case T_ADDROF:
        VarType t = checkexpr(e->d.ADDROF.right, env);
        // TODO：添加检查，只有左值表达式能够允许取地址
        return new_VarType_PTR(t);

    case T_TYPECONV:
        VarType t = checkexpr(e->d.TYPECONV.right, env);
        // TODO: CONVERSION Rules
        return e->d.TYPECONV.t; // conversion result type

    default:
        std::cerr << "[UNKOWN]: checkexpr" << std::endl;
    }
}