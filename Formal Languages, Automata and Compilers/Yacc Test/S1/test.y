%{                      
#include <stdio.h>
void yyerror(char *c) { printf("%s\n", c); }
%}
              
%token LET OBJTYPE STRINGVAL ARRAYTYPE STRINGTYPE NUMBERTYPE ID NR
%start program

%%           

program : body 					{ printf("Corect!\n"); }
	;

body	: body line				{ }
	| line					{ }
	;

line 	: object_declaration			{ }
	| normal_declaration                    { }
	;

object_declaration	: LET ID ':' OBJTYPE '=' '{' object_body '}' ';' { }
	;

object_body	: object_body ',' object_body_declaration     { }
		| object_body_declaration                 { }
		| /* emtpy */ 				  { }	
		;

object_body_declaration : ID ':' STRINGVAL 		{ }
			| ID ':' NR	                { }
			| ID ':' array_declaration	{ }
			| ID ':' '{' object_body '}'	{ }
			| ID ':'			{ }
			;

array_declaration	: '[' array_elements ']'        { }
			| '[' ']'			{ }
			;

array_elements		: array_elements ',' STRINGVAL 	{ }
			| array_elements ',' NR             {} 
			| array_elements ',' array_declaration { }
			| array_elements ',' '{' object_body '}' { }
			| STRINGVAL                    { }
			| NR                            { }
			| array_declaration		{ }
			| '{' object_body '}' { }
			;
			                                               
normal_declaration 	: LET ID ':' NUMBERTYPE '=' NR ';'             {  }
			| LET ID ':' STRINGTYPE '=' STRINGVAL ';'      {  }
			| LET ID ':' ARRAYTYPE '=' array_declaration ';' {  }
			;                                                             

%%

int main(){
	yyparse();
 	return 0;
}