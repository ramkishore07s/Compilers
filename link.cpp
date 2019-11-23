#include "node.h"

void linkIoFunctions(Module *module, IRBuilder<> &Builder) {
    std::vector<Type*> outargs;
    outargs.push_back(Builder.getInt32Ty());

    FunctionType *outtype = FunctionType::get(Builder.getVoidTy(), makeArrayRef(outargs), false);
    module->getOrInsertFunction("outputInt", outtype);

    FunctionType *outtype2 = FunctionType::get(Builder.getVoidTy(), makeArrayRef(outargs), false);
    module->getOrInsertFunction("outputUint", outtype);

    std::vector<Type*> outargs2;
    outargs2.push_back(Builder.getInt32Ty());
    FunctionType *outtype3 = FunctionType::get(Builder.getVoidTy(), makeArrayRef(outargs2), false);
    module->getOrInsertFunction("outputChar", outtype3);

    /*
    std::vector<Type*> inargs;
    FunctionType *intype = FunctionType::get(Builder.getInt32Ty(), makeArrayRef(inargs), false);
    module->getOrInsertFunction("inputInt", intype);

    FunctionType *intype2 = FunctionType::get(Builder.getInt32Ty(), makeArrayRef(inargs), false);
    module->getOrInsertFunction("inputChar", intype2);
    */
    //std::vector<Type*> readStrArgs;
    //readStrArgs.push_back(Type::getInt8PtrTy(getGlobalContext()));
    //FunctionType *input_str = FunctionType::get(Type::getInt8PtrTy(getGlobalContext()), makeArrayRef(readStrArgs), false);
    //module->getOrInsertFunction("readStr", input_str);
}