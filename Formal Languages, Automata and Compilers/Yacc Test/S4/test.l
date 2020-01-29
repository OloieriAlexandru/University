%{
#include <stdio.h>
#include "test.tab.h"
%}
%option noyywrap
%option yylineno
%%
"/*"(.|\n)+"*/" ;
"public" {return PUBLIC;}
"private" {return PRIVATE;}
"protected" {return PROTECTED;}
"int" {return INT;}
"float" {return FLOAT;}
"char" {return CHAR;}
"class" {return CLASS;}
"return" {return RET;}
[A-Za-z][A-Za-z0-9]* {return ID;}
[1-9][0-9]*|0 {return NR;}
([1-9][0-9]*\.[0-9]+)|(0\.[0-9]+) {return NRF;}
[ \t] ;
\n {yylineno++;}
. {return yytext[0];}
%%