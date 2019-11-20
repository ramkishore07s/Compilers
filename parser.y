%{
    #include <stdio.h>
    #include <iostream>
    #include "node.h"

    NProgram *programNode;

    using namespace std;
    extern int yylex();
    void yyerror(char *s) { std::printf("Error: %s\n", s);std::exit(1); }
%}

%union {
    Node *node;
    NProgram *program;
    NFunctionDef *functionDef;
    NfunctionCall *functionCall;
    NStatement *statement;
    NStatementBlock *statementBlock;

    NVariableName *varName;
    VarNames *varNames;
    Nconstant *constant;
    Nnum *n;
    NChar *c;

    Expr *expr;
    Narg *arg;
    Nparam *param;
    Nparams *params;
    Nargs *arguments;

    NbinOp *binOp;
    NassignOp * assignOp;
    NconditionalOp *conditionalOp;

    NifBlock *ifBlock;
    NwhileBlock *whileBlock;
    NforBlock *forBlock;
    Nbreak *nbreak;
    Ncontinue *ncontinue;
    Nreturn *returnStmt;

    int token;
    std::string *str;
}

%token  <str> ID STR BINOPADDSUB BINOPMULDIV BINOPMOD BINOPBOOL TYPE NUM CHAR
%token  <token> LBRACE RBRACE LPAREN RPAREN LSQPAREN RSQPAREN
%token  <token> EQTO QMARK
%token  <token> FOR WHILE IF ELSE RETURN BREAK CONTINUE
%token  <token> COMMA COLON SCOLON

%type <program> program functionDefs
%type <functionDef> functionDef
%type <statement> stmt variableDecls
%type <statementBlock> stmts stmtBlock
%type <varNames> ids
%type <varName> varName
%type <constant> constant
%type <expr> expr
%type <param> fParam
%type <params> fParams
%type <arg> arg
%type <arguments> args
%type <assignOp> assignExpr
%type <conditionalOp> condiExpr
%type <ifBlock> ifBlock
%type <whileBlock> whileBlock
%type <forBlock> forBlock
%type <functionCall> funcCall


%left QMARK
%left COLON
%left BINOPADDSUB
%left BINOPMOD
%left BINOPMULDIV
%left BINOPBOOL


%start program

%%

program : functionDefs { programNode=$1; }
;

args : arg { $$ = new Nargs(); $$->arguments.push_back($<arg>1); }
| args COMMA arg { $1->arguments.push_back($<arg>3); }
;

arg : TYPE varName { $$ = new Narg(*$1, *$2); }
;

functionDefs : functionDef { $$ = new NProgram(); $$->functions.push_back($<functionDef>1); }
| functionDefs functionDef { $1->functions.push_back($<functionDef>2); }
;

functionDef : TYPE ID LPAREN args RPAREN stmtBlock { $$ = new NFunctionDef(*$1, *$2, *$4, *$6); }
| TYPE ID LPAREN RPAREN stmtBlock { $$ = new NFunctionDef(*$1, *$2, *$5); }
;

stmtBlock : LBRACE stmts RBRACE { $$ = $2; }
;

stmts : stmts stmt { $1->statements.push_back($2); }
| stmt { $$ = new NStatementBlock(); $$->statements.push_back($1); }
;

constant : varName { $$ = $1; }
| NUM { $$ = new Nnum(stoi(*$1)); }
| CHAR { $$ = new NChar(*$1); }
;

expr : constant { $$ = $1; }
| expr BINOPADDSUB expr { $$ = new NbinOp(*$1, *$3, *$2); }
| expr BINOPMULDIV expr { $$ = new NbinOp(*$1, *$3, *$2);  }
| expr BINOPMOD expr { $$ = new NbinOp(*$1, *$3, *$2);  }
| expr BINOPBOOL expr { $$ = new NbinOp(*$1, *$3, *$2); }
| funcCall { $$ = $1; }
| condiExpr { $$ = $1; }
| LPAREN expr RPAREN { $$ = $2; }
| BINOPADDSUB expr { $$ = new NbinOp(*((Expr*) new Nnum(0)), *$2, *$1); }
;

stmt : variableDecls { $$ = $1; }
| expr SCOLON { $$ = $1; }
| RETURN expr SCOLON { $$ = new Nreturn(*$2); }
| ifBlock { $$ = $1; }
| whileBlock { $$ = $1; }
| forBlock { $$ = $1; }
| assignExpr SCOLON { $$ = $1; }
| SCOLON {}
| BREAK SCOLON { $$ = new Nbreak(); }
| CONTINUE SCOLON { $$ = new Ncontinue(); }
;

assignExpr : varName EQTO expr { $$ = new NassignOp(*$1, *$3); }
;

condiExpr : expr QMARK expr COLON expr { $$ = new NconditionalOp(*$1, *$3, *$5); }
;

funcCall : ID LPAREN fParams RPAREN { $$ = new NfunctionCall(*$1, *$3); }
| ID LPAREN RPAREN { $$ = new NfunctionCall(*$1); }
;

fParams : fParams COMMA fParam { $1->params.push_back($3); }
| fParam { $$ = new Nparams(); $$->params.push_back($1); }
;

fParam : expr { $$ = new Nparam(*$1); }
| STR { $$ = new Nparam(*(new Nstr(*$1))); }
;

ifBlock : IF LPAREN expr RPAREN stmtBlock { $$ = new NifBlock(*$3, *$5); }
| IF LPAREN expr RPAREN stmtBlock ELSE stmtBlock { $$ = new NifBlock(*$3, *$5, *$7); }
;

whileBlock : WHILE LPAREN expr RPAREN stmtBlock { $$ = new NwhileBlock(*$3, *$5); }
;

forBlock : FOR LPAREN assignExpr SCOLON expr SCOLON assignExpr RPAREN stmtBlock { $$ = new NforBlock(*$3, *$5, *$7, *$9); }
;

variableDecls : TYPE ids SCOLON { $$ = new NVariableDecls(*$1, *$2); }
;

ids : ids COMMA varName { $1->push_back($3); }
| varName { $$ = new VarNames(); $$->push_back($1); }
;

varName : ID { $$ = new NVariableName(*$1); }
| varName LSQPAREN NUM RSQPAREN { $1->sizes.push_back(stoi(*$3)); }
;

%%

int main(int argc, char **argv)
{
    yyparse();
    printf("Parsing Over\n");
    programNode->debug();

    ModuleOb->print(llvm::errs(), nullptr);
    return 0;
}




