#include "node.h"
#include "parser.hpp"

extern int yyparse();
extern NProgram *programNode;

int main(int argc, char **argv)
{
    static IRBuilder<> Builder(llvmContext);
    Context local, global;

    global.module = new Module("main", llvmContext);

    yyparse();
    printf("Parsing Over\n");
    programNode->debug(local, global);

    programNode->codeGen(local, global, Builder);

    global.module->print(llvm::errs(), nullptr);
    return 0;
}