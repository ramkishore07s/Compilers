#include "expr.h"

using namespace std;

class NStatementBlock : public Node {
public:
    StatementBlock statements;

    virtual void debug();
};

class NVariableDecls : public NStatement {
public:
    VarNames varNames;
    NVariableDecls(string type, VarNames names) {
        for (size_t t=0; t<names.size(); t++)
            varNames.push_back(new NVariableName(type, names[t]->name, names[t]->sizes));
    }

    virtual void debug();
};


class NifBlock : public NStatement {
public:
    Expr &condition;
    NStatementBlock ifBlock;
    NStatementBlock elseBlock;

    NifBlock(Expr &condition, NStatementBlock ifBlock) : condition(condition), ifBlock(ifBlock) {}
    NifBlock(Expr &condition, NStatementBlock ifBlock, NStatementBlock elseBlock) : condition(condition), ifBlock(ifBlock), elseBlock(elseBlock)  {}

    virtual void debug();
};

class NwhileBlock : public NStatement {
public:
    Expr &condition;
    NStatementBlock statementBlock;

    NwhileBlock(Expr &condition, NStatementBlock statementBlock) : condition(condition), statementBlock(statementBlock) {}

    virtual void debug();
};

class NforBlock : public NStatement {
public:
    Expr &expr1;
    Expr &expr2;
    Expr &expr3;
    NStatementBlock statementBlock;

    NforBlock(Expr &expr1, Expr &expr2, Expr &expr3, NStatementBlock statementBlock) : expr1(expr1), expr2(expr2), expr3(expr3), statementBlock(statementBlock) {}

    virtual void debug();
};

class Nreturn : public NStatement {
public:
    Expr &expr;

    Nreturn(Expr &expr) : expr(expr) {}

    virtual void debug();
};

class Nbreak : public NStatement {
public:
    virtual void debug();
};

class Ncontinue : public NStatement {
public:
    virtual void debug();
};
