%{
#include <stdio.h>
#include "yacc.tab.h"
%}
%%
[0-9]+ {yylval=atoi(yytext); return SMB;}
[ \t] ;
\n {return 0;}
. {return yytext[0];}
%%
int yywrap() { return 1; }