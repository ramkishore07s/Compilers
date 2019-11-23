#include "expr.h"

using namespace std;

class NStatementBlock : public Node {
public:
    StatementBlock statements;

    virtual void debug(Context &localContext, Context &globalContext);
    virtual Value* codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder);
};

class NVariableDecls : public NStatement {
public:
    VarNames varNames;
    NVariableDecls(string type, VarNames names) {
        for (size_t t=0; t<names.size(); t++)
            varNames.push_back(new NVariableName(type, names[t]->name, names[t]->sizes));
    }

    virtual void debug(Context &localContext, Context &globalContext);
    virtual Value* codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder);
};

class NassignOp : public NStatement {
public:
    NVariableName lhs;
    Expr &rhs;

    NassignOp(NVariableName lhs, Expr &rhs) :lhs(lhs), rhs(rhs) {}

    virtual void debug(Context &localContext, Context &globalContext);
    virtual Value* codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder);
};

class NifBlock : public NStatement {
public:
    Expr &condition;
    NStatementBlock ifBlock;
    NStatementBlock elseBlock;

    NifBlock(Expr &condition, NStatementBlock ifBlock) : condition(condition), ifBlock(ifBlock) {}
    NifBlock(Expr &condition, NStatementBlock ifBlock, NStatementBlock elseBlock) : condition(condition), ifBlock(ifBlock), elseBlock(elseBlock)  {}

    BasicBlock* ThenBlock;
    BasicBlock* ElseBlock;
    BasicBlock* PhiBlock;

    virtual void debug(Context &localContext, Context &globalContext);
    virtual Value* codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder);
};

class NwhileBlock : public NStatement {
public:
    Expr &condition;
    NStatementBlock statementBlock;

    NwhileBlock(Expr &condition, NStatementBlock statementBlock) : condition(condition), statementBlock(statementBlock) {}

    BasicBlock *CondBlock;
    BasicBlock *LoopBlock;
    BasicBlock *PhiBlock;

    virtual void debug(Context &localContext, Context &globalContext);
    virtual Value* codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder);
};

class NforBlock : public NStatement {
public:
    NStatement &expr1;
    NStatement &expr2;
    NStatement &expr3;
    NStatementBlock statementBlock;

    NforBlock(NStatement &expr1, NStatement &expr2, NStatement &expr3, NStatementBlock statementBlock) : expr1(expr1), expr2(expr2), expr3(expr3), statementBlock(statementBlock) {}

    BasicBlock *CondBlock;
    BasicBlock *LoopBlock;
    BasicBlock *PhiBlock;

    virtual void debug(Context &localContext, Context &globalContext);
    virtual Value* codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder);
};

class Nreturn : public NStatement {
public:
    Expr &expr;

    Nreturn(Expr &expr) : expr(expr) {}

    virtual void debug(Context &localContext, Context &globalContext);
    virtual Value* codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder);
};



class Nbreak : public NStatement {
public:
    BasicBlock *AfterBreak;

    virtual void debug(Context &localContext, Context &globalContext);
    virtual Value* codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder);
};

class Ncontinue : public NStatement {
public:
    BasicBlock* AfterContinue;
    virtual void debug(Context &localContext, Context &globalContext);
    virtual Value* codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder);
};
