#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern char yytext[];
extern int column;
extern int yydebug;
char input_str[500];

int
yyerror (char *s)
{
   printf ("%s", input_str);
   fflush (stdout);
   printf ("\n%*s\n%*s\n", column, "^", column, s);
   return (1);
}

int
main (int argc, char **argv)
{
   yydebug = 1;
   int yyparse ();
   int result;

   if (argc != 2) {
      printf ("Usage: %s 'C Statement' \n", argv[0]);
      exit (1);
   }

   strcpy (input_str, argv[1]);

   result = yyparse ();

   if (result == 0) {
      printf ("%s", input_str);
      printf ("\nSYNTAX CORRECT \n");
   } else {
      printf ("SYNTAX INCORRECT \n");
   }

   return (0);
}
