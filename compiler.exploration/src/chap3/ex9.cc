#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include "stl.h"
#include "slr.h"


char input_str[200];

extern int yyparse();
int
main (int argc, char **argv)
{
   slr *G;
   char file_name[MAX_FILE_NAME];

   if (argc != 2 ) {
      printf ("Usage: %s <Grammar-File> \n", argv[0]);
      exit (1);
   }
   strcpy (file_name, argv[1]);


   /* Fill in the Grammar Table */
   G = new slr (file_name);
   G->init_all_tables ();

   printf ("**** FIRST and FOLLOW sets **** \n");
	G->print_FIRST_FOLLOW_sets();

	G -> create_augmented_grammar();

   G->build_canonical_collection ();

   G->print_canonical_sets ();

	G -> construct_parsing_table ();

   return (0);

}
