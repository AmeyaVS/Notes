#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern void evaluate_semantic_rules();
extern void create_dep_graph();
extern int print_result();

extern char input_str[];



int
main (int argc, char **argv)
{
   int yyparse ();
   int result;

   if (argc != 2) {
      printf ("Usage: %s 'C Statement' \n", argv[0]);
      return (1);
   }

   strcpy (input_str, argv[1]);

   result = yyparse ();

   if (result == 0) {
      printf ("SYNTAX CORRECT \n");
   } else {
      printf ("SYNTAX INCORRECT \n");
		return(0);
   }

	create_dep_graph();

	evaluate_semantic_rules();

	print_result();

   return (0);
}
