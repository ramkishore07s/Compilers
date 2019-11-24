#include <iostream>
#include <stack>
#include <vector>
#include <typeinfo>

#include <llvm/IR/Value.h>
#include <llvm/IR/Constants.h>
#include "llvm/IR/Verifier.h"
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Attributes.h>
#include <llvm-c/Target.h>
#include <llvm/Bitcode/BitstreamReader.h>
#include <llvm/Bitcode/BitstreamWriter.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/Support/raw_ostream.h>

using namespace std;
using namespace llvm;
class BBlock;

class NProgram;
class NFunctionDef;
class Narg;
class Nargs;
class Nparam;
class Nparams;
class NStatement;

typedef std::vector<NStatement*> StatementBlock;
typedef std::vector<NFunctionDef*> FunctionDefs;
typedef std::vector<Narg*> Args;
typedef std::vector<Nparam*> Params;

class NVariableDecl;
class NVariableDecls;
class NVariableName;
typedef std::vector<NVariableName*> VarNames;
typedef std::vector<NVariableDecl*> VariableDecls;

class Expr;
class Constant;
class Nnum;
class NChar;
class Nstr;

typedef std::vector<Expr*> Exprs;


class Context {
public:
    std::map<std::string, Value*> locals;
    std::map<std::string, NVariableName*> localtypes;
    std::stack<BasicBlock*> loopblocks;
    std::stack<BasicBlock*> endloopblocks;
    Module *module;
    Function *function;
};

typedef std::vector<BasicBlock*> Blocks;

class Node {
public:
    virtual ~Node() {}
    virtual Value* codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
        return NULL;
    }
    virtual void debug(Context &localContext, Context &globalContext) {}
};

static LLVMContext llvmContext;

void linkIoFunctions(Module* module, IRBuilder<> &Builder);
