bison -d -o parser.cpp parser.y
flex -o scanner.cpp scanner.l parser.hpp
g++ parser.cpp scanner.cpp -ll -o parser
