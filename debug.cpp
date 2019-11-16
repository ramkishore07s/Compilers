#include "node.h"
#include "parser.hpp"

using namespace std;

void NProgram::debug() {
    cout << "\n";
    cout<< "Total top level nodes: " << functions.size() << "\n";
    for (size_t t=0; t < functions.size(); t++) {
        cout << "node: " << t << " :: ";
        functions[t]->debug();
        cout << "\n";
    }
}

void NStatementBlock::debug() {
    for (size_t t=0; t<statements.size(); t++) {
        statements[t]->debug();
        cout << "\n";
    }
}
void Nnum::debug() {
    cout << "Number: " << num;
}

void Narg::debug() {
    cout << "Arg: "<< type << " " << varName.name << " " << varName.type;
}

void Nargs::debug() {
    cout<<"Func Args: " <<this->arguments.size() << " ";
    for (size_t t=0; t<this->arguments.size(); t++)
        this->arguments[t]->debug();
    cout<<"\n";
}

void Nparam::debug() {
    cout <<"Func params: ";
    expr.debug();
    cout <<" ";
}

void Nparams::debug() {
    for (size_t t=0; t<params.size(); t++)
        params[t]->debug();
}

void NFunctionDef::debug() {
    cout<< "Function Decl: " << "Type: " << this->type << " Name: " << this->id << "\n";
    args.debug();
    cout << "\n";
    statementBlock.debug();
    cout<<" ";
}

void NfunctionCall::debug() {
    cout <<"Function call: " << id << " ";
    params.debug();
    cout << " ";
}

void NVariableName::debug() {
    cout<< "VarName :: " << this->type << " : " << this->name << " ";
    for (size_t t=0; t<sizes.size(); t++)
        cout << sizes[t] << " ";
}

void NVariableDecls::debug() {
    cout << "VarDecl :: ";
    for (size_t t=0; t < varNames.size(); t++)
        varNames[t]->debug();
    cout << " ";
}

void NbinOp::debug() {
    lhs.debug();
    cout <<" Op: " << op << " ";
    rhs.debug();
    cout << " ";
}

void NassignOp::debug() {
    cout << "AssignOp: " << lhs.name << " ";
    lhs.debug();
    rhs.debug();
    cout <<" ";
}

void NconditionalOp::debug() {
    cout <<"Conditional Op: If: ";
    condition.debug();
    cout << " Then: ";
    lval.debug();
    cout << " Else: ";
    rval.debug();
}

void NifBlock::debug() {
    cout <<"If ";
    condition.debug();
    cout << "then ";
    ifBlock.debug();
    cout << "else ";
    elseBlock.debug();
    cout <<" ";
}

void NwhileBlock::debug() {
    cout <<"While ";
    condition.debug();
    cout << " Do: \n";
    statementBlock.debug();
}

void NforBlock::debug() {
    cout<< "For ";
    expr1.debug();
    cout << "; ";
    expr2.debug();
    cout << "; ";
    expr3.debug();
    cout << "\n";
    statementBlock.debug();
}

void Nreturn::debug() {
    cout <<"Return ";
    expr.debug();
    cout <<" ";
}