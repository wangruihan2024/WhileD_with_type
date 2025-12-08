#include <stdio.h>
#include "lang.h"
#include "lexer.h"
#include "parser.h"
#include "checker.h"

extern struct Cmd *root;
extern "C" int yyparse();

int main(int argc, char **argv)
{
    yyin = stdin;
    yyparse();
    fclose(stdin);

    printf("初始的AST解析结果：\n");
    print_cmd(root); // 初始的AST解析结果
    printf("\n");

    // checkcmd(root, &env);
    // 这倒是如果使用了checkcmd_conv则root会被修改
    // 如果使用了checkcmd则可能直接报错炸掉啊😆
    // and如果需要同时展示，那么应当开两个新的环境

    VarTypeEnv env;
    checkcmd_implicit(root, &env); // 笑死，发生了超级无敌神秘bug，把conv和普通的名字写混了（
    printf("类型检查成功\n隐式转换显式化后的AST树：\n");

    print_cmd(root); // 初始的AST解析结果
    printf("\n");
    return 0;
}
