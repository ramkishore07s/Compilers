#include <iostream>
#include <vector>
#include <llvm/IR/Value.h>

using namespace std;

class CodeGenContext;

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

class Node {
public:
    virtual ~Node() {}
    virtual void debug() {}
    //virtual llvm::Value* codeGen(CodeGenContext& context);
};