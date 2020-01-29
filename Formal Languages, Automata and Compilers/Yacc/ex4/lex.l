%{
#include <stdio.h>
#include "yacc.tab.h"
%}
%option yylineno
%%
"int"|"float"|"char"|"string" {return TIP;}
"begin_progr" {return BGIN;}
"end_progr" {return END;}
@[_a-zA-Z][_a-zA-Z0-9]* {return ID;}
":=" {return ASSIGN;}
[0-9]+ {return NR;}
[ \t] ;
\n {yylineno++;}
. {return yytext[0];}
%%
int yywrap() { return 1; }