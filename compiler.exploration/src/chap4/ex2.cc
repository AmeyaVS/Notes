#include <stdio.h>
#include <stdlib.h>
#include <string.h>


extern FILE *yyin;
extern int yyparse();

extern char input_str[];

int
main (int argc, char **argv)
{
   int ret;

   if (argc != 2) {
      printf ("Usage: %s 'C Statement' \n", argv[0]);
      return (1);
   }

   strcpy (input_str, argv[1]);

   ret = yyparse ();

   if (ret == 0) {
      printf ("SYNTAX CORRECT \n");
   } else {
      printf ("SYNTAX INCORRECT \n");
   }

   return (0);
}
