%{
#include <stdio.h>
int i=0;
%}
%%
infoiasi {i++;}
inf {printf("INF");}
%%
int yywrap() { return 1; }
int main(int argc, char** argv ){
if(argc>0)
 yyin = fopen(argv[1],"r");
yylex();
printf("\n numarul de aparitii ale sirului infoiasi: %d\n",i);
    
}