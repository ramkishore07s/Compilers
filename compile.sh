bison -d -o parser.cpp parser.y
flex -o scanner.cpp scanner.l parser.hpp
g++ -std=c++11 -Wno-deprecated-register -Wno-writable-strings parser.cpp scanner.cpp debug.cpp `llvm-config --system-libs --cppflags --ldflags --libs core` -ll -o parser
