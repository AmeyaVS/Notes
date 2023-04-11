#include <stdio.h>
#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include "stl.h"
#include "c-stmt-hdr.h"
#include "prod_entry.h"
#include "c-stmt-lr-parse.h"


int yyparse ();
int get_next_token ();

extern "C"
{
   extern int yylex ();
   extern char *yytext;
	extern int column;
}

#ifdef CHAP3_EX8

#define error() { \
		printf("Error not finding entry action_table[%d][%d]..Exiting\n",tos,input);  \
   		printf ("%s", input_str); \
   		fflush (stdout); \
   		printf ("\n%*s\n", column, "^"); \
			return(1);  \
			}
#elif CHAP3_EX12
int error_handler(int tos,int input) ;
#endif


int
yyparse ()
{
   stack < int >stk;
   int tos =
      0, input, next_state, prod, lhs_of_prod, no_of_elem_in_rhs, i;
   int tmp;

   stk.push (0);
   tos = stk.top ();
   printf ("Entering state 0\n");
   input = get_next_token ();

   while (action_table[tos][input] != ACCEPT) {

      if (action_table[tos][input] == ERROR) {
#ifdef CHAP3_EX8
         error ();
#elif CHAP3_EX12
         if(!(next_state=error_handler (tos,input))){
   			printf ("%s", input_str);
   			fflush (stdout);
   			printf ("\n%*s\n", column, "^");
//   			printf ("\n%*s\n%*s\n", column, "^", column, s);
				return(1);
			}
         stk.push (next_state);
#endif
      } else if ((next_state = action_table[tos][input]) > 0) { /* shift */
         stk.push (next_state);
         tmp = input ? (input - TOK_BASE) : input;
         printf ("Shifting token %d (%s), Entering state %d \n",
                 input, tname[tmp], next_state);
         input = get_next_token ();
      } else if ((prod = action_table[tos][input]) < 0) {       /* Reduce */
         prod = -(prod);
         printf ("Reducing via Rule %d \n", prod);
         lhs_of_prod = prod_table[prod].lhs_elem;
         no_of_elem_in_rhs = prod_table[prod].no_of_elem_in_rhs;
         for (i = 0; i < no_of_elem_in_rhs; i++) {
            stk.pop ();
         }
         tos = stk.top ();
         printf ("Entering state %d\n", goto_table[tos][lhs_of_prod]);
         stk.push (goto_table[tos][lhs_of_prod]);
      }
      tos = stk.top ();
/*
		if(input==0 ){
			return(1);
		}
*/
   }
   printf ("Success\n");
   return (0);
}

int
get_next_token ()
{
   int next_tok;

   next_tok = yylex ();

   printf ("Reading a token: ");
   if (next_tok == 0) {
      printf ("Now at end of input.\n");
      return (0);
   }

   printf ("Next token is %d (%s)\n", next_tok,
           tname[next_tok - TOK_BASE]);
   return (next_tok);
}

int error_handler(int tos,int input) 
{ 
	int state=tos,new_state;
	printf("Error not finding entry action_table[%d][%d]..\n",tos,input); 	  
	switch(state)
	{
		case 0 : printf("%s","Expecting IDENTIFIER" ); 

					/* Attempting recovery */
					new_state=action_table[0][IDENTIFIER];

					/* making sure that the input would be absobed */
					if(action_table[new_state][input] != ERROR ){
						printf(" (Inserted it)\n");
						return(new_state);
					}
					break;
		case 1 : printf("%s","Expecting EQ_TO_OP" );

					/* Attempting recovery */
					new_state=action_table[1][EQ_TO_OP];

					/* making sure that the input would be absobed */
					if(action_table[new_state][input] != ERROR ){
						printf(" (Inserted it)\n");
						return(new_state);
					}
					break;

		case 2 :
		case 6 :printf("%s","Expecting IDENTIFIER or CONSTANT" );
				  break;
		case 3:
		case 4:
		case 5:
		case 8:
		case 9:
				  printf("%s","Expecting SEMI_COLON or OPERATOR" );
				  break;

		case 7:
		case 10:
		        printf("%s","Expecting EOI" );
				  break;
	}
	printf("\n");
	return(0); 
}
