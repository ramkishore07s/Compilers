all: parser

OBJS = parser.o  \
       scanner.o \
       codegen.o \
       debug.o \
       main.o

LLVMCONFIG = llvm-config
CPPFLAGS = `$(LLVMCONFIG) --cppflags`
LDFLAGS = `$(LLVMCONFIG) --ldflags` -lpthread -ldl -lz -lncurses -rdynamic
LIBS = `$(LLVMCONFIG) --libs core executionengine mcjit interpreter analysis target codegen support X86Disassembler ObjCARCOpts ScalarOpts InstCombine TransformUtils MC Object RuntimeDyld X86AsmParser X86CodeGen SelectionDAG AsmPrinter MCParser`
#LIBS = -lLLVMCore -lLLVMSupport -lLLVMCJIT -lLLVMRuntimeDyld -lLLVMExecutionEngine -lLLVMCodeGen -lLLVMObjCARCOpts -lLLVMScalarOpts -lLLVMInstCombine -lLLVMTransformUtils -lLLVMipa -lLLVMAnalysis -lLLVMTarget -lLLVMMC -lLLVMObject -lLLVMCore -lLLVMSupport -lLLVMMCJIT -lLLVMJIT -lLLVMCodeGen
#-lLLVMObjCARCOpts -lLLVMScalarOpts -lLLVMInstCombine -lLLVMTransformUtils -lLLVMipa -lLLVMAnalysis -lLLVMRuntimeDyld -lLLVMExecutionEngine -lLLVMTarget -lLLVMMC -lLLVMObject -lLLVMCore -lLLVMSupport -lLLVMX86Disassembler -lLLVMX86AsmParser -lLLVMX86CodeGen -lLLVMSelectionDAG -lLLVMAsmPrinter -lLLVMMCParser -lLLVMCodeGen -lLLVMObjCARCOpts -lLLVMScalarOpts -lLLVMInstCombine -lLLVMTransformUtils -lLLVMipa -lLLVMAnalysis -lLLVMX86Desc -lLLVMX86Info -lLLVMTarget -lLLVMX86AsmPrinter -lLLVMMC -lLLVMObject -lLLVMX86Utils -lLLVMCore -lLLVMSupport
WARNINGS = -Wno-deprecated-register -Wno-writable-strings

clean:
	$(RM) -rf parser.cpp parser.hpp parser scanner.cpp $(OBJS)

parser.cpp: parser.y
	bison -d -o $@ $^

scanner.cpp: scanner.l parser.hpp
	flex -o $@ $^

%.o: %.cpp
	g++ -std=c++11 $(WARNINGS) -c $(CPPFLAGS) -o $@ $<

parser: $(OBJS)
	g++ -std=c++11 $(WARNINGS) -o $@ $(OBJS) $(LIBS) $(LDFLAGS)