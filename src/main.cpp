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
    printf("\n\n");


    VarTypeEnv env1;
    env1.parent = nullptr;
    try
    {
        checkcmd_strict(root, &env1);
        printf("类型检查成功\n严格类型检查后的AST树：\n");

        print_cmd(root); // 严格类型检查的AST解析结果
        printf("\n\n");
    } catch (const std::exception &e)
    {
        printf("严格类型检查失败：%s\n\n", e.what());
    }


    VarTypeEnv env2;
    env2.parent = nullptr;
    try
    {
        checkcmd_implicit(root, &env2);
        printf("类型检查成功\n隐式转换显式化后的AST树：\n");

        print_cmd(root); // 加入隐式转换的AST解析结果
        printf("\n");
    } catch (const std::exception &e)
    {
        printf("隐式转换显式化失败：%s\n", e.what());
    }
    return 0;
}
