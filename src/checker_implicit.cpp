#include <stdio.h>
#include <stdlib.h>
#include "lang.h"
#include "checker.h"

// 隐式转换的优先级： long long > long > int > short

Expr *implicit_typeconv(VarType to_type, Expr *expr)
{
    Expr *res = new Expr;
    res->t = T_TYPECONV;
    res->d.TYPECONV.t = to_type;
    res->d.TYPECONV.right = expr;
    return res;
}

VarType lookup_vartype(VarTypeEnv *env, char *name)
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
            return lookup_vartype(env->parent, name);
        }
        else // 找到全局作用域还没有获取到类型，则不存在
        {
            exception("[Error]: 使用了未声明的变量");
        }
    }
    return VarType{}; // 永远不会到达这里
}

VarType check_binop_implicit(struct Expr *e, struct VarTypeEnv *env)
{
    Expr *left = e->d.BINOP.left;
    Expr *right = e->d.BINOP.right;
    BinOpType op = e->d.BINOP.op;
    VarType left_type = checkexpr_implicit(left, env);
    VarType right_type = checkexpr_implicit(right, env);
    switch (op)
    {
    case T_PLUS:
    case T_MINUS:
    case T_MUL:
    case T_DIV:
    case T_MOD:
        if (left_type.tag == T_PTR || right_type.tag == T_PTR)
        {
            exception("[Error]: 指针类型不能参与加减乘除模（未定义数组相关）");
        }
        else // 改过后默认的T_BASIC枚举类型的顺序即满足优先级
        {
            if (left_type.tbasic < right_type.tbasic) // 优先提升到右操作数的类型
            {
                e->d.BINOP.left = implicit_typeconv(right_type, e->d.BINOP.left); // 修改挂在原AST上的左表达式
                return right_type;
            }
            else if (left_type.tbasic > right_type.tbasic)
            {
                e->d.BINOP.right = implicit_typeconv(left_type, e->d.BINOP.right);
                return left_type;
            }
            else
            {
                return left_type;
            }
        }
    case T_AND:
    case T_OR:
        if (left_type.tag == T_PTR || right_type.tag == T_PTR)
        {
            exception("[Error]: 指针类型不参与布尔类运算");
        }
        else // 都要默认转为int / 'bool'类 / 并且不用提醒，语义上是非零为真
        {
            // if (left_type.tbasic != T_INT)
            //     e->d.BINOP.left = implicit_typeconv(new_VarType_BASIC(T_INT), e->d.BINOP.left); // 左侧隐式转换
            // if (right_type.tbasic != T_INT)
            //     e->d.BINOP.right = implicit_typeconv(new_VarType_BASIC(T_INT), e->d.BINOP.right); // 左侧隐式转换
            return new_VarType_BASIC(T_INT);
        }
    case T_LT:
    case T_GT:
    case T_LE:
    case T_GE:
    case T_EQ:
    case T_NE:
        if (left_type.tag == T_PTR || right_type.tag == T_PTR)
        {
            exception("[Error]: 指针类型不参与比较运算");
        }
        else // 改过后默认的T_BASIC枚举类型的顺序即满足优先级
        {
            if (left_type.tbasic < right_type.tbasic)       // 优先提升到右操作数的类型
                e->d.BINOP.left = implicit_typeconv(right_type, e->d.BINOP.left); // 修改挂在原AST上的左表达式
            if (left_type.tbasic > right_type.tbasic)
                e->d.BINOP.right = implicit_typeconv(left_type, e->d.BINOP.right);
            return new_VarType_BASIC(T_INT); // 总之都返回INT -> 作为Bool的代表
        }
    }
    return VarType{}; // 永远不会到达这里
}

VarType check_unop_implicit(struct Expr *e, struct VarTypeEnv *env)
{
    // Expr *right = e->d.UNOP.right;
    UnOpType op = e->d.UNOP.op;
    VarType expr_type = checkexpr_implicit(e->d.UNOP.right, env);
    if (expr_type.tag == T_PTR)
    {
        exception("[Error]: 指针类型不支持一元运算符");
    }
    else
    {
        switch (op)
        {
        case T_NEG:
            return expr_type;
        case T_NOT:
            // if (expr_type.tbasic != T_INT)
                // e->d.UNOP.right = implicit_typeconv(new_VarType_BASIC(T_INT), e->d.UNOP.right); // 隐式转换为INT
            return new_VarType_BASIC(T_INT); // 最后返回INT / bool 类型
        }
    }
    return VarType{}; // 永远不会到达这里
}

VarType checkexpr_implicit(struct Expr *e, struct VarTypeEnv *env)
{
    switch (e->t)
    {
    case T_CONST:
        if (e->d.CONST.value <= INT32_MAX)
            return new_VarType_BASIC(T_INT);
        else
            return new_VarType_BASIC(T_LONGLONG);
    case T_VAR:
        return lookup_vartype(env, e->d.VAR.name); // 支持父域查找
    case T_BINOP:
        return check_binop_implicit(e, env);
    case T_UNOP:
        return check_unop_implicit(e, env);
    case T_DEREF:
    {
        VarType t = checkexpr_implicit(e->d.DEREF.right, env);
        switch (t.tag) // 根据tag来跟踪union中到底是谁有效
        {
        case T_PTR:
            return *t.tptr; // 交出指针包裹的类型
        case T_BASIC:
            exception("[Error]: 不能解引用非指针类型");
        }
    }
    case T_ADDROF:
    {
        VarType t = checkexpr_implicit(e->d.ADDROF.right, env);
        switch (e->d.ADDROF.right->t) // 表达式，
        {
        case T_VAR:
        case T_DEREF:
            return new_VarType_PTR(t); // 有且仅有以上两种左值表达式？
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
            exception("[Error]: 不支持的类型转换");
        }
    }
    return VarType{}; // 永远不会到达这里
}

void checkcmd_implicit(struct Cmd *c, struct VarTypeEnv *env)
{
    switch (c->t)
    {
    case T_SKIP:
        return; // 无事发生直接退出，出问题的选择是直接退出程序，“编译失败”
    case T_ASGN:
    {
        char *left = c->d.ASGN.left;
        Expr *right = c->d.ASGN.right;
        VarType left_type = lookup_vartype(env, left);
        VarType right_type = checkexpr_implicit(right, env);
        if (!VarTypeCmp(left_type, right_type)) // 左右类型不匹配才不OK
        {
            if (left_type.tag == T_PTR || right_type.tag == T_PTR) // 类型不同且存在指针
            {
                exception("[Error]: 不支持指针类型的隐式转换");
            }
            else
            {                                             // 赋值语句始终需要右侧表达式转换到左侧
                if (left_type.tbasic < right_type.tbasic) // 反优先级的
                {
                    printf("[Warning]: 赋值语句存在类型 [");
                    print_type(right_type);
                    printf("] 向类型 [");
                    print_type(left_type);
                    printf("] 的隐式转换反优先级，可能损失数据信息\n");
                } // 其余就是顺优先级无所谓
                c->d.ASGN.right = implicit_typeconv(left_type, c->d.ASGN.right);
            }
        }
        return; // 左右类型匹配，则OK
    }
    case T_ASGNDREF:
    {
        Expr deref;
        deref.t = T_DEREF;
        deref.d.DEREF.right = c->d.ASGNDREF.left;
        VarType left_type = checkexpr_implicit(&deref, env); // 帮忙解引用了
        VarType right_type = checkexpr_implicit(c->d.ASGNDREF.right, env);
        if (!VarTypeCmp(left_type, right_type))
        {
            if (left_type.tag == T_PTR || right_type.tag == T_PTR) // 类型不同且存在指针
            {
                exception("[Error]: 不支持指针类型的隐式转换");
            }
            else
            {                                             // 赋值语句始终需要右侧表达式转换到左侧
                if (left_type.tbasic < right_type.tbasic) // 反优先级的
                {
                    printf("[Warning]: 赋值语句存在类型 [");
                    print_type(right_type);
                    printf("] 向类型 [");
                    print_type(left_type);
                    printf("] 的隐式转换反优先级，可能损失数据信息\n");
                } // 其余就是顺优先级无所谓
                c->d.ASGNDREF.right = implicit_typeconv(left_type, c->d.ASGNDREF.right);
            }
        }
        return;
    }
    case T_SEQ:
        checkcmd_implicit(c->d.SEQ.left, env);
        checkcmd_implicit(c->d.SEQ.right, env);
        return;
    case T_IF:
    {
        checkexpr_implicit(c->d.IF.cond, env);
        VarTypeEnv left_son;
        left_son.parent = env;
        checkcmd_implicit(c->d.IF.left, &left_son);
        VarTypeEnv right_son;
        right_son.parent = env;
        checkcmd_implicit(c->d.IF.right, &right_son);
        return;
    }
    case T_WHILE:
    {
        checkexpr_implicit(c->d.WHILE.cond, env);
        VarTypeEnv son;
        son.parent = env;
        checkcmd_implicit(c->d.WHILE.body, &son);
        return;
    }
    case T_VARDECLARE:
        // 声明变量，只需要记录即可
        env->vartypes[c->d.VARDECLARE.var_name] = c->d.VARDECLARE.t;
        return;
    }
}