#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>




extern FILE *yyin;
extern int yydebug;

extern void init_global_sym_tab();
extern void print_global_sym_tab();

extern int sem_err;




int
main (int argc, char **argv)
{
   int yyparse ();
   int result;

	FILE *fp;

   if (argc != 2) {
      printf ("Usage: %s <input file> \n", argv[0]);
      exit (1);
   }



//	 yydebug=1;

   fp=fopen(argv[optind],"r");

	if(fp == NULL ){
		printf("The file [%s] does not exist or is not readable\n",argv[1]);
		exit(1);
	}

	yyin=fp;

	init_global_sym_tab();

   result = yyparse ();

   if (result != 0) {
      printf ("SYNTAX INCORRECT \n");
		return(0);
   }

	if(sem_err){
		printf("Semantic Errors in the Program \n");
	}else{
		printf("Semantically correct Program \n");
	}

	return(0);

}
