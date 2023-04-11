#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>




extern FILE *yyin;
extern int yydebug;

extern void	init_global_sym_tab();
extern void print_sym_tables();


extern void generate_target_code();
extern int optimize_intermediate_code(int optimize_flags,int verbose);
extern int generate_optimized_target_code();
extern void print_tac();

extern int sem_err;
extern void	set_example_flag(int flg);

#ifdef CHAP7_EX1
#define FLAG 0x0701
#elif CHAP7_EX2
#define FLAG 0x0702
#else 
#define FLAG 0x0000
#endif

#define LOCAL  0x01

extern int get_optim(char *optimi_str);

void Usage(char *str)
{
	printf ("Usage: %s [-i ] [-O opt_type ] <input file> \n", str);
	printf ("\t -i for Intermediate Code generation Only  \n");
	printf ("\t opt_type=[local|gcse|gcp|gcm|gpo|all] \n");
	printf ("\t          local for Local optimization\n");
	printf ("\t          gcse  for Global Common Subexpression Elimination\n");
	printf ("\t          gdce  for Global Dead Code Elimination \n");
	printf ("\t          gcp   for Global Copy Propagation \n");
	printf ("\t          gcm   for Global Code Motion\n");
	printf ("\t          gpo   for Global Peephole Optimization \n");
	printf ("\t          all   for All the above Optimizations \n");
	exit(1);
}


int
main (int argc, char **argv)
{
   int yyparse ();
   int result;
	int ich;
	int intermediate_code_only = 0;
	int verbose=0;

	FILE *fp;

	while ((ich = getopt (argc, argv, "iO:vt")) != EOF) {
      switch (ich) {
         case 'i': intermediate_code_only=1;
              break;
         case 'v': verbose=1;
              break;
         default:
               printf("unknown switch\n");
					Usage(argv[0]);
               break;
      }
   }

   if ((argc-optind) != 1) {
		Usage(argv[0]);
   }


//	 yydebug=1;
	

   fp=fopen(argv[optind],"r");

	if(fp == NULL ){
		printf("The file [%s] does not exist or is not readable\n",argv[1]);
		exit(1);
	}

	yyin=fp;

	init_global_sym_tab();

	/* 
	Parse the input code 
	also generates Intermediate Code 
	*/
   result = yyparse ();

   if (result == 0) {
   } else {
      printf ("SYNTAX INCORRECT \n");
		return(0);
   }

	if(sem_err){
		printf("Semantic Errors in the Program\n");
		return(0);
	}

	/* Optimize IC , if there are options for it */
	if(verbose ==1 ){
		print_tac();
	}

	if(intermediate_code_only){
		/* If it is already verbose..dont print IC */
		if(verbose !=1 ){
			print_tac();
		}
		return(0);
	}


	/* Target Code generation */
	generate_target_code();

}
