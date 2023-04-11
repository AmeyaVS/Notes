#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include "stl.h"
#include "c-stmt-hdr.h"
#include "grammar.h"


extern parse_tab_entry M[MAX_PARSE_TAB_SIZE][MAX_PARSE_TAB_SIZE];

void generate_parsing_tab (char *file_name);
extern int start_symbol;
extern int tbl_driven_parse ();
char input_str[500];

int
main (int argc, char **argv)
{
   char file_name[MAX_FILE_NAME];
   int result;

   if (argc != 3) {
      printf ("Usage: %s <Grammar-File> 'C Statement' \n", argv[0]);
      exit (1);
   }
   strcpy (file_name, argv[1]);
   strcpy (input_str, argv[2]);

   generate_parsing_tab (file_name);

   printf ("\n**** Parsing **** \n");
   start_symbol = C_STMT;
   result = tbl_driven_parse ();

   if (result == 0) {
      printf ("%s", input_str);
      printf ("\nSYNTAX CORRECT \n");
   } else {
      printf ("SYNTAX INCORRECT \n");
   }

}

void
generate_parsing_tab (char *file_name)
{

   grammar *G;

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

   printf ("\n**** Table Entries **** \n");
   G->gen_parsing_tab_entries (M);

}
