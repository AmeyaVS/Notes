#include <unistd.h>
#include <stdio.h>
#include "rlex.h"

void Usage(char *str)
{
	printf ("Usage :%s [-v] <Regular Expression> <file name>\n", str);
	printf ("\t -v for verbose mode \n");
	exit (1);
}

int
main (int argc, char **argv)
{
  char reg_str[200];
  rlex *my_lexer;

  FILE *fp;
  char str[500];
  char file_name[500];
  int line_no = 1;
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

   if ((argc-optind) != 2) {
		Usage(argv[0]);
   }


  strcpy (reg_str, argv[optind]);
  strcpy (file_name, argv[optind+1]);


  if ((fp = fopen (file_name, "r")) == NULL) {
    printf ("[%s] file does not exist\n", file_name);
    exit (1);
  }

  my_lexer = new rlex;
  my_lexer->convert_re2nfa (reg_str);

  if(verbose == 1){
  	my_lexer->print_nfa_tree ();
  }

  while (fgets (str, sizeof (str), fp) != NULL) {
    if (my_lexer->interpret_nfa (str,verbose)) {
      printf ("%d:%s", line_no, str);
    }
    line_no++;
  }
}
