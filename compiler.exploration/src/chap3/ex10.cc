#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include "stl.h"
#include "canonical_lr.h"

char input_str[200];
int done;

extern int yyparse();



int
main (int argc, char **argv)
{
   canonical_lr *G;
   char file_name[MAX_FILE_NAME];

   if (argc != 2) {
      printf ("Usage: %s <Grammar-File> \n", argv[0]);
      exit (1);
   }
   strcpy (file_name, argv[1]);


   /* Fill in the Grammar Table */
   G = new canonical_lr (file_name);
   G->init_all_tables ();

	/*
   printf ("**** Production Table **** \n");
   G->print_productions ();

   printf ("\n**** Symbol Table **** \n");
   G->print_symbol_table ();
	*/

	 G -> create_augmented_grammar();

   G-> build_canonical_collection ();

   G->print_canonical_sets ();

   G -> construct_canonical_parsing_table();

	return(0);
}
