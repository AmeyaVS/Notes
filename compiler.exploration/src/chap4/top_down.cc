#include <stdio.h>
#include "top_down.h"
#include "stl.h"

int yylex (void);
extern FILE *yyin;

#define SUCCESS   1
#define FAILURE   0

int current_token;
char current_token_string[200];
extern char *yytext;
char input_str[2048];

/* prototypes */
int sentence();
int expr(int &expr_place);
int term(int &term_place);
int factor(int &factor_place);
int expr_rest(int expr_rest_i, int &expr_rest_s);
int term_rest(int term_rest_i, int &term_rest_s);


int
match (int expected)
{

   if (expected == current_token) {
      current_token = yylex ();
		strcpy(current_token_string,yytext);
      return (SUCCESS);
   }
	//printf("expected =%d got=%d \n",expected,current_token);
   return (FAILURE);
}


int sentence()
{
	int expr_place;

	if(expr(expr_place) == SUCCESS ){
		if(match(0) ){
			printf("result=%d \n",expr_place);
			return(SUCCESS);
		}
	}
	return(FAILURE);
}

/*
       (5) expr --> term { expr_rest.i = term.place }
	         expr_rest { expr.place = expr_rest.s }
*/
int expr(int &expr_place)
{
	int term_place; /* term.place */
	int expr_rest_s; /* expr_rest.s */
	int expr_rest_i; /* expr_rest.i */


	if(term(term_place) == SUCCESS ){
		expr_rest_i = term_place;
		if(expr_rest(expr_rest_i,expr_rest_s) == SUCCESS ){
			expr_place = expr_rest_s;
			return(SUCCESS);
		}
	}

	return(FAILURE);
}

/*
       (6) expr_rest --> + term { place = newtemp();
                               emit(place,":=",expr_rest.i,"+",term.place);
                               expr_rest1.i = place
                               }
	                 expr_rest1 { expr_rest.s = expr_rest1.s }

       (7) expr_rest --> - term { place = newtemp();
                               emit(place,":=",expr_rest.i,"-",term.place);
                               expr_rest1.i = place
                               }
	                 expr_rest1 { expr_rest.s = expr_rest1.s }

       (8) expr_rest --> epsilon { expr_rest.s expr_rest.i }

*/

int expr_rest(int expr_rest_i,int &expr_rest_s)
{
	int term_place;
	int expr_rest1_i;
	int expr_rest1_s;

	int op;

	if(match('+')){
		op='+';
	}else if(match('-')){
		op = '-';
	}else {
		expr_rest_s=expr_rest_i;
		return(SUCCESS);
	}

	if(term(term_place) == SUCCESS ){
		if(op == '+' ){
			expr_rest1_i = expr_rest_i + term_place ;
		}else{
			expr_rest1_i = expr_rest_i - term_place ;
		}

		if(expr_rest(expr_rest1_i,expr_rest1_s) == SUCCESS ){
			expr_rest_s = expr_rest1_s;
			return(SUCCESS);
		}
	}

	return(FAILURE);
}

/*
        (9) term --> factor { term_rest.i = factor.place }
	         term_rest { term.place = term_rest.s }
*/
int term(int &term_place)
{
	int factor_place; /* factor.place */
	int term_rest_i; /* term_rest.i */
	int term_rest_s; /* term_rest.s */

	if(factor(factor_place) == SUCCESS ){
		term_rest_i = factor_place;
		if(term_rest(term_rest_i,term_rest_s) == SUCCESS ) {
			term_place = term_rest_s;
			return(SUCCESS);
		}
	}
	return(FAILURE);
}

/*
       (10) term_rest --> * 
                      factor { place = newtemp();
                               emit(place,":=",term_rest.i,"*",factor.place);
                               term_rest1.i = place
                             }
                     term_rest1 { term_rest.s = term_rest1.s ; }

       (11) term_rest --> / 
                      factor { place = newtemp();
                               emit(place,":=",term_rest.i,"/",factor.place);
                               term_rest1.i = place
                             }
                     term_rest1 { term_rest.s = term_rest1.s ; }

       (12) term_rest --> epsilon { term_rest.s = term_rest.i }

*/

int term_rest(int term_rest_i, int &term_rest_s)
{
	int factor_place;
	int term_rest1_s;
	int term_rest1_i;

	int op;

	if(match('*')){
		op='*';
	}else if(match('/')){
		op = '/';
	}else {
		term_rest_s = term_rest_i;
		return(SUCCESS);
	}

   if(factor(factor_place) == SUCCESS ){

		if(op == '*' ){
			term_rest1_i = term_rest_i * factor_place;
		}else{
			term_rest1_i = term_rest_i / factor_place;
		}
			
		if(term_rest(term_rest1_i,term_rest1_s) == SUCCESS ){
			term_rest_s = term_rest1_s;
			return(SUCCESS);
		}
	}


	return(FAILURE);
}

/*
       (13) factor --> IDENTIFIER  { factor.place = IDENTIFIER.place }

       (14) factor --> CONSTANT { place = newtemp();
			      emit(place,":=",CONSTANT.value)
			      factor.place = place
                            }

       (15) factor --> '(' expr ')' { place = newtemp();
                                  emit(place,":=",expr.place)
                                  factor.place = place
                                }

*/

int factor(int &factor_place)
{
	int CONSTANT_val;
	int expr_place;

	if(current_token == CONSTANT ){
		CONSTANT_val = atoi(yytext);
		factor_place = CONSTANT_val;
		match(CONSTANT);
		return(SUCCESS);
	} else if(match('(') ){
		if(expr(expr_place) == SUCCESS ){
			factor_place = expr_place;
			if(match(')') ){
				return(SUCCESS);
			}
		}
	}

	return(FAILURE);
}



int
yyparse ()
{
   current_token = yylex ();
	strcpy(current_token_string,yytext);

   if (sentence () != SUCCESS) {
      return (1);
   }

	return(0);

}
