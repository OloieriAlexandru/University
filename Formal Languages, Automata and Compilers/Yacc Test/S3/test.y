%{
#include <stdio.h>
void yyerror(char *str) { printf("%s\n", str); }
%}

%start check
%token INT FLOAT CHAR FNC ID NR NRF CALL

%%
      
check		: program 							{ printf("Esti bun\n"); }
		;

program		: function_declarations main					{ }
		;

function_declarations	: function_declarations	function_declaration		{ }
			| function_declaration					{ }
			;

function_declaration	: FNC '(' function_declaration_params ')' ',' simple_type ':' ID ';'		{ }
			| FNC '(' ')' ',' simple_type ':' ID ';'                                        { }
			;

function_declaration_params 	: function_declaration_params ',' function_declaration_param               	{ }
				| function_declaration_param							{ }
				;

function_declaration_param 	: simple_type ID '[' NR ']'						{ }
				| simple_type ID							{ }
				;

simple_type		: CHAR							{ }
			| INT							{ }
			| FLOAT							{ }
			;

main			: main function_call_complete				{ }
			| function_call_complete				{ }
			;

function_call_complete	: function_call ';'                                     { }
			;

function_call		: ID '=' CALL '(' function_call_params ')' ':' ID 	{ }
			| ID '=' CALL '(' ')' ':' ID 				{ }
			;

function_call_params	: function_call_params ',' arithmetic_expression	{ }
			| arithmetic_expression					{ } 
			;
                                                                                   
arithmetic_expression	: arithmetic_expression '+' arithmetic_expression_term  { }
			| arithmetic_expression '-' arithmetic_expression_term  { }
			| arithmetic_expression '*' arithmetic_expression_term  { }
			| arithmetic_expression '/' arithmetic_expression_term  { }
			| arithmetic_expression_term				{ }
			;

arithmetic_expression_term	: NR						{ }
				| NRF						{ }
				| ID						{ }
				| '(' function_call ')'				{ }
				;

%%

int main() {
	yyparse();
 	return 0;
}