%{
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "node.h"
#include "parser.hpp"

#define SAVE_TOKEN  yylval.str = new std::string(yytext, yyleng)
#define TOKEN(t)    (yylval.token = t)

int lineno = 0;
using namespace std;
%}

%option noyywrap


%%
[ \t] 	    ;
"int"       SAVE_TOKEN; return TYPE;
"uint"      SAVE_TOKEN; return TYPE;
"bool"      SAVE_TOKEN; return TYPE;
"char"      SAVE_TOKEN; return TYPE;
"file"      SAVE_TOKEN; return TYPE;

"for"       return TOKEN(FOR);
"while"     return TOKEN(WHILE);
"if"        return TOKEN(IF);
"else"      return TOKEN(ELSE);

"break"     return TOKEN(BREAK);
"continue"  return TOKEN(CONTINUE);

"return"    return TOKEN(RETURN);

\{         return TOKEN(LBRACE);
\}         return TOKEN(RBRACE);
\(         return TOKEN(LPAREN);
\)         return TOKEN(RPAREN);
\[         return TOKEN(LSQPAREN);
\]         return TOKEN(RSQPAREN);
;          return TOKEN(SCOLON);
=          return TOKEN(EQTO);

\+         SAVE_TOKEN; return BINOPADDSUB;
-          SAVE_TOKEN; return BINOPADDSUB;
\*         SAVE_TOKEN; return BINOPMULDIV;
\/         SAVE_TOKEN; return BINOPMULDIV;
%          SAVE_TOKEN; return BINOPMOD;

==         SAVE_TOKEN; return BINOPBOOL;
\<         SAVE_TOKEN; return BINOPBOOL;
\>         SAVE_TOKEN; return BINOPBOOL;
\<=        SAVE_TOKEN; return BINOPBOOL;
\>=?       SAVE_TOKEN; return BINOPBOOL;

:          return TOKEN(COLON);
\?         return TOKEN(QMARK);
,          return TOKEN(COMMA);

\"[^\"]*\"     SAVE_TOKEN; return STR;
[0-9][0-9]*   SAVE_TOKEN; return NUM;
'.'         SAVE_TOKEN; return CHAR;

#.*        ;
[a-zA-Z][a-zA-Z0-9]*	SAVE_TOKEN; return ID;
\n		   ++lineno;

.		{ cout << "Line No %d: Unrecognized character" << lineno << yytext[0];std::exit(1); }

%%
//

/*
int main()
{
    // open a file handle to a particular file:
    FILE *myfile = fopen("test.code", "r");
    // make sure it's valid:
    if (!myfile) {
        std::cout << "I can't open file!" << endl;
        return -1;
    }
    // set lex to read from it instead of defaulting to STDIN:
    yyin = myfile;

    // lex through the input:
    while(yylex());
}

*/