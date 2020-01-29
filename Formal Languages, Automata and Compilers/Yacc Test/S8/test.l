%{
#include <stdio.h>
#include "test.tab.h"
%}
%option noyywrap
%option yylineno
%%
"/*"(.|\n)+"*/" ;
"<object>" {return OBJTAG;}
"</object>" {return EOBJTAG;}
\"[^\"]+\" {return STRINGVAL;}
"<array>" {return ARRAYTAG;}
"</array>" {return EARRAYTAG;}
"<string>" {return STRINGTAG;}
"</string>" {return ESTRINGTAG;}
"<number>" {return NRTAG;}
"</number>" {return ENRTAG;}
"<key>" {return KEYTAG;}
"</key>" {return EKEYTAG;}
"<val>" {return VALTAG;}
"</val>" {return EVALTAG;}
[1-9][0-9]*|0 {return NR;}
[ \t] ;
\n {yylineno++;}
. {return yytext[0];}
%%