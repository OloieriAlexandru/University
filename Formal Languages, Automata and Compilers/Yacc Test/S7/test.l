%{
#include <stdio.h>
#include "test.tab.h"
%}
%option noyywrap
%option yylineno
%%
"/*"(.|\n)+"*/" ;
"program" {return PROGRAM;}
"endP" {return ENDP;}
"declaration" {return DECL;}
"endD" {return ENDDECL;}
"code" {return CODE;}
"endC" {return ENDCODE;}
"int" {return INT;}
"float" {return FLOAT;}
"char" {return CHAR;}
"var" {return VAR;}
"type" {return TYPE;}
"simple" {return SMPL;}
"array" {return ARR;}
"assign" {return ASGN;}
"assert" {return ASRT;}
"item" {return ITM;}
"print" {return PRINT;}
[A-Za-z][A-Za-z0-9]* {return ID;}
[1-9][0-9]*|0 {return NR;}
([1-9][0-9]*\.[0-9]+)|(0\.[0-9]+) {return NRF;}
[ \t] ;
\n {yylineno++;}
. {return yytext[0];}
%%