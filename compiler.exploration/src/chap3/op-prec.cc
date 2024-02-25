#include <stdio.h>
#include <iostream>
#include <deque>
#include <stack>
#include "op-prec.h"

using std::deque;
using std::stack;
using std::cout;

#define	ERR1	-1
#define	ERR2	-2
#define	ERR3	-3
#define	ERR4	-4

extern "C"
{
   extern int yylex ();
   extern char *yytext;
};

typedef deque < int> my_deque;

void print_vect( my_deque &v);
int validate_popped_elem ( my_deque &v);
void print_err_msg(int no);


int O[MAX_PARSE_TAB_SIZE][MAX_PARSE_TAB_SIZE] =
{
	/* +     -     *     /     ^    ID     (     )    $   */
	{ '>' , '>' , '<' , '<' , '<' , '<' , '<' , '>' , '>'  }, /* Add + */
	{ '>' , '>' , '<' , '<' , '<' , '<' , '<' , '>' , '>'  }, /* Sub - */
	{ '>' , '>' , '>' , '>' , '<' , '<' , '<' , '>' , '>'  }, /* Mul * */
	{ '>' , '>' , '>' , '>' , '<' , '<' , '<' , '>' , '>'  }, /* Div / */
	{ '>' , '>' , '>' , '>' , '<' , '<' , '<' , '>' , '>'  }, /* Pow ^ */
	{ '>' , '>' , '>' , '>' , '>' , ERR3, ERR3, '>' , '>'  }, /* ID */
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , '=' , ERR4 }, /* LP */
	{ '>' , '>' , '>' , '>' , '>' , ERR3, ERR3, '>' , '>'  }, /* RP */
	{ '<' , '<' , '<' , '<' , '<' , '<' , '<' , ERR2, ERR1 }  /* $ EOI */

};

char *tname [] ={
	" ",
	"+",
	"-",
	"*",
	"/",
	"^",
	"ID",
	"(",
	")",
	"EOI",
	"E"
};


int op_prec(int a, int b)
{
	return(O[a-1][b-1]); 
}

int
yyparse ()
{
	int a,b;
	int val;
	int tos,most_recently_popped;
	int no_of_terminals_popped = 0 ;
   stack < int >stk;
	my_deque elements_popped;

	stk.push(EOI);
	b = yylex();
	a=stk.top();

	do{


		if( (a == EOI ) && ( b == EOI ) ){
			return(0);
		}

		val=op_prec(a,b);

		if( (val == '<' ) ||
				( val == '=' ) ){ /* Shift */
			stk.push(b);
			printf("Shifting %s [%s] \n",tname[b],yytext); 
			a = b;
			b = yylex();
			if(b == 0 ){
				b=EOI;
			}
	
		} else if ( val == '>' ) { /* Reduce */

			elements_popped.clear();

			no_of_terminals_popped = 0 ;
			do{
					tos = stk.top();

					if( tos == NONTERM ){
						stk.pop();
						elements_popped.push_front(tos);
						continue;
					}

					/* Terminal */
					if(no_of_terminals_popped){

						if( op_prec(tos,most_recently_popped) == '<' ){
							break;
						}
					}

					
					most_recently_popped= tos ;
					stk.pop();

					elements_popped.push_front(most_recently_popped);
					no_of_terminals_popped ++ ;


			} while( 1);

			a = tos;

			stk.push(E);

			if(validate_popped_elem(elements_popped)== 1 ){
				return(1);
			}


			cout << "Reducing E --> " ;
			print_vect(elements_popped);

		} else {
			print_err_msg(val);
			return(1);
		}
		
	}while(1);
}

void
print_vect( my_deque &v)
{
	int i,j,sz=v.size();

	for(i=0;i<sz;i++){
		j= (v[i]);
		printf(" %s", tname[j]);
	}
	printf("\n");
}

int
validate_popped_elem ( my_deque &v)
{
	int i,sz;
	int element,lhs,rhs;

	sz = v.size();

	for(i=0;i<sz;i++){

		element = v[i];
		lhs = (i==0) ?  NOT_EXIST : v[i-1] ;
		rhs = (i == (sz-1) ) ? NOT_EXIST : v[i+1] ;

		switch(element){

			case PLUS	:
			case MINUS	:
			case STAR	:
			case DIV	:
			case POW	:
							if( (lhs != NONTERM ) ||
								(rhs != NONTERM ) ){
								printf("Missing Operand \n");
								return(1);
							}
							break;
			case ID	:
							if( (lhs == NONTERM ) ||
								(rhs == NONTERM ) ){
								printf("Missing Operator \n");
								return(1);
							}
							break;
			case LP	:
			case RP	:
							if( (lhs == NONTERM ) ||
								(rhs == NONTERM ) ){
								printf("Missing Operator \n");
								return(1);
							}
							break;
			case EOI	:

			default :
							break;
		}
	}
	return(0);
}

void
print_err_msg(int no)
{
	
	switch(no)
	{
		case ERR1 :

						printf("Missing Operand \n");
						break;
		case ERR2 :
						printf("Unbalanced Paranthesis \n");
						break;
		case ERR3 :
						printf("Missing Operator \n");
						break;
		case ERR4 :
						printf("Missing Right Parenthesis \n");
						break;
	}
}
