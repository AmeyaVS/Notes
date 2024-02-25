#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include "rlex.h"

/*
 *
 * This example generation of code from a Lex rules file
 * by the rlex class
 */
void Usage(char *str)
{
   printf ("Usage :%s [-v] <file name>\n", str);
	printf ("\t -v for verbose mode \n");
	exit (1);
}

int
main (int argc, char **argv)
{
  rlex *my_lexer;

  FILE *fp;
  char file_name[500];

  int verbose=0;
  int opt;

	while ((opt = getopt (argc, argv, "v")) != EOF) {
      switch (opt) {
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



  strcpy (file_name, argv[optind]);

  if ((fp = fopen (file_name, "r")) == NULL) {
    printf ("[%s] file does not exist\n", file_name);
    exit (1);
  }

  my_lexer = new rlex;
  my_lexer->machine (fp);

  if(verbose){
  		cout << "#ifdef VERBOSE_INFO" << endl;
  		my_lexer->print_nfa_tree ();
  		cout << "#endif" << endl;
  }

  my_lexer->convert_nfa2dfa ();
  my_lexer->minimise_dfa ();

  if(verbose){
  		cout << "#ifdef VERBOSE_INFO" << endl;
  		my_lexer->print_min_dfa_tree ();
  		cout << "#endif" << endl;
  }

  my_lexer->gen_code ();
}
