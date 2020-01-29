%{   
#include <stdio.h>             
extern int yylineno;
void yyerror(char *str) { printf("%s la linia %d\n", str, yylineno); } 
%}

%start check
%token NR OBJTAG EOBJTAG STRINGVAL ARRAYTAG EARRAYTAG STRINGTAG ESTRINGTAG NRTAG ENRTAG KEYTAG EKEYTAG VALTAG EVALTAG

%%

check		: program				{ printf("Esti bun\n"); }
		;

program		: program decl				{ }
		| decl					{ }
		;

decl		: number_decl				{ }
		| string_decl				{ }
		| object_decl				{ }
		| arr_decl				{ }
		;

number_decl	: NRTAG NR ENRTAG			{ }
		;

string_decl	: STRINGTAG string_list ESTRINGTAG	{ }
		| STRINGTAG ESTRINGTAG			{ }
		;

string_list	: string_list ':' ':' string_list_item	{ }
		| string_list_item			{ }
		;

string_list_item	: NR				{ }
			| STRINGVAL			{ }
			;

object_decl	: OBJTAG obj_body EOBJTAG		{ }
		| OBJTAG EOBJTAG			{ }
		;

obj_body	: obj_body obj_item			{ }
		| obj_item				{ }
		;

obj_item	: KEYTAG decl EKEYTAG			{ }
		;

arr_decl	: ARRAYTAG array_items EARRAYTAG	{ }
		| ARRAYTAG EARRAYTAG			{ }
		;

array_items	: array_items array_val			{ }
		| array_val				{ }
		;

array_val	: VALTAG decl EVALTAG			{ }
		; 

%%

int main() {    
	yyparse();
 	return 0;
}