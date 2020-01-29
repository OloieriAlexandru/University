%{
#include <stdio.h>
extern FILE* yyin;
extern char* yytext;
extern int yylineno;
%}
%token DEF CLS CLASSES END MAIN ATRIB NEW FCT NR INVOKE EVAL ID
%start s
%%

s	: progr 							{printf ("Limbaj acceptat! Well done!\n");}

progr	: class_def main						{ }
	;

class_def	: class_def_before class_def_main class_def_after 	{ } 
		;

class_def_before	: DEF CLASSES 					{ }
			;

class_def_main		: class_def_main class_def_def			{ }
			| class_def_def					{ } 
			;

class_def_def		: class_def_def_header '{' class_def_def_body '}' { }
			| class_def_def_header '{' '}' 			{ }
			;

class_def_def_header	: CLS ID					{ }
			;

class_def_def_body	: ATRIB ':' class_def_def_attr FCT ':' class_def_def_fct { }
			| FCT ':' class_def_def_fct				{ }
			| ATRIB ':' class_def_def_attr				{ }
			;

class_def_def_attr	: class_def_def_attr class_def_def_attr_def     { }
			| class_def_def_attr_def                        { } 
			;

class_def_def_attr_def 	: NEW attr_list ';'				{ }
			;

attr_list		: attr_list ',' ID                              { }
			| ID                                            { }
			;				

class_def_def_fct	: class_def_def_fct class_def_def_fct_def	{ }
			| class_def_def_fct_def				{ }
			;

class_def_def_fct_def	: ID '(' fct_params_list ')' ';'		{ }
			| ID '(' ')' ';'				{ }
			;

fct_param		: ID						{ }
			| ID '=' NR                                     { }
			;

fct_params_list		: fct_params_list ',' fct_param			{ }
			| fct_param                                     { }	
			;

class_def_after		: END DEF CLASSES				{ }
			;      

main			: MAIN main_body END MAIN			{ }
			| MAIN END MAIN					{ }
			;

main_body		: main_body function_call			{ }
			| main_body eval_call				{ }
			| function_call					{ }
			| eval_call					{ }
			;

function_call		: INVOKE ID '.' ID ';'				{ }
			;

eval_call		: EVAL ID '.' ID ';'				{ }
			;

%%
int yyerror(char * s){
printf("eroare: %s la linia:%d\n",s,yylineno);
}

int main(int argc, char** argv){
yyin=fopen(argv[1],"r");
yyparse();
} 
