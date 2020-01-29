%{   
#include <stdio.h>             
extern int yylineno;
void yyerror(char *str) { printf("%s la linia %d\n", str, yylineno); } 
%}

%start check
%token BODY ENDBODY ITEM ENDITEM LIST ENDLIST ENTRY ENDENTRY ID

%%

check		: program				{ printf("Esti bun\n"); }
		;

program		: BODY main ENDBODY                     { }
		| BODY ENDBODY				{ }
		;

main		: main main_item			{ }
		| main_item				{ }
		;

main_item	: ITEM items ENDITEM			{ }
		| ITEM ENDITEM				{ }
		;

items		: items list_obj                        { }
		| items entry	                        { }
		| list_obj	                        { }
		| entry	                                { }
		;                 
		
list_obj	: LIST list_body ENDLIST 		{ }
		| LIST ENDLIST				{ }
		;

list_body	: list_body entry			{ }
		| entry					{ }
		;

entry		: ENTRY ID ENDENTRY			{ }
		| ENTRY list_obj ENDENTRY		{ }

		;


%%

int main() {    
	yyparse();
 	return 0;
}