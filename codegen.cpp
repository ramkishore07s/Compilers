#include "node.h"

using namespace std;

Value *nullVal;

Value* NProgram::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    cout << "\n";
    cout<< "Total top level nodes: " << functions.size() << "\n";
    for (size_t t=0; t < functions.size(); t++) {
        cout << "node: " << t << " :: ";
        functions[t]->codeGen(localContext, globalContext, Builder);
        cout << "\n";
    }
    return nullVal;
}

Value* NStatementBlock::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    for (size_t t=0; t<statements.size(); t++) {
        statements[t]->codeGen(localContext, globalContext, Builder);
        cout << "\n";
    }
    return nullVal;
}
Value* Nnum::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    cout << "Number: " << num;
    return Builder.getInt32(num);
}

Value* NChar::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    cout << "Char: " << int(c);
    return Builder.getInt32(int(c));
}
Value* Narg::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    cout << "Arg: "<< type << " " << varName.name << " " << varName.type;
    return nullVal;
}

Value* Nargs::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    cout<<"Func Args: " <<this->arguments.size() << " ";
    for (size_t t=0; t<this->arguments.size(); t++)
        this->arguments[t]->codeGen(localContext, globalContext, Builder);
    cout<<"\n";
    return nullVal;
}

Value* Nparam::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    cout <<"Func param: ";
    cout <<" ";
    return expr.codeGen(localContext, globalContext, Builder);
}

Value* Nparams::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    for (size_t t=0; t<params.size(); t++)
        params[t]->codeGen(localContext, globalContext, Builder);
    return nullVal;
}

Value* NFunctionDef::codeGen(Context &localContext_, Context &globalContext, IRBuilder<> &Builder) {
    std::vector<string> argNames;
    std::vector<Type*> argTypes;

    for (size_t t=0; t<args.arguments.size(); t++) {
        argNames.push_back(args.arguments[t]->varName.name);
        //TODO: assign type according to var type
        argTypes.push_back(Type::getInt32Ty(llvmContext));
    }

    funcType = FunctionType::get(Builder.getInt32Ty(), argTypes, false);
    fooFunc = Function::Create(funcType, GlobalValue::InternalLinkage, id, globalContext.module);

    Function::arg_iterator AI, AE; unsigned i = 0;
    for (AI = fooFunc->arg_begin(), AE = fooFunc->arg_end(); AI != AE; ++AI, ++i)
        AI->setName(argNames[i]);

    BasicBlock *entry = BasicBlock::Create(llvmContext, "entry", fooFunc);
    Builder.SetInsertPoint(entry);
    blocks.push_back(entry);

    statementBlock.codeGen(localContext, globalContext, Builder);

    verifyFunction(*fooFunc);
    return nullVal;
}

Value* NfunctionCall::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    cout <<"Function call: " << id << " ";
    cout << " ";
    Function *function = globalContext.module->getFunction(id);
    if (function == NULL) {
        std::cerr << "no such function " << id << std::endl;
    }
    std::vector<Value*> args;
    Params::const_iterator it;
    for (it=params.params.begin(); it!=params.params.end(); it++) {
        args.push_back((*it)->codeGen(localContext, globalContext, Builder));
    }
    return Builder.CreateCall(function, args);
}

Value* NVariableName::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    cout<< "VarName :: " << this->type << " : " << this->name << " ";
    for (size_t t=0; t<sizes.size(); t++)
        cout << sizes[t] << " ";

    return Builder.CreateLoad(localContext.locals[name]);
}

Value* NVariableDecls::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    cout << "VarDecl :: ";
    for (size_t t=0; t < varNames.size(); t++) {
        //varNames[t]->codeGen is only called for variable access
        localContext.locals[varNames[t]->name] = Builder.CreateAlloca(Builder.getInt32Ty(), Builder.getInt32(0), varNames[t]->name);
        localContext.localtypes[varNames[t]->name] = varNames[t];
    }
    cout << " ";
    return nullVal;
}

Value* NVariableAccess::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    cout << "Var access: " << name;
    return Builder.CreateLoad(localContext.locals[name]);
}


Value* NbinOp::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    lhs.codeGen(localContext, globalContext, Builder);
    cout <<" Op: " << op << " ";
    rhs.codeGen(localContext, globalContext, Builder);
    cout << " ";

    return Builder.CreateAdd(lhs.codeGen(localContext, globalContext, Builder),
                             rhs.codeGen(localContext, globalContext, Builder),
                             "op");
}

Value* NassignOp::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    cout << "AssignOp: " << lhs.name << " ";
    cout <<" ";

    return Builder.CreateStore(rhs.codeGen(localContext, globalContext, Builder),
                               lhs.codeGen(localContext, globalContext, Builder),
                               false);
}

Value* NconditionalOp::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    cout <<"Conditional Op: If: ";
    condition.codeGen(localContext, globalContext, Builder);
    cout << " Then: ";
    lval.codeGen(localContext, globalContext, Builder);
    cout << " Else: ";
    rval.codeGen(localContext, globalContext, Builder);
    return nullVal;
}

Value* NifBlock::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    cout <<"If ";
    condition.codeGen(localContext, globalContext, Builder);
    cout << "then ";
    ifBlock.codeGen(localContext, globalContext, Builder);
    cout << "else ";
    elseBlock.codeGen(localContext, globalContext, Builder);
    cout <<" ";
    return nullVal;
}

Value* NwhileBlock::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    cout <<"While ";
    condition.codeGen(localContext, globalContext, Builder);
    cout << " Do: \n";
    statementBlock.codeGen(localContext, globalContext, Builder);
    return nullVal;
}

Value* NforBlock::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    cout<< "For ";
    expr1.codeGen(localContext, globalContext, Builder);
    cout << "; ";
    expr2.codeGen(localContext, globalContext, Builder);
    cout << "; ";
    expr3.codeGen(localContext, globalContext, Builder);
    cout << "\n";
    statementBlock.codeGen(localContext, globalContext, Builder);
    return nullVal;
}

Value* Nbreak::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    cout << "break; ";
    return nullVal;
}

Value* Ncontinue::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    cout << "continue; ";
    return nullVal;
}

Value* Nreturn::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    cout <<"Return ";
    ;
    cout <<" ";

    Builder.CreateRet(expr.codeGen(localContext, globalContext, Builder));
    return nullVal;
}