#include <stdio.h>
#include "lang.h"
#include "lexer.h"
#include "parser.h"

extern struct Cmd * root;
int yyparse();

int main(int argc, char **argv) {
    yyin = stdin;
    yyparse();
    fclose(stdin);
    print_cmd(root);
}
