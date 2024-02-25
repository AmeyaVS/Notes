#include <unistd.h>
#include <stdio.h>
#include "rlex.h"


/*
 * This example demonstrates the Conversion of Regular
 * Expression into a NFA by the rlex class
 */

int
main (int argc, char **argv)
{
  char reg_str[200];
  rlex *my_lexer;

  if (argc != 2) {
    printf ("Usage : %s 'Regular Expression' \n", argv[0]);
    exit (1);
  }
  strcpy (reg_str, argv[1]);

  my_lexer = new rlex;
  my_lexer->convert_re2nfa (reg_str);
  my_lexer->print_nfa_tree ();
}
