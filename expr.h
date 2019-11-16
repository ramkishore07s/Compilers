#include "ast.h"

class NStatement : public Node {
};

class Expr : public NStatement {
};

class Constant : public Expr {
public:
};

class Nnum : public Constant {
public:
    int num;
    Nnum(int x) { num = x; }

    virtual void debug();
};

class NChar : public Constant {
public:
    string c;
    NChar(string x) : c(x) {}
};

class Nstr : public Constant {
public:
    string s;
    Nstr(string s) : s(s) {}
};

class NVariableName : public Constant {
public:
    string type;
    string name;
    vector<int> sizes;

    NVariableName(string name) : name(name) {}
    NVariableName(string name, vector<int> sizes_) : name(name), sizes(sizes_) {}
    NVariableName(string type, string name, vector<int> sizes_) : type(type), name(name), sizes(sizes_) {
    }

    virtual void debug();
};

class NbinOp : public Expr {
public:
    Expr &lhs;
    Expr &rhs;
    string op;

    NbinOp(Expr &lhs, Expr &rhs, string op) : lhs(lhs), rhs(rhs), op(op) {}

    virtual void debug();
};

class NassignOp : public Expr {
public:
    NVariableName lhs;
    Expr &rhs;

    NassignOp(NVariableName lhs, Expr &rhs) : lhs(lhs), rhs(rhs) {}

    virtual void debug();
};


class Nparam : public Node {
public:
    Expr &expr;
    Nparam(Expr &expr) : expr(expr) {}
    virtual void debug();
};

class Nparams : public Node {
public:
    Params params;
    Nparams() {}
    Nparams(Params params) : params(params) {}
    virtual void debug();
};

class NfunctionCall : public Expr {
public:
    string id;
    Nparams params;

    NfunctionCall(string id) : id(id) {}
    NfunctionCall(string id, Nparams params) : id(id), params(params) {}

    virtual void debug();
};

class NconditionalOp : public Expr {
public:
    Expr &condition;
    Expr &lval;
    Expr &rval;

    NconditionalOp(Expr &condition, Expr &lval, Expr &rval) : condition(condition), lval(lval), rval(rval) {}

    virtual void debug();
};