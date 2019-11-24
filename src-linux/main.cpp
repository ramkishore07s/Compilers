#include "node.h"
#include "parser.hpp"

extern int yyparse();
extern NProgram *programNode;

int main(int argc, char **argv)
{
    static IRBuilder<> Builder(llvmContext);
    Context local, global;

    global.module = new Module("Compilers", llvmContext);
    linkIoFunctions(global.module, Builder);
    yyparse();
    printf("Parsing Over\n");
    programNode->debug(local, global);

    programNode->codeGen(local, global, Builder);

    //global.module->print(llvm::errs(), nullptr);

    //Try adding optimisations later?
    //PassManager pm;
    //pm.add(createPrintModulePass(&outs()));
    //pm.run(*global.module);

    legacy::PassManager pm;
    pm.add(createPrintModulePass(outs()));
    pm.run(*global.module);

    Function *mainFunction = global.module->getFunction("main");
    if (mainFunction == NULL) {
        std::cerr << "no main function " << std::endl;
    } else {
        LLVMInitializeNativeAsmPrinter();
        LLVMInitializeNativeAsmParser();
        InitializeNativeTarget();
        ExecutionEngine *ee = EngineBuilder( std::unique_ptr<Module>(global.module) ).create();
        cerr << "Engine created\n";
        ee->finalizeObject();
        cerr << "Object finalised\n";
        std::vector<GenericValue> noargs;
        ee->runFunction(mainFunction, noargs);
        cerr << "Code was run.\n";
    }


    return 0;
}