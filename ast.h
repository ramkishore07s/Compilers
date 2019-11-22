#include <iostream>
#include <vector>
#include <llvm/IR/Value.h>
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"

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


class Context {
public:
    std::map<std::string, Value*> locals;
    std::map<std::string, NVariableName*> localtypes;
    Module *module;
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
