#include <stdio.h>
#include <stdlib.h>

extern int yydebug;

int main(int argc, char **argv)
{
   int yyparse ();
   int result;
	FILE *fp;
	extern FILE *yyin;

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
      printf ("\nSYNTAX CORRECT \n");
   } else {
      printf ("SYNTAX INCORRECT \n");
   }
	return(0);
}
