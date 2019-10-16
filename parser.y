%{
#include <stdio.h>
#include <iostream>

using namespace std;
extern int yylex();
void yyerror(char *s) { std::printf("Error: %s\n", s);std::exit(1); }
%}

%union {
    int token;
    std::string *str;
}

%token  <string> ID NUM STR CHAR BINOPADDSUB BINOPMULDIV BINOPMOD BINOPBOOL TYPE
%token  <token> LBRACE RBRACE LPAREN RPAREN LSQPAREN RSQPAREN
%token  <token> EQTO QMARK
%token  <token> FOR WHILE IF ELSE RETURN
%token  <token> COMMA COLON SCOLON


%left QMARK
%left COLON
%left BINOPADDSUB
%left BINOPMOD
%left BINOPMULDIV
%left BINOPBOOL


%start program

%%

program : functionDecls { cout << "Root\n"; }
;

args : arg args { cout << "argList\n"; }
| COMMA arg { cout << "args\n"; }
|
;

arg : TYPE varName {cout << "arg\n"; }
;

functionDecls : functionDecls functionDecl { cout << "mul Funcs\n"; }
| functionDecl {cout << "single Func\n"; }
;

functionDecl : TYPE ID LPAREN args RPAREN stmtBlock { cout << "func Decl\n"; }
;

stmtBlock : LBRACE stmts RBRACE { cout << "stmtBlock\n"; }
;

stmts : stmts stmt { cout << "stmts\n"; }
| stmt { cout << "stmt\n"; }
;

constant : varName { cout << "expr id\n"; }
| NUM { cout << "expr num\n"; }
| CHAR { cout << "expr char\n"; }
;

expr : expr BINOPADDSUB expr { cout << "addsub\n"; }
| expr BINOPMULDIV expr { cout << "muldiv\n"; }
| expr BINOPMOD expr { cout << "mod\n"; }
| expr BINOPBOOL expr { cout << "binbool\n";}
| constant { cout << "constant\n"; }
| funcCall { cout << "funcCall\n"; }
| condiExpr { cout << "condiExpr\n"; }
| LPAREN expr RPAREN { cout << "paren expr\n"; }
| BINOPADDSUB expr { cout << "signed expr\n"; }
;

stmt : variableDecls { cout << "varDecl\n"; }
| expr SCOLON { cout << "expr\n"; }
| RETURN expr SCOLON { cout << "return\n"; }
| ifBlock { cout << "ifblock\n"; }
| whileBlock { cout << "while block\n"; }
| forBlock { cout << "famous forBlock\n"; }
| assignExpr SCOLON { cout << "assignment\n"; }
| SCOLON
;

assignExpr : varName EQTO expr { cout << "assignment stmt\n"; }
;

condiExpr : expr QMARK expr COLON expr { cout << "condistmt\n"; }
;

funcCall : ID LPAREN fParams RPAREN { cout << "funcCall with args\n"; }
| ID LPAREN RPAREN { cout << "funcCall without args\n"; }
;

fParams : fParams COMMA fParam { cout << "mul params\n"; }
| fParam { cout << "single param: "}
;

fParam : expr { cout << "expr param\n"; }
| STR { cout << "String param\n"; }
;

ifBlock : IF LPAREN expr RPAREN stmtBlock { cout << "only if\n"; }
| IF LPAREN expr RPAREN stmtBlock ELSE stmtBlock { cout << "if and else\n"; }
;

whileBlock : WHILE LPAREN expr RPAREN stmtBlock { cout << "while Block\n"; }
;

forBlock : FOR LPAREN assignExpr SCOLON expr SCOLON assignExpr RPAREN stmtBlock { cout << "forloop"; }
;

variableDecls : TYPE ids SCOLON { cout << "type ids ;\n"; }
;

ids : ids COMMA varName { cout<< "multiple ids\n"; }
| varName { cout << "id\n"; }
;

varName : ID { cout << "id name\n"; }
| varName LSQPAREN NUM RSQPAREN { cout << "array type\n"; }
| varName LSQPAREN varName RSQPAREN { cout << "array with array\n"; }
;

%%

int main(int argc, char **argv)
{
    yyparse();
    printf("Parsing Over\n");
    return 0;
}




