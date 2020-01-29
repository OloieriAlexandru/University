%{
#include <stdio.h>
#include "y.tab.h"
%}
%option yylineno
%%
"/*"(.|\n)+"*/" ;
"int" {return INT;}
"float" {return FLOAT;}
"char" {return CHAR;}
"call" {return CALL;}
"function" {return FNC;}
[A-Za-z][A-Za-z0-9]* {return ID;}
[1-9][0-9]*|0 {return NR;}
([1-9][0-9]*\.[0-9]+)|(0\.[0-9]+) {return NRF;}
[ \t] ;
\n {yylineno++;}
. {return yytext[0];}
%%

 