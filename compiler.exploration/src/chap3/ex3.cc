#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "parse_tab.h"



char input_str[500];

int
main (int argc, char **argv)
{
   int result;

   if (argc != 2) {
      printf ("Usage: %s 'C Statement' \n", argv[0]);
      exit (1);
   }
   strcpy (input_str, argv[1]);

   fill_in_parsing_tab ();

   result = tbl_driven_parse ();

   if (result == 0) {
      printf ("%s", input_str);
      printf ("\nSYNTAX CORRECT \n");
   } else {
      printf ("SYNTAX INCORRECT \n");
   }

}
