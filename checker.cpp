#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include "lang.h"

// checker 即类型检查器，基础是类型表（支持作用域）

struct VarTypeEnv
{
    std::map<char *, VarType> vartypes;
    struct VarTypeEnv *parent; // 父作用域，全局作用域则为空
};
