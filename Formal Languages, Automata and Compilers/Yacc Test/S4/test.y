%{                
#include <stdio.h>
void yyerror(char *str) { printf("%s\n", str); }
%}

%start check
%token PUBLIC PRIVATE PROTECTED INT FLOAT CHAR CLASS RET ID NR NRF                                             

%%

check		: program					{ printf("!!!\n"); }
		;

program 	: program class_def				{ }
		| program function_def				{ }
		| class_def					{ }
		| function_def					{ }
		;

class_def	: class_sig ID '{' class_body  '}'		{ }
		| class_sig ID '{' '}'				{ }
		;

class_sig	: PROTECTED CLASS				{ }
		| PRIVATE CLASS					{ }
		| PUBLIC CLASS					{ }
		| CLASS						{ }
		;

class_body	: PUBLIC ':' class_decl PROTECTED ':' class_decl PRIVATE ':' class_decl		{ }
		| PUBLIC ':' class_decl PROTECTED ':' class_decl				{ }
		| PUBLIC ':' class_decl PRIVATE ':' class_decl					{ }
		| PROTECTED ':' class_decl PRIVATE ':' class_decl				{ }
		| PUBLIC ':' class_decl								{ }
		| PROTECTED ':' class_decl							{ }
		| PRIVATE ':' class_decl                                                        { }
		;

class_decl	: class_decl var_decl								{ }
		| class_decl function_decl							{ }
		| var_decl									{ }
		| function_decl									{ }
		;

var_decl	: type var_decl_list ';'							{ }
		;

var_decl_list	: var_decl_list ',' var_decl_decl						{ }
		| var_decl_decl									{ }
		;

var_decl_decl	: ID							{ }
		| ID '[' NR ']'
		| ID '=' NR						{ }
		| ID '=' NRF						{ }
		| ID '=' ID						{ }
		;

function_decl	: type ID '(' function_params ')' function_body ';'	{ }
		| type ID '(' function_params ')' ';'			{ }
		| type ID '(' ')' function_body ';'			{ }
		| type ID '(' ')' ';'					{ }
		;

function_params	: function_params ',' function_param			{ }
		| function_param					{ }
		;

function_param	: type ID						{ }
		| type ID '[' NR ']'					{ }
		;

function_body	: '{' return_stat '}'					{ }
		;

return_stat	: RET ';'						{ }
		| RET ID ';'						{ }
		| RET NR ';'						{ }
		| RET NRF ';'						{ }
		;

function_def	: ID ':' ':' ID '(' function_params ')' function_body ';'	{ }
		| ID ':' ':' ID '(' function_params ')' ';'		{ }
		| ID ':' ':' ID '(' ')' function_body ';'		{ }
		| ID ':' ':' ID '(' ')' ';'				{ }
		;

type		: INT							{ }
		| FLOAT							{ }
		| CHAR							{ }
		;

%%
      
int main(){   
	yyparse();
 	return 0;
}