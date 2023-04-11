#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char input_str[500];
int done = 0;
int
main (int argc, char **argv)
{
   int yyparse ();
   int result;

   if (argc != 2) {
      printf ("Usage: %s 'Expression' \n", argv[0]);
		printf("argc=%d \n",argc);
      exit (1);
   }

   strcpy (input_str, argv[1]);

   result = yyparse ();

   if (result == 0) {
      printf ("%s", input_str);
      printf ("\nSYNTAX CORRECT \n");
   } else {
      printf ("\nSYNTAX INCORRECT \n");
   }

   return (0);
}

