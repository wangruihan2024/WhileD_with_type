#include <stdio.h>
#include "lang.h"
#include "lexer.h"
#include "parser.h"
#include "checker.h"

extern struct Cmd *root;
int yyparse();

VarTypeEnv env;

int main(int argc, char **argv)
{
    yyin = stdin;
    yyparse();
    fclose(stdin);
    print_cmd(root);
    printf("\n");
    checkcmd(root, &env);
    printf("类型检查成功\n");
    return 0;
}
