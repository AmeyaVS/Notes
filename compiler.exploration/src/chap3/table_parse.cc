#include <stdio.h>
#include <stdarg.h>
#include "stl.h"
#include "c-stmt-hdr.h"
#include "parse_tab.h"

extern "C"
{
   extern int yylex ();
   extern char *yytext;
};


parse_tab_entry M[MAX_PARSE_TAB_SIZE][MAX_PARSE_TAB_SIZE];
int start_symbol;

int get_next_tok();

#define error() { printf("Error\n"); return(-1); }

int
tbl_driven_parse ()
{
   int i;
   parse_tab_entry tmp;
   stack < int >stk;

   int X, a;

   /* Push the '$' and the Start Symbol on the Stack */
   stk.push ('$');
   stk.push (start_symbol);

   a = get_next_tok ();

#ifdef	DEBUG
   printf ("first Token=%d (%s) \n", a, yytext);
#endif
   do {
      X = stk.top ();
#ifdef	DEBUG
      printf ("X=%d a=%d \n", X, a);
#endif

		if( (X == '$') || ( a == '$' ) ){
			if( X == a ){ 
				return(0);
			}else{
				error();
			}
		}


		/* X is not $ and a is also not $ */

      if (X > 255) {    /* Terminal */
         if (X == a) {
            stk.pop ();
            a = get_next_tok ();
#ifdef	DEBUG
            printf ("Next Token=%d (%s) \n", a, yytext);
#endif
         } else {
            error ();
         }
      } else {
         tmp = M[X][a];
         if (tmp.prod_lhs != 0) {       /* valid entry */
            stk.pop ();
            printf ("%s\n", tmp.prod_string);
            for (i = (tmp.no_of_elem_in_rhs - 1); i >= 0; i--) {
               stk.push (tmp.prod_rhs[i]);
            }
         } else {
            error ();
         }
      }

   } while (1);

}

int get_next_tok()
{
	int a;

	a = yylex();
	if( a == 0 ){
		a = '$';
	}
	return(a);
}


void
fill_in_parsing_tab ()
{

   /* C_STMT -> IDENTIFIER EQ_TO_OP C_EXPR SEMI_COLON */
   M[C_STMT][IDENTIFIER] =
      fill_in_tab_entry (5,
                         "c_statement -> IDENTIFIER EQ_TO_OP c_expression SEMI_COLON",
                         C_STMT, IDENTIFIER, EQ_TO_OP, C_EXPR,
                         SEMI_COLON);

   /* C_EXPR -> CONSTANT C_EXPR_REST */
   M[C_EXPR][CONSTANT] =
      fill_in_tab_entry (3,
                         "c_expression -> CONSTANT c_expr_rest",
                         C_EXPR, CONSTANT, C_EXPR_REST);

   /* C_EXPR -> IDENTIFIER C_EXPR_REST */
   M[C_EXPR][IDENTIFIER] =
      fill_in_tab_entry (3,
                         "c_expression -> IDENTIFIER c_expr_rest",
                         C_EXPR, IDENTIFIER, C_EXPR_REST);

   /* C_EXPR_REST -> OPERATOR C_EXPR_FACTOR */
   M[C_EXPR_REST][OPERATOR] =
      fill_in_tab_entry (3,
                         "c_expr_rest -> OPERATOR c_expr_factor",
                         C_EXPR_REST, OPERATOR, C_EXPR_FACTOR);

   /* C_EXPR_REST -> EPSILON */
   M[C_EXPR_REST][SEMI_COLON] =
      fill_in_tab_entry (1, "c_expr_rest -> EPSILON", C_EXPR_REST);

   /* C_EXPR_FACTOR -> IDENTIFIER C_EXPR_REST */
   M[C_EXPR_FACTOR][IDENTIFIER] =
      fill_in_tab_entry (3,
                         "c_expr_factor -> IDENTIFIER c_expr_rest",
                         C_EXPR_FACTOR, IDENTIFIER, C_EXPR_REST);

   /* C_EXPR_FACTOR -> CONSTANT C_EXPR_REST */
   M[C_EXPR_FACTOR][CONSTANT] =
      fill_in_tab_entry (3,
                         "c_expr_factor -> CONSTANT c_expr_rest",
                         C_EXPR_FACTOR, CONSTANT, C_EXPR_REST);

	/* start Symbol */
   start_symbol = C_STMT;
}

parse_tab_entry
fill_in_tab_entry (int count, ...)
{
   va_list ptr;
   int num;
   int i;
   char *prule;
   parse_tab_entry tmp;

   va_start (ptr, count);

   prule = va_arg (ptr, char *);

   strcpy (tmp.prod_string, prule);

   tmp.prod_lhs = va_arg (ptr, int);

   count--;
   for (i = 0; i < count; i++) {
      num = tmp.prod_rhs[i] = va_arg (ptr, int);
   }
   tmp.no_of_elem_in_rhs = count;
   va_end (ptr);
   return (tmp);
}
