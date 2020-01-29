%{
#include <stdio.h>
#include "test.tab.h"
%}
%option noyywrap
%option yylineno
%%
"/*"(.|\n)+"*/" ;
"<xml>" {return XMLTAG;}
"</xml>" {return EXMLTAG;}
\"[^\"]+\" {return CHARSVAL;}
"<list>" {return LISTTAG;}
"</list>" {return ELISTTAG;}
"<chararray>" {return CHARSTAG;}
"</chararray>" {return ECHARSTAG;}
"<integer>" {return NRTAG;}
"</integer>" {return ENRTAG;}
"<data>" {return DATATAG;}
"</data>" {return EDATATAG;}
"<value>" {return VALTAG;}
"</value>" {return EVALTAG;}
"0"|-?[1-9][0-9]* {return NR;}                                                                  
[ \t] ;
\n {yylineno++;}
. {return yytext[0];}
%%