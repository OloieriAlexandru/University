%{
#include <stdio.h>
#include "test.tab.h"
%}
%option noyywrap
%option yylineno
%%
"/*"(.|\n)+"*/" ;
"let" {return LET;}
"Object" {return OBJTYPE;}
\"[^\"]+\" {return STRINGVAL;}
"Array" {return ARRAYTYPE;}
"String" {return STRINGTYPE;}
"Number" {return NUMBERTYPE;}
"eval_expr" {return EVAL;}
"eval_array" {return EARRAY;}
[A-Za-z][A-Za-z0-9]* {return ID;}
[1-9][0-9]*|0 {return NR;}
[ \t] ;
\n {yylineno++;}
. {return yytext[0];}
%%