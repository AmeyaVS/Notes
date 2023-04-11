#include "grammar.h"

int
main (int argc, char **argv)
{

   grammar *G;
   char file_name[MAX_FILE_NAME];

   /* Parse the command line to get the file name */
   if (argc != 2) {
      printf ("Usage :%s <Grammar file name>\n", argv[0]);
      exit (1);
   }

   strcpy (file_name, argv[1]);

   /* Fill in the Grammar Table */
   G = new grammar (file_name);
   G->fill_in_grammar_table ();

   /* Calculate the FIRST set */
   G->calculate_FIRST ();

   /* Calculate the FOLLOW set */
   G->calculate_FOLLOW ();

	/*
   printf ("**** Production Table **** \n");
   G->print_productions ();

   printf ("\n**** Symbol Table **** \n");
   G->print_symbol_table ();
	*/

	G->print_FIRST_FOLLOW_sets();


}
