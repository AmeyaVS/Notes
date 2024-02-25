#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "c-stmt-hdr.h"
#include "prod_entry.h"
#include "c-stmt-lr-parse.h"

/*
Grammar
rule 1    c_statement -> IDENTIFIER EQ_TO_OP c_expression SEMI_COLON
rule 2    c_expression -> CONSTANT
rule 3    c_expression -> IDENTIFIER
rule 4    c_expression -> c_expression OPERATOR CONSTANT
rule 5    c_expression -> c_expression OPERATOR IDENTIFIER
*/

#define	c_statement 1
#define	c_expression 2
#define	EOI 0

extern int yyparse ();

prod_entry prod_table[MAX_PROD_TAB_SIZE];
int action_table[MAX_TAB_SIZE][MAX_TAB_SIZE];
int goto_table[MAX_TAB_SIZE][MAX_TAB_SIZE];

char *tname[] = {
   "SEMI_COLON",
   "EQ_TO_OP",
   "CONSTANT",
   "IDENTIFIER",
   "OPERATOR",
   "c_statement",
   "c_expression"
};

#define fill_in(a,b,c)  \
	prod_table[a].rule_no = a;\
	prod_table[a].lhs_elem = b;\
	prod_table[a].no_of_elem_in_rhs = c;


void init_prod_entry ();


void
init_prod_entry ()
{
   fill_in (1, c_statement, 4);
   fill_in (2, c_expression, 1);
   fill_in (3, c_expression, 1);
   fill_in (4, c_expression, 3);
   fill_in (5, c_expression, 3);
}

void
init_action_table ()
{
   action_table[0][IDENTIFIER] = 1;
   action_table[1][EQ_TO_OP] = 2;
   action_table[2][IDENTIFIER] = 3;
   action_table[2][CONSTANT] = 4;
   action_table[5][OPERATOR] = 6;
   action_table[5][SEMI_COLON] = 7;
   action_table[6][IDENTIFIER] = 8;
   action_table[6][CONSTANT] = 9;

   action_table[3][SEMI_COLON] = -3;
   action_table[3][OPERATOR] = -3;
   goto_table[2][c_expression] = 5;

   action_table[4][SEMI_COLON] = -2;
   action_table[4][OPERATOR] = -2;

   action_table[8][SEMI_COLON] = -5;
   action_table[8][OPERATOR] = -5;

   action_table[9][SEMI_COLON] = -4;
   action_table[9][OPERATOR] = -4;


   action_table[7][EOI] = -1;
   goto_table[0][c_statement] = 10;

   action_table[10][EOI] = ACCEPT;


}

char input_str[500];

int
main (int argc, char **argv)
{
   int result;

   if (argc != 2) {
      printf ("Usage: %s 'C Statement' \n", argv[0]);
      exit (1);
   }
   strcpy (input_str, argv[1]);

   init_action_table ();
   init_prod_entry ();
   result = yyparse ();

   if (result == 0) {
      printf ("%s", input_str);
      printf ("\nSYNTAX CORRECT \n");
   } else {
      printf ("SYNTAX INCORRECT \n");
   }
   return (0);
}
