%{
#include <stdio.h>
extern FILE* yyin;
extern char* yytext;
extern int yylineno;
%}
%token XMLTAG EXMLTAG CHARSTAG ECHARSTAG CHARSVAL NRTAG ENRTAG LISTTAG ELISTTAG DATATAG EDATATAG VALTAG EVALTAG NR
%start s
%%

s: tags {printf ("Limbaj acceptat! Well done\n");}

tags	: tags decl_char_arr        		{ }
	| tags decl_list                        { }
	| tags decl_xml                         { }
	| tags decl_integer                     { }
	| decl_char_arr                         { }
	| decl_list                             { }
	| decl_xml                              { }
	| decl_integer                          { }
     	;
     
decl_xml	: XMLTAG xml_body EXMLTAG       { }
		| XMLTAG EXMLTAG		{ }
		;

xml_body 	: xml_body decl_data		{ }
		| decl_data			{ }
		;

decl_data	: DATATAG decl_data_body EDATATAG	{ }
		| DATATAG EDATATAG			{ }
		;

decl_data_body	: decl_char_arr			{ }
		| decl_list			{ }
		| decl_xml			{ }
		| decl_integer			{ }
		;

decl_list	: LISTTAG decl_list_body ELISTTAG	{ }
		| LISTTAG ELISTTAG			{ }
		;

decl_list_body	: decl_list_body decl_value		{ }
		| decl_value				{ }
		;

decl_value	: VALTAG decl_value_body EVALTAG	{ }
		| VALTAG EVALTAG			{ }
		;

decl_value_body	: decl_char_arr			{ }
		| decl_list			{ }
		| decl_xml			{ }
		| decl_integer			{ }
		; 

decl_char_arr	: CHARSTAG CHARSVAL ECHARSTAG	{ }
		;

decl_integer	: NRTAG NR ENRTAG		{ }
		;

%%
int yyerror(char * s){
printf("eroare: %s la linia:%d\n",s,yylineno);
}

int main(int argc, char** argv){
     yyin=fopen(argv[1],"r");
     yyparse();
} 