%{   
#include <stdio.h>             
extern int yylineno;
void yyerror(char *str) { printf("%s la linia %d\n", str, yylineno); } 
%}

%start check
%token PROGRAM ENDP DECL ENDDECL CODE ENDCODE INT FLOAT CHAR VAR TYPE SMPL ARR ASGN ASRT PRINT ID NR NRF ITM

%%

check		: program				{ printf("Esti bun\n"); }
		;

program		: PROGRAM declarations code ENDP	{ }
		;

declarations	: DECL decl_list ENDDECL		{ }
		| DECL ENDDECL				{ }
		;

decl_list	: decl_list decl			{ }
		| decl					{ }
		;

decl		: VAR ID '(' TYPE type_list ',' SMPL ')'{ }
		| VAR ID '(' TYPE type_list ',' ARR ')' { }
		| type_list ID '=' '(' term ')'		{ }
		;

code 		: CODE instr_list ENDCODE		{ }
		| CODE ENDCODE				{ }
		;

instr_list	: instr_list instr_full			{ }
		| instr_full				{ }
		;

instr_full	: instr					{ }
		| PRINT '(' instr_list_2 ')'		{ }
		;

instr_list_2	: instr_list_2 ',' instr		{ }
		| instr_list_2 ',' term			{ }
		| instr					{ }
		| term					{ }
		;

instr		: ASGN '(' ID ',' full_term ')'		{ }
		| ASRT '(' code_term ')'		{ }
		| ITM '(' ID ',' NR ')'			{ }
		;

full_term	: term					{ }
		| code_term				{ }
		;

code_term	: '+' '(' term_list ')'			{ }
		| '*' '(' term_list ')'			{ }
		;

term_list	: term_list ',' full_term_2		{ }
		| full_term_2				{ }
		;

full_term_2	: ITM '(' ID ',' NR ')'			{ }
		| term					{ }
		;

term		: NR					{ }
		| NRF					{ }
		| ID					{ } 
		;

type_list	: INT					{ }
		| FLOAT					{ }
		| CHAR 					{ }
		;

%%

int main() {    
	yyparse();
 	return 0;
}