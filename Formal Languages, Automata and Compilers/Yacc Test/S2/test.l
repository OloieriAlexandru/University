%{
#include <stdio.h>
#include "test.tab.h"
%}
%option noyywrap
%option yylineno
%%
"/*"(.|\n)+"*/" ;
"def" {return DEF;}
"classes" {return CLASSES;}
"class" {return CLS;}
"end" {return END;}
"main" {return MAIN;}
"new" {return NEW;}
"atrib" {return ATRIB;}
"method" {return FCT;}
"invoke" {return INVOKE;}
"eval" {return EVAL;}
[A-Za-z][A-Za-z0-9]* {return ID;}
[1-9][0-9]*|0 {return NR;}
[ \t] ;
\n {yylineno++;}
. {return yytext[0];}
%%