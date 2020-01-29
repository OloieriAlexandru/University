%{   
#include <stdio.h>             
extern int yylineno;
void yyerror(char *str) { printf("%s la linia %d\n", str, yylineno); }
%}

%start check
%token LET OBJTYPE STRINGVAL ARRAYTYPE STRINGTYPE NUMBERTYPE EVAL EARRAY ID NR

%%

check		: program				{ printf("Esti bun\n"); }
		;

program		: declarations main			{ }
		| declarations 				{ }
		;

declarations	: declarations declaration		{ }
		| declaration				{ }
		;

declaration	: normal_decl				{ }
		| object_decl                           { }
		;

object_decl	: LET ID ':' OBJTYPE '=' object_body ';'	{ }   
		;

object_body	: '{' object_body_list '}'			{ }
		| '{' '}'					{ }		
		;

object_body_list 	: object_body_list ',' object_body_item	{ }
			| object_body_item			{ }
			;

object_body_item	: ID ':' STRINGVAL			{ }
			| ID ':' NR				{ }
			| ID ':' array_decl			{ }
			| ID ':' object_body			{ } 
			;	

normal_decl	: LET ID ':' STRINGTYPE '=' STRINGVAL ';'	{ }
		| LET ID ':' NUMBERTYPE '=' NR	      ';'	{ }
		| LET ID ':' ARRAYTYPE '=' array_decl ';'	{ }
		;

array_decl	: '[' array_decl_list ']'		{ }
		| '[' ']'
		;

array_decl_list	: array_decl_list ',' array_decl_elem	{ }
		| array_decl_elem			{ }
		;

array_decl_elem	: STRINGVAL  				{ }
		| NR                                    { }
		| array_decl				{ }
		| object_body				{ }
		;

main		: main normal_eval 			{ }
		| main normal_eval_arr			{ }
		| normal_eval				{ }
		| normal_eval_arr			{ }
		;
                      
normal_eval	: EVAL ':' arithmetic_expression ';'	{ }
		;

normal_eval_arr	: EARRAY ':' '[' arithmetic_expressions_list ']' ';'	{ }
		| EARRAY ':' '[' ']' ';'				{ }
		;

arithmetic_expression	: arithmetic_expression '+' term	{ }
			| arithmetic_expression '-' term	{ }
			| arithmetic_expression '*' term	{ }
			| arithmetic_expression '/' term	{ }
			| term					{ }
			;

term		: NR
		| ID
		| '(' arithmetic_expression ')'			{ }
		;

arithmetic_expressions_list	: arithmetic_expressions_list ',' arithmetic_expression		{ }
				| arithmetic_expression						{ }
				;

%%

int main() {    
	yyparse();
 	return 0;
}