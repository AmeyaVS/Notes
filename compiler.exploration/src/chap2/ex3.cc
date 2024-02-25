#include <unistd.h>
#include <stdio.h>
#include "rlex.h"


/*
 * This example demonstrates the Conversion of NFA
 * into a DFA by the rlex class
 */
void Usage(char *str)
{
	printf ("Usage :%s [-v] <Regular Expression> \n", str);
	printf ("\t -v for verbose mode \n");
	exit (1);
}

int
main (int argc, char **argv)
{
  char reg_str[200];
  rlex *my_lexer;
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


  strcpy (reg_str, argv[optind]);

  my_lexer = new rlex;
  my_lexer->convert_re2nfa (reg_str);
  my_lexer->print_nfa_tree ();
  my_lexer->convert_nfa2dfa ();
  my_lexer->print_dfa_tree (verbose);
}
