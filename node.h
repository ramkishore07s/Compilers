#include "stmt.h"

class NProgram : public Node {
public:
    FunctionDefs functions;
    NProgram() {}
    virtual void debug();
};

class Narg : public Node {
public:
    string type;
    NVariableName varName;
    Narg(string type, NVariableName varName) : type(type), varName(varName) {}

    virtual void debug();
};

class Nargs : public Node {
public:
    Args arguments;
    Nargs() {}
    virtual void debug();
};

class NFunctionDef : public Node {
public:
    string type;
    string id;
    Nargs args;
    NStatementBlock statementBlock;

    NFunctionDef(string type, string id, NStatementBlock stmtBlock) : type(type), id(id), statementBlock(stmtBlock) {}
    NFunctionDef(string type, string id, Nargs args2, NStatementBlock stmtBlock) : type(type), id(id), args(args2), statementBlock(stmtBlock) {}

    virtual void debug();
};






