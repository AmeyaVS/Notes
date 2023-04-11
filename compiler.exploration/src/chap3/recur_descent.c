#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "c-stmt-hdr.h"

int yylex (void);

#define SUCCESS	1
#define FAILURE	0

int c_statement ();
int c_expression ();
int c_expr_rest ();
int c_expr_factor ();

int current_token;
extern char *yytext;

int
match (int expected)
{

   if (expected == current_token) {
      current_token = yylex ();
      return (SUCCESS);
   }
   return (FAILURE);
}

int
yyparse ()
{
   current_token = yylex ();
   if (c_statement () == SUCCESS) {
      return (0);
   } else {
      return (1);
   }
}

/*
 * c_statement ->IDENTIFIER EQ_TO_OP c_expression ';'
 */
int
c_statement ()
{

   printf
      ("c_statement -> IDENTIFIER EQ_TO_OP c_expression SEMI_COLON\n");
   if (match (IDENTIFIER)) {
      if (match (EQ_TO_OP)) {
         if (c_expression () == SUCCESS) {
            if (match (SEMI_COLON)) {
               return (SUCCESS);
            }
         }
      }
   }
   printf ("Error ");
   return (FAILURE);
}

/*   
   c_expression -> CONSTANT   c_expr_rest
                   IDENTIFIER c_expr_rest
*/

int
c_expression ()
{

   if (match (CONSTANT)) {
      printf ("c_expression -> CONSTANT c_expr_rest\n");
      if (c_expr_rest () == SUCCESS) {
         return (SUCCESS);
      }
   } else if (match (IDENTIFIER)) {
      printf ("c_expression -> IDENTIFIER c_expr_rest\n");
      if (c_expr_rest () == SUCCESS) {
         return (SUCCESS);
      }
   }
   printf ("Error ");
   return (FAILURE);
}

/*   
   c_expr_rest ->  OPERATOR   c_expr_factor
                   EPSILON
*/

int
c_expr_rest ()
{

   if (match (OPERATOR)) {
      printf ("c_expr_rest -> OPERATOR c_expr_factor\n");
      if (c_expr_factor () == SUCCESS) {
         return (SUCCESS);
      }
   } else {
      printf ("c_expr_rest -> EPSILON\n");
      return (SUCCESS);
   }
   printf ("Error ");
   return (FAILURE);
}

/*   
   c_expr_factor ->  IDENTIFIER   c_expr_rest
                     CONSTANT   c_expr_rest
*/

int
c_expr_factor ()
{
   if (match (IDENTIFIER) == SUCCESS) {
      printf ("c_expr_factor -> IDENTIFIER c_expr_rest\n");
      if (c_expr_rest () == SUCCESS) {
         return (SUCCESS);
      }
   } else if (match (CONSTANT) == SUCCESS) {
      printf ("c_expr_factor -> CONSTANT c_expr_rest\n");
      if (c_expr_rest () == SUCCESS) {
         return (SUCCESS);
      }
   }
   printf ("Error ");
   return (FAILURE);
}

char input_str[500];
int
main (int argc, char **argv)
{
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
      printf ("\nSYNTAX INCORRECT \n");
   }

   return (0);
}
