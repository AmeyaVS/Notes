#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "recur_eval.h"

extern FILE *yyin;
extern int yydebug;

parse_tree_node *parse_root;
declaration_list_node *declaration_list_node_ptr;
void visit(parse_tree_node *p,int pr);

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

//	yydebug=1;

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

	declaration_list_node_ptr = (declaration_list_node *)parse_root;

	declaration_list_node_ptr  -> eval_semantic_rules();


   return (0);
}
