%token IDENTIFIER CONSTANT OPERATOR EQ_TO_OP SEMI_COLON 

%{
	#include <stdlib.h>
	#include "c-stmt-hdr.h"
	extern int yyerror();
	extern int yylex();
%}


%start c_statement

%%

c_statement
		: IDENTIFIER EQ_TO_OP c_expression SEMI_COLON
		;

c_expression
		: CONSTANT  
		| IDENTIFIER 
		| c_expression OPERATOR CONSTANT
		| c_expression OPERATOR IDENTIFIER
		;


%%
