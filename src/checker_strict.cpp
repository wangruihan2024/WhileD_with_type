#include <stdlib.h>
#include "lang.h"
#include "checker.h"
#include <stdexcept>

void exception(const std::string &msg)
{
    throw std::runtime_error(msg);
}

VarType lookup_vartype_conv(struct VarTypeEnv *env, char *name)
{
    auto it = env->vartypes.find(name);
    if (it != env->vartypes.end())
    {
        return it->second;
    }
    else
    {
        if (env->parent) // 如果父作用域存在，则递归回去寻找
        {
            return lookup_vartype_conv(env->parent, name);
        }
        else // 找到全局作用域还没有获取到类型，则不存在
        {
            exception("[Error]: 使用了未声明的变量");
        }
    }
    return VarType{}; // 永远不会到达这里
}

VarType check_binop_strict(struct Expr *e, struct VarTypeEnv *env)
{
    VarType left = checkexpr_strict(e->d.BINOP.left, env);
    VarType right = checkexpr_strict(e->d.BINOP.right, env);
    switch (e->d.BINOP.op)
    {
    case T_PLUS:
    case T_MINUS:
    case T_MUL:
    case T_DIV:
    case T_MOD:
        if (VarTypeCmp(left, right))
        {
            switch (left.tag)
            {
            case T_BASIC:
                return left;
            case T_PTR:
                exception("[Error]: 指针类型不能相加/减/乘");
            }
        }
        exception("[Error]: 操作数类型不一致（没有隐式转换的版本）");
    case T_AND:
    case T_OR:
        if (VarTypeCmp(left, right))
        {
            switch (left.tag)
            {
            case T_BASIC:
                // exception("[Error]: 整数类型不能与，非（没有隐式转换的版本）");
                return new_VarType_BASIC(T_INT);
            case T_PTR:
                exception("[Error]: 指针类型不能相与，非");
            }
        }
        exception("[Error]: 操作数类型不一致（没有隐式转换的版本）");
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
                return new_VarType_BASIC(T_INT);
            case T_PTR:
                exception("[Error]: 指针类型不能比较");
            }
        }
        exception("[Error]: 操作数类型不一致（没有隐式转换的版本）");
    }
    return VarType{}; // 永远不会到达这里
}
VarType check_unop_strict(struct Expr *e, struct VarTypeEnv *env)
{
    VarType expr_type = checkexpr_strict(e->d.UNOP.right, env);
    if (expr_type.tag == T_PTR)
    {
        exception("[Error]: 指针类型不支持一元运算符");
    }
    else
    {
        switch (e->d.UNOP.op)
        {
        case T_NEG:
            return expr_type;
        case T_NOT:
            return new_VarType_BASIC(T_INT);
        }
    }
    return VarType{}; // 永远不会到达这里
}

// 先定义表达式的类型检查叭
VarType checkexpr_strict(struct Expr *e, struct VarTypeEnv *env)
{
    switch (e->t)
    {
    case T_CONST:
        // 默认落在INT范围内，唯一涉及语义的地方🤔
        // 后续可能还得改一改风格，这种超长链条的内存访问ww
        if (e->d.CONST.value <= INT32_MAX)
            return new_VarType_BASIC(T_INT);
        else
            return new_VarType_BASIC(T_LONGLONG);
    case T_VAR:
        return lookup_vartype_conv(env, e->d.VAR.name); // 支持父域查找
    case T_BINOP:
        return check_binop_strict(e, env);
    case T_UNOP:
        return check_unop_strict(e, env);
    case T_DEREF:
    {
        VarType t = checkexpr_strict(e->d.DEREF.right, env);
        switch (t.tag) // 根据tag来跟踪union中到底是谁有效
        {
        case T_PTR:
            return *t.tptr; // 交出指针包裹的类型
        case T_BASIC:
            exception("[Error]：不能解引用非指针类型");
        }
    }
    case T_ADDROF:
    {
        VarType t = checkexpr_strict(e->d.ADDROF.right, env);
        switch (e->d.ADDROF.right->t) // 表达式，
        {
        case T_VAR:
        case T_DEREF:
            return new_VarType_PTR(t);
        default:
            exception("[Error]: 取地址需要左值表达式");
        }
    }
    case T_TYPECONV:
        VarType src_t = checkexpr_strict(e->d.TYPECONV.right, env);
        VarType dest_t = e->d.TYPECONV.t;
        
        if (src_t.tag == T_BASIC && dest_t.tag == T_BASIC)
        {
            return dest_t; // 基本类型之间直接转换
        }
        else if (src_t.tag == T_PTR && dest_t.tag == T_PTR)
        {
            return dest_t; // 指针之间直接转换
        }
        else
        {
            exception("[Error]: 不支持的类型转换（无隐式转换）");
        }
    }
    return VarType{}; // 永远不会到达这里
}

// 直觉理解上，要边建立基础环境，同时进行类型分析
// 递归执行语句的类型检查
void checkcmd_strict(struct Cmd *c, struct VarTypeEnv *env)
{
    switch (c->t)
    {
    case T_SKIP:
        return; // 无事发生直接退出，出问题的选择是直接退出程序，“编译失败”
    case T_ASGN:
    {
        VarType left_type = lookup_vartype_conv(env, c->d.ASGN.left);
        VarType right_type = checkexpr_strict(c->d.ASGN.right, env);
        if (VarTypeCmp(left_type, right_type))
            return; // 左右类型匹配，则OK
        exception("[Error]: 赋值语句左右类型不匹配（隐式类型转换未支持）");
    }
    case T_ASGNDREF:
    {
        Expr deref;
        deref.t = T_DEREF;
        deref.d.DEREF.right = c->d.ASGNDREF.left;
        VarType left_type = checkexpr_strict(&deref, env); // 帮忙解引用了
        VarType right_type = checkexpr_strict(c->d.ASGNDREF.right, env);
        if (VarTypeCmp(left_type, right_type))
            return;
        exception("[Error]: 赋值语句左右类型不匹配（隐式类型转换未支持）");
    }
    case T_SEQ:
        checkcmd_strict(c->d.SEQ.left, env);
        checkcmd_strict(c->d.SEQ.right, env);
        return;
    case T_IF:
    {
        // 任意表达式都能作为条件放入，就不管了
        // IF语句对了类型分析唯一的影响是作用域是叭🤔
        VarTypeEnv left_son;
        left_son.parent = env;
        checkcmd_strict(c->d.IF.left, &left_son);
        VarTypeEnv right_son;
        right_son.parent = env;
        checkcmd_strict(c->d.IF.right, &right_son);
        return;
    }
    case T_WHILE:
    {
        VarTypeEnv son;
        son.parent = env;
        checkcmd_strict(c->d.WHILE.body, &son);
        return;
    }
    case T_VARDECLARE:
        // 声明变量，只需要记录即可
        env->vartypes[c->d.VARDECLARE.var_name] = c->d.VARDECLARE.t;
        return;
    }
}