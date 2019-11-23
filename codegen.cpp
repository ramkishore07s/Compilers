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
    return (Value*) ConstantInt::get(Builder.getInt32Ty(), int(num), false);
    //return num;
}

Value* NChar::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    cout << "Char: " << int(c);
    return ConstantInt::get(Builder.getInt32Ty(), int(c), false);
}

Value* NVariableName::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    cerr<< "VarName :: " << this->type << " : " << this->name << " " << localContext.localtypes[name]->isarg << "; ";
    for (size_t t=0; t<sizes.size(); t++)
        cout << sizes[t] << " ";

    if (localContext.localtypes[name]->isarg) {
        // Arguments cannot be loaded since they are not stored anywhere!
        // All arguments are allocated memory initially
        return localContext.locals[name];
    } else {
        return Builder.CreateLoad(localContext.locals[name]);
    }
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
        args.arguments[t]->varName.isarg = true;
        argNames.push_back(args.arguments[t]->varName.name);
        //TODO: assign type according to var type
        argTypes.push_back(Builder.getInt32Ty());
    }

    funcType = FunctionType::get(Builder.getInt32Ty(), argTypes, false);
    fooFunc = Function::Create(funcType, GlobalValue::InternalLinkage, id, globalContext.module);

    localContext.function = fooFunc;

    Function::arg_iterator AI, AE; unsigned i = 0;
    for (AI = fooFunc->arg_begin(), AE = fooFunc->arg_end(); AI != AE; ++AI, ++i) {
        cout << argNames[i] << "\n";
        AI->setName(argNames[i]);
    }

    BasicBlock *entry = BasicBlock::Create(llvmContext, "entry", fooFunc);
    Builder.SetInsertPoint(entry);

    // Allocate memory for variables
    NVariableDecls *varDecl;
    NassignOp *assignOp;
    VarNames *varNames;

    Context argContext;

    size_t t = 0;
    for (AI = fooFunc->arg_begin(), AE = fooFunc->arg_end(); AI != AE; ++AI, ++t) {
        varNames = new VarNames();
        varNames->push_back(&args.arguments[t]->varName);
        varDecl = new NVariableDecls(args.arguments[t]->varName.type, *varNames);
        varDecl->codeGen(localContext, globalContext, Builder);
        Builder.CreateStore(AI, localContext.locals[args.arguments[t]->varName.name]);
    }

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


Value* NbinOp::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    Value *val;
    if (op == "+") {
        val = Builder.CreateAdd(lhs.codeGen(localContext, globalContext, Builder),
                                rhs.codeGen(localContext, globalContext, Builder));
    } else if (op == "-") {
        val = Builder.CreateSub(lhs.codeGen(localContext, globalContext, Builder),
                                rhs.codeGen(localContext, globalContext, Builder));
    } else if (op == "*") {
        val = Builder.CreateMul(lhs.codeGen(localContext, globalContext, Builder),
                               rhs.codeGen(localContext, globalContext, Builder));
    } else if (op == "/") {
        val = Builder.CreateSDiv(lhs.codeGen(localContext, globalContext, Builder),
                                rhs.codeGen(localContext, globalContext, Builder));
    } else if (op == "<") {
        val = Builder.CreateICmpSLT(lhs.codeGen(localContext, globalContext, Builder),
                                    rhs.codeGen(localContext, globalContext, Builder));
    } else if (op == ">") {
        val = Builder.CreateICmpSGT(lhs.codeGen(localContext, globalContext, Builder),
                                    rhs.codeGen(localContext, globalContext, Builder));
    } else if (op == "<=") {
        val = Builder.CreateICmpSLE(lhs.codeGen(localContext, globalContext, Builder),
                                    rhs.codeGen(localContext, globalContext, Builder));
    } else if (op == ">=") {
        val = Builder.CreateICmpSGE(lhs.codeGen(localContext, globalContext, Builder),
                                    rhs.codeGen(localContext, globalContext, Builder));
    }
    return val;
}

Value* NassignOp::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    cerr << "AssignOp: " << lhs.name << " ";
    cerr <<" ";

    return Builder.CreateStore(rhs.codeGen(localContext, globalContext, Builder),
                              localContext.locals[lhs.name]);
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
    ThenBlock = BasicBlock::Create(llvmContext, "then", localContext.function);
    ElseBlock = BasicBlock::Create(llvmContext, "else", localContext.function);
    PhiBlock = BasicBlock::Create(llvmContext, "endif", localContext.function);

    Builder.CreateCondBr(condition.codeGen(localContext, globalContext, Builder),
                         ThenBlock, ElseBlock);

    Builder.SetInsertPoint(ThenBlock);
    ifBlock.codeGen(localContext, globalContext, Builder);
    Builder.CreateBr(PhiBlock);

    Builder.SetInsertPoint(ElseBlock);
    elseBlock.codeGen(localContext, globalContext, Builder);
    Builder.CreateBr(PhiBlock);

    Builder.SetInsertPoint(PhiBlock);

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