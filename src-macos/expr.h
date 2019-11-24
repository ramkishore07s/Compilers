#include "ast.h"

class NStatement : public Node {
};

class Expr : public NStatement {
};

class Nconstant : public Expr {
public:
};

class Nnum : public Nconstant {
public:
    int num;
    Nnum(int x) { num = x; }

    virtual void debug(Context &localContext, Context &globalContext);
    virtual Value* codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder);
};

class NChar : public Nconstant {
public:
    char c;
    NChar(string x) { c = (char) x[1]; }

    virtual void debug(Context &localContext, Context &globalContext);
    virtual Value* codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder);
};

class NBool : public Nconstant {
public:
    string value;

    NBool(string x) : value(x) {}
    virtual Value* codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder);
};

class Nstr : public Nconstant {
public:
    string s;
    Nstr(string s) : s(s) {}
};

class NVariableName : public Nconstant {
public:
    string type;
    string name;
    bool isarg;
    vector<int> sizes;

    NVariableName(string name) : name(name) { isarg = false; }
    NVariableName(string name, vector<int> sizes_) : name(name), sizes(sizes_) { isarg = false; }
    NVariableName(string type, string name, vector<int> sizes_) : type(type), name(name), sizes(sizes_) { isarg = false; }

    virtual void debug(Context &localContext, Context &globalContext);
    virtual Value* codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder);
    Value* codeGen2(Context &localContext, Context &globalContext, IRBuilder<> &Builder);
};

class NArrayAccess : public Nconstant {
public:
    string type;
    string name;
    Exprs exprs;

    NArrayAccess(string name) : name(name) {}
    NArrayAccess(string name, Exprs exprs) : name(name), exprs(exprs) {}

    virtual Value* codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder);
    virtual void debug(Context &localContext, Context &globalContext);

    Value* codeGen2(Context &localContext, Context &globalContext, IRBuilder<> &Builder);
};

class NbinOp : public Expr {
public:
    Expr &lhs;
    Expr &rhs;
    string op;

    NbinOp(Expr &lhs, Expr &rhs, string op) : lhs(lhs), rhs(rhs), op(op) {}

    virtual void debug(Context &localContext, Context &globalContext);
    virtual Value* codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder);
};


class Nparam : public Node {
public:
    Expr &expr;
    Nparam(Expr &expr) : expr(expr) {}

    virtual void debug(Context &localContext, Context &globalContext);
    virtual Value* codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder);
};

class Nparams : public Node {
public:
    Params params;
    Nparams() {}
    Nparams(Params params) : params(params) {}

    virtual void debug(Context &localContext, Context &globalContext);
    virtual Value* codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder);
};

class NfunctionCall : public Expr {
public:
    string id;
    Nparams params;

    NfunctionCall(string id) : id(id) {}
    NfunctionCall(string id, Nparams params) : id(id), params(params) {}

    virtual void debug(Context &localContext, Context &globalContext);
    virtual Value* codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder);
};

class NconditionalOp : public Expr {
public:
    Expr &condition;
    Expr &lval;
    Expr &rval;

    NconditionalOp(Expr &condition, Expr &lval, Expr &rval) : condition(condition), lval(lval), rval(rval) {}

    virtual void debug(Context &localContext, Context &globalContext);
    virtual Value* codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder);
};

class NnotOp : public Expr {
public:
    Expr &expr;

    NnotOp(Expr &expr) : expr(expr) {}

    virtual Value* codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder);
};