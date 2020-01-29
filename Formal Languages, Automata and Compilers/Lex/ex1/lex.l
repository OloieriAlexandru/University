%{
#include <stdio.h>
%}
cifra [0-9]
nr {cifra}+

goodLetter [a-y]
letter [a-z]
twoLetters {letter}{letter}
item  {nr}|{twoLetters}*({letter}{goodLetter})
items ({item};)*{item}
%%
\[{items}\] {printf("gasit lista: %s\n", yytext);}
\<{nr}\> {ECHO;}
ab+  {printf ("[%s]\n", yytext);}
.|\n ;
%%

int yywrap() { return 1; } 

int main(int argc, char** argv){
FILE *g;
if(argc>0)
 yyin = fopen(argv[1],"r");
       
yylex();
}


