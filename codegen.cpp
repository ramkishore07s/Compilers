#include "node.h"

using namespace std;

Type* getElementType(string type, IRBuilder<> &Builder) {
    if (type == "int") {
        return Builder.getInt32Ty();
    } else if (type == "uint") {
        return Builder.getInt32Ty();
    } else if (type == "bool") {
        return Builder.getInt1Ty();
    } else if (type == "char") {
        return Builder.getInt8Ty();
    }
    return Builder.getInt1Ty();
}

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
    return (Value*) ConstantInt::get(getElementType("int", Builder), int(num), false);
    //return num;
}

Value* NChar::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    cout << "Char: " << int(c);
    return ConstantInt::get(getElementType("char", Builder), int(c), false);
}


Value* NBool::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    cout << "Bool: " << value;
    int c;
    if (value == "true") { c = 1; }
    else { c = 0; }
    return ConstantInt::get(getElementType("bool", Builder), int(c), false);
}



Value* NVariableName::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    cerr<< "VarName :: " << this->type << " : " << this->name << " " << localContext.localtypes[name]->isarg << "; ";
    for (size_t t=0; t<sizes.size(); t++)
        cout << sizes[t] << " ";

    if (localContext.localtypes[name]->isarg) {
        // Arguments cannot be loaded since they are not stored anywhere!
        // All arguments are allocated memory initially
        return localContext.locals[name];
    }

    cout << "\n" << localContext.localtypes[name]->sizes.size() << " " << sizes.size() << "\n";
    if (localContext.localtypes[name]->sizes.size() == 0) {
        return Builder.CreateLoad(localContext.locals[name]);
    } else {
        int index = 0;
        size_t t;
        for (t=0; t<sizes.size()-1; t++) {
            index = index + localContext.localtypes[name]->sizes[t] * sizes[t];
        }
        index = index + sizes[t];
        return Builder.CreateLoad(getElementType(localContext.localtypes[name]->type, Builder), Builder.CreateGEP(localContext.locals[name], Builder.getInt32(index)));
    }
}

Value* NVariableName::codeGen2(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    cout << "\n" << localContext.localtypes[name]->sizes.size() << " dda" << sizes.size() << "\n";
    if (localContext.localtypes[name]->sizes.size() == 0) {
        return localContext.locals[name];
    } else {
        int index = 0;
        size_t t;
        for (t=0; t<sizes.size()-1; t++) {
            index = index + localContext.localtypes[name]->sizes[t] * sizes[t];
        }
        index = index + sizes[t];
        return Builder.CreateGEP(localContext.locals[name], Builder.getInt32(index));
    }
}

Value* NArrayAccess::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    cerr << "array access:: " << name;
    size_t t;
    Value *index, *dim;
    index = Builder.getInt32(0);
    if (exprs.size() > 0) {
        for (t = 0; t < exprs.size() - 1; t++) {
            dim = Builder.CreateMul(Builder.getInt32(localContext.localtypes[name]->sizes[t]),
                                    exprs[t]->codeGen(localContext, globalContext, Builder));
            index = Builder.CreateAdd(index, dim);
        }
        index = Builder.CreateAdd(index, exprs[t]->codeGen(localContext, globalContext, Builder));
        return Builder.CreateLoad(getElementType(localContext.localtypes[name]->type, Builder),
                                  Builder.CreateGEP(localContext.locals[name], index));
    }
    else {
        return Builder.CreateLoad(localContext.locals[name]);
    }
}

Value* NArrayAccess::codeGen2(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    cerr << "array access:: " << name;
    size_t t;
    Value *index, *dim;
    index = Builder.getInt32(0);
    if (exprs.size() > 0) {
        for (t = 0; t < exprs.size() - 1; t++) {
            dim = Builder.CreateMul(Builder.getInt32(localContext.localtypes[name]->sizes[t]),
                                    exprs[t]->codeGen(localContext, globalContext, Builder));
            index = Builder.CreateAdd(index, dim);
        }
        index = Builder.CreateAdd(index, exprs[t]->codeGen(localContext, globalContext, Builder));
        return Builder.CreateGEP(localContext.locals[name], index);
    } else {
        return localContext.locals[name];
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

    funcType = FunctionType::get(getElementType(type, Builder), argTypes, false);
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
    int totalsize = 1;
    for (size_t t=0; t < varNames.size(); t++) {
        //varNames[t]->codeGen is only called for variable access
        if (varNames[t]->sizes.size() == 0) {
            localContext.locals[varNames[t]->name] = Builder.CreateAlloca(getElementType(type, Builder), Builder.getInt32(0), varNames[t]->name);
            localContext.localtypes[varNames[t]->name] = varNames[t];
        } else { //Array types
            for (size_t i=0; i<varNames[t]->sizes.size(); i++) { totalsize *= varNames[t]->sizes[i]; }
            localContext.locals[varNames[t]->name] = Builder.CreateAlloca(getElementType(type, Builder), Builder.getInt32(totalsize), varNames[t]->name);
            localContext.localtypes[varNames[t]->name] = varNames[t];
        }
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
    } else if (op == "&") {
        val = Builder.CreateAnd(lhs.codeGen(localContext, globalContext, Builder),
                               rhs.codeGen(localContext, globalContext, Builder));
    } else if (op == "|") {
        val = Builder.CreateOr(lhs.codeGen(localContext, globalContext, Builder),
                               rhs.codeGen(localContext, globalContext, Builder));
    }
    return val;
}

Value* NnotOp::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    return Builder.CreateNot(expr.codeGen(localContext, globalContext, Builder));
}

Value* NassignOp::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    cerr << "array access assign:: " << lhs.name << lhs.exprs.size() << "\n";
    return Builder.CreateStore(rhs.codeGen(localContext, globalContext, Builder),// localContext.locals[lhs.name]);
                               lhs.codeGen2(localContext, globalContext, Builder));

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

    Builder.getInt1Ty();

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


Value* NforBlock::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    CondBlock = BasicBlock::Create(llvmContext, "loopCondition", localContext.function);
    LoopBlock = BasicBlock::Create(llvmContext, "loop", localContext.function);
    PhiBlock = BasicBlock::Create(llvmContext, "endloop", localContext.function);

    localContext.loopblocks.push(CondBlock);
    localContext.endloopblocks.push(PhiBlock);

    expr1.codeGen(localContext, globalContext, Builder);
    Builder.CreateBr(CondBlock);

    Builder.SetInsertPoint(CondBlock);
    Builder.CreateCondBr(expr2.codeGen(localContext, globalContext, Builder),
                         LoopBlock, PhiBlock);

    Builder.SetInsertPoint(LoopBlock);
    statementBlock.codeGen(localContext, globalContext, Builder);

    expr3.codeGen(localContext, globalContext, Builder);

    Builder.CreateBr(CondBlock);

    localContext.loopblocks.pop();
    localContext.endloopblocks.pop();

    Builder.SetInsertPoint(PhiBlock);

    return nullVal;
}


Value* NwhileBlock::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    CondBlock = BasicBlock::Create(llvmContext, "loopCondition", localContext.function);
    LoopBlock = BasicBlock::Create(llvmContext, "loop", localContext.function);
    PhiBlock = BasicBlock::Create(llvmContext, "endloop", localContext.function);

    localContext.loopblocks.push(CondBlock);
    localContext.endloopblocks.push(PhiBlock);

    Builder.CreateBr(CondBlock);
    Builder.SetInsertPoint(CondBlock);
    Value * val = condition.codeGen(localContext, globalContext, Builder);
    Builder.CreateCondBr(val,
                         LoopBlock, PhiBlock);

    Builder.SetInsertPoint(LoopBlock);
    statementBlock.codeGen(localContext, globalContext, Builder);

    Builder.CreateBr(CondBlock);

    localContext.loopblocks.pop();
    localContext.endloopblocks.pop();

    Builder.SetInsertPoint(PhiBlock);

    return nullVal;
}

Value* Nbreak::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    AfterBreak = BasicBlock::Create(llvmContext, "break", localContext.function);
    Builder.CreateBr(localContext.endloopblocks.top());
    Builder.SetInsertPoint(AfterBreak);

    return nullVal;
}

Value* Ncontinue::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    AfterContinue = BasicBlock::Create(llvmContext, "continue", localContext.function);
    Builder.CreateBr(localContext.loopblocks.top());
    Builder.SetInsertPoint(AfterContinue);

    return nullVal;
}

Value* Nreturn::codeGen(Context &localContext, Context &globalContext, IRBuilder<> &Builder) {
    Builder.CreateRet(expr.codeGen(localContext, globalContext, Builder));
    return nullVal;
}