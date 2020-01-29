%{
#include <stdio.h>
#include "yacc.tab.h"
%}
%%
[a-z] {return SMB;}
[ \t] ;
\n {return 0;}
. {return yytext[0];}
%%            
int yywrap() { return 1; }