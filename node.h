#include "stmt.h"

class Narg : public Node {
public:
    string type;
    NVariableName varName;
    Narg(string type, NVariableName varName) : type(type), varName(varName) {
    }

    virtual void debug(Context &localContext, Context &globalContext);
    virtual Value* codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder);
};

class Nargs : public Node {
public:
    Args arguments;
    Nargs() {}

    virtual void debug(Context &localContext, Context &globalContext);
    virtual Value* codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder);
};

class NFunctionDef : public Node {
public:
    string type;
    string id;
    Nargs args;
    NStatementBlock &statementBlock;
    Context localContext;

    FunctionType *funcType;
    Function *fooFunc;

    Blocks blocks;

    NFunctionDef(string type, string id, NStatementBlock &stmtBlock) : type(type), id(id), statementBlock(stmtBlock) {}
    NFunctionDef(string type, string id, Nargs args2, NStatementBlock &stmtBlock) : type(type), id(id), args(args2), statementBlock(stmtBlock) {}


    virtual void debug(Context &localContext, Context &globalContext);
    virtual Value* codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder);
};

class NProgram : public Node {
public:
    FunctionDefs functions;
    NProgram() {}

    virtual void debug(Context &localContext, Context &globalContext);
    virtual Value* codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder);

    // for f in functions:
    //      f.genCode()
    //
    //
};







