%token IDENTIFIER CONSTANT INT CHAR FLOAT

%{
	#include <stdio.h>
	#include <stdlib.h>
	#include "c_decl_node.h"
	#include "c_decl_gram.hh"

	extern int yyerror (char *s);
	extern int yylex();

	void insert(string name, int data_type)
	{
		string str;

		switch(data_type){
			case INT_TYPE: str="INT";break;
			case CHAR_TYPE : str="CHAR";break;
			case FLOAT_TYPE : str="FLOAT";break;
		}
		printf("Identifier name=%s type=%s\n",name.c_str(),str.c_str());
	}


%}

%start declaration_list


%%

declaration_list 	: declaration_list declaration
                 	| declaration
			;

declaration 		: type_spec identifier_list ';'
			;

type_spec		: INT { $$ = new node("type_spec",INT_TYPE); }
			| CHAR  { $$ = new node("type_spec",CHAR_TYPE); }
			| FLOAT { $$ = new node("type_spec",FLOAT_TYPE); }
			;

identifier_list		: identifier_list ',' IDENTIFIER
			  { 

				insert($3->name,($0)->data_type);
				$$ = new node("identifier_list",($0)->data_type);
			  } 
			| IDENTIFIER 
			  { 
				insert($1->name,($0)->data_type);
				$$ = new node("identifier_list",$0->data_type);

			  }
			;
					;
%%
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern FILE *yyin;
extern int yydebug;

int
main (int argc, char **argv)
{
   int yyparse ();
   int result;
	FILE *fp;

   if (argc != 2) {
      printf ("Usage: %s <input file> \n", argv[0]);
      exit (1);
   }

//	         yydebug=1;

   fp=fopen(argv[1],"r");

	if(fp == NULL ){
		printf("The file [%s] does not exist or is not readable\n",argv[1]);
		exit(1);
	}

	yyin=fp;

   result = yyparse ();

   if (result == 0) {
      printf ("SYNTAX CORRECT \n");
   } else {
      printf ("SYNTAX INCORRECT \n");
		return(0);
   }

   return (0);
}
