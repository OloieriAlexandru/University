%{
#include <stdio.h>
#include "test.tab.h"
%}
%option noyywrap
%option yylineno
%%
"/*"(.|\n)+"*/" ;
"<body>" {return BODY;}
"</body>" {return ENDBODY;}
"<item>" {return ITEM;}
"</item>" {return ENDITEM;}
"<list>" {return LIST;}
"</list>" {return ENDLIST;}
"<entry>" {return ENTRY;}
"</entry>" {return ENDENTRY;}
\|[A-Za-z0-9 ]+\| {return ID;}
[ \t] ;
\n {yylineno++;}
. {return yytext[0];}
%%
