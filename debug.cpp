#include "node.h"
#include "parser.hpp"

using namespace std;

void NProgram::debug(Context &localContext, Context &globalContext) {
    cout << "\n";
    cout<< "Total top level nodes: " << functions.size() << "\n";
    for (size_t t=0; t < functions.size(); t++) {
        cout << "node: " << t << " :: ";
        functions[t]->debug(localContext, globalContext);
        cout << "\n";
    }
}

void NStatementBlock::debug(Context &localContext, Context &globalContext) {
    for (size_t t=0; t<statements.size(); t++) {
        statements[t]->debug(localContext, globalContext);
        cout << "\n";
    }
}
void Nnum::debug(Context &localContext, Context &globalContext) {
    cout << "Number: " << num;
}

void NChar::debug(Context &localContext, Context &globalContext) {
    cout << "Char: " << int(c);
}

void Narg::debug(Context &localContext, Context &globalContext) {
    cout << "Arg: "<< type << " " << varName.name << " " << varName.type;
}

void Nargs::debug(Context &localContext, Context &globalContext) {
    cout<<"Func Args: " <<this->arguments.size() << " ";
    for (size_t t=0; t<this->arguments.size(); t++)
        this->arguments[t]->debug(localContext, globalContext);
    cout<<"\n";
}

void Nparam::debug(Context &localContext, Context &globalContext) {
    cout <<"Func params: ";
    expr.debug(localContext, globalContext);
    cout <<" ";
}

void Nparams::debug(Context &localContext, Context &globalContext) {
    for (size_t t=0; t<params.size(); t++)
        params[t]->debug(localContext, globalContext);
}

void NFunctionDef::debug(Context &localContext_, Context &globalContext) {
    cout<< "Function Decl: " << "Type: " << this->type << " Name: " << this->id << "\n";
    args.debug(localContext, globalContext);
    cout << "\n";
    statementBlock.debug(localContext, globalContext);
    cout<<" ";

    cout<<"\n";
    cout<<"Local vars";
    for(std::map<string,Value*>::iterator iter = localContext.locals.begin(); iter != localContext.locals.end(); ++iter) {
        string k =  iter->first;
        cout << k;
    }
    cout <<"\n";
}

void NfunctionCall::debug(Context &localContext, Context &globalContext) {
    cout <<"Function call: " << id << " ";
    params.debug(localContext, globalContext);
    cout << " ";
}

void NVariableName::debug(Context &localContext, Context &globalContext) {
    cout<< "VarName :: " << this->type << " : " << this->name << " ";
    for (size_t t=0; t<sizes.size(); t++)
        cout << sizes[t] << " ";
}

void NVariableDecls::debug(Context &localContext, Context &globalContext) {
    cout << "VarDecl :: ";
    for (size_t t=0; t < varNames.size(); t++) {
        varNames[t]->debug(localContext, globalContext);
    }
    cout << " ";
}

void NbinOp::debug(Context &localContext, Context &globalContext) {
    lhs.debug(localContext, globalContext);
    cout <<" Op: " << op << " ";
    rhs.debug(localContext, globalContext);
    cout << " ";
}

void NassignOp::debug(Context &localContext, Context &globalContext) {
    cout << "AssignOp: " << lhs.name << " ";
    lhs.debug(localContext, globalContext);
    rhs.debug(localContext, globalContext);
    cout <<" ";
}

void NconditionalOp::debug(Context &localContext, Context &globalContext) {
    cout <<"Conditional Op: If: ";
    condition.debug(localContext, globalContext);
    cout << " Then: ";
    lval.debug(localContext, globalContext);
    cout << " Else: ";
    rval.debug(localContext, globalContext);
}

void NifBlock::debug(Context &localContext, Context &globalContext) {
    cout <<"If ";
    condition.debug(localContext, globalContext);
    cout << "then ";
    ifBlock.debug(localContext, globalContext);
    cout << "else ";
    elseBlock.debug(localContext, globalContext);
    cout <<" ";
}

void NwhileBlock::debug(Context &localContext, Context &globalContext) {
    cout <<"While ";
    condition.debug(localContext, globalContext);
    cout << " Do: \n";
    statementBlock.debug(localContext, globalContext);
}

void NforBlock::debug(Context &localContext, Context &globalContext) {
    cout<< "For ";
    expr1.debug(localContext, globalContext);
    cout << "; ";
    expr2.debug(localContext, globalContext);
    cout << "; ";
    expr3.debug(localContext, globalContext);
    cout << "\n";
    statementBlock.debug(localContext, globalContext);
}

void Nbreak::debug(Context &localContext, Context &globalContext) {
    cout << "break; ";
}

void Ncontinue::debug(Context &localContext, Context &globalContext) {
    cout << "continue; ";
}

void Nreturn::debug(Context &localContext, Context &globalContext) {
    cout <<"Return ";
    expr.debug(localContext, globalContext);
    cout <<" ";
}