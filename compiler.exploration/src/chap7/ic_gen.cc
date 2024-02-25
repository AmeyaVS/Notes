/*!
	@file ic_gen.cc
	@brief Intermediate Code Generation Source.
*/

#include <assert.h>

#include "ic_gen.h"

//! Place for storing all the quads 
vector <quad *>quad_arr;

//! Flag to tell if tac is valid
int tac_not_valid;

//! Pointer used by Target code generator for generating target code
vector <quad *> *final_quad_arr_ptr=&quad_arr;

//! Gives the count of number of quads
int next_quad;

/* backpatch.cc begin */

static void quadlist_append (quad_list *p3,quad_list *p1 );

//! Makes a quad list given an index of the quad
quad_list * makelist(int index)
{
	quad_list *p;

	p = new quad_list ;
	p -> push_back (index);

	return(p);
}

//! Merges the quadlists p1 and p2 and appends them into p3
void quadlist_merge (quad_list *p3,quad_list *p1 , quad_list *p2 )
{
	quadlist_append(p3,p1);
	quadlist_append(p3,p2);
}

//! Appends the quad list p1 into p3
void quadlist_append (quad_list *p3,quad_list *p1 )
{
	int q;
	quad_list :: iterator iter;

	iter = p1 -> begin();

	while ( iter != p1 -> end() ){
		q = (*iter);
		iter ++ ;
		p3 -> push_back(q);
	}
}

//! Backpatches all the quads in p with lbl
void backpatch(quad_list *p, sym_tab_entry *lbl )
{
	int q;

	quad *quad_ptr ;
	quad_list :: iterator iter;

	iter = p -> begin();

	while ( iter != p -> end() ){
		q = (*iter);
		quad_ptr = quad_arr[q];
		if(quad_ptr -> op == GOTO ){
			quad_ptr -> arg1 = lbl;
		}else{
			quad_ptr -> res = lbl;
		}
		iter ++ ;
	}

}

//! Prints the quads in the quad list p1
void quadlist_print (quad_list *p1 )
{
	int q;
	quad_list :: iterator iter;

	iter = p1 -> begin();

	while ( iter != p1 -> end() ){
		q = (*iter);
		printf("%d ..",q);
		iter ++ ;
	}
}



/* backpatch.cc end */

/* quad.cc begin */

//! Emit a quad given op,arg1,arg2 and res
quad *emit(oprtr op,sym_tab_entry *arg1,sym_tab_entry *arg2,sym_tab_entry *res)
{
	quad *quad_ptr;

	quad_ptr = new quad(op,arg1,arg2,res);
	quad_arr.push_back(quad_ptr);
	next_quad ++ ;
	return(quad_ptr);
}

//! Prints the Three Address Code (tac) from the global final_quad_arr_ptr
void print_tac()
{
	int n,i;
    int j;
	string print_str;

	if(tac_not_valid){
		return;
	}

	n = (*final_quad_arr_ptr).size();

    j=0;
	for(i=0;i<n;i++) {
		print_str = (*final_quad_arr_ptr)[i] -> as_string();
		printf("(%d) %s\n",j,print_str.c_str() );
        j++;
		if((*final_quad_arr_ptr)[i]-> op == PROC_END ){
			printf("\n");
            j=0;
		}
	}
}

//! Creates a Quad given the Operator,arg1,arg2,res
quad :: quad(oprtr opr, sym_tab_entry *argmt1,sym_tab_entry *argmt2,sym_tab_entry *result)
{
	op = opr;
	arg1 = argmt1;
	arg2 = argmt2;
	res = result;

	if( (opr  >= ADD) && ( opr <= GOTO) ){
	}else{
		printf("unknown operator=%d\n",opr);
	}
}

//! Creates a string that can be printed 
string 
quad ::as_string()
{

	string to_print;
	string ar1;
	string ar2;
	string re;

	if(arg1 != NULL ){
		ar1 = arg1->get_name();
	}

	if(arg2 != NULL ){
		ar2 = arg2->get_name();
	}

	if(res != NULL ){
		re = res->get_name();
	}

	switch(op)
	{
		case ASSIGN : 
		             to_print = re + " := " + ar1 ;
						 break;
		case ADD :
		             to_print = re + " := " + 
						              ar1 + " + " + ar2 ;
						 break;
		case MUL :
		             to_print = re + " := " + 
						               ar1 + " * " + ar2 ;
						 break;
		case DIV :
		             to_print = re + " := " + 
						               ar1 + " / " + ar2 ;
						 break;
		case SUB :
		             to_print = re + " := " + 
						               ar1 + " - " + ar2 ;
						 break;
		case UMINUS :
		             to_print = re + " := " + "- " + ar1 ;
						 break;
		case GOTO :
		             to_print = "goto " + ar1;
						 break;
		case PROC_BEGIN :
		             to_print = "proc_begin " + ar1;
						 break;
		case PROC_END :
		             to_print = "proc_end " + ar1;
						 break;
		case PARAM :
		             to_print = "param " + ar1;
						 break;
		case CALL :
		             to_print = "call " + ar1 + " " + ar2 ;
						 break;
		case RETRIEVE :
		             to_print = "retrieve " + ar1;
						 break;
		case RETURN :
		             to_print = "return " + ar1 ;
						 break;
		case LT :
		             to_print = "if " + ar1 + " < " + ar2  
						              + " goto " + re ;
						 break;
		case GT :
		             to_print = "if " + ar1 + " > " + ar2
						              + " goto " + re ;
						 break;
		case LE :
		             to_print = "if " + ar1 + " <= " + ar2 
						              + " goto " + re ;
						 break;
		case GE :
		             to_print = "if " + ar1 + " >= " + ar2 
						              + " goto " + re;
						 break;
		case EQ :
		             to_print = "if " + ar1 + " == " + ar2  
						              + " goto " + re ;
						 break;
		case NE :
		             to_print = "if " + ar1 + " != " + ar2 
						              + " goto " + re;
						 break;
		case L_INDEX_ASSIGN :
		             to_print = ar1 + "[" + ar2 + "] := " + re ;
		             break;
		case R_INDEX_ASSIGN :
		             to_print = re + " := " + ar1 + "[" + ar2 + "]"  ;
		             break;
		case ADDR_OF :
		             to_print = re + " := &" + ar1 ;
						 break;
		case LBL :
		             to_print = "label " + ar1 ;
						 break;
		default :
		             printf("Unsupported operator \n");
		             break;

  }	

  return(to_print);
}


//! Checks if the quad is sane
void 
quad ::check_sanity()
{

	switch(op)
	{
		case ASSIGN : 
						 assert((res != NULL ) && (arg1 !=NULL) && (arg2==NULL));
						 break;
		case ADD :
						 assert((res != NULL ) && (arg1 !=NULL) && (arg2 !=NULL) );
						 break;
		case MUL :
						 assert((res != NULL ) && (arg1 !=NULL) && (arg2 !=NULL) );
						 break;
		case DIV :
						 assert((res != NULL ) && (arg1 !=NULL) && (arg2 !=NULL) );
						 break;
		case SUB :
						 assert((res != NULL ) && (arg1 !=NULL) && (arg2 !=NULL) );
						 break;
		case UMINUS :
						 assert((res != NULL ) && (arg1 !=NULL) && (arg2==NULL));
						 break;
		case GOTO :
						 assert((res == NULL ) && (arg1 !=NULL) && (arg2==NULL));
						 break;
		case PROC_BEGIN :
						 assert((res == NULL ) && (arg1 !=NULL) && (arg2==NULL));
						 break;
		case PROC_END :
						 assert((res == NULL ) && (arg1 !=NULL) && (arg2==NULL));
						 break;
		case PARAM :
						 assert((res == NULL ) && (arg1 !=NULL) && (arg2==NULL));
						 break;
		case CALL :
						 assert((res == NULL ) && (arg1 !=NULL) && (arg2!=NULL));
						 break;
		case RETRIEVE :
						 assert((res == NULL ) && (arg1 !=NULL) && (arg2==NULL));
						 break;
		case RETURN :
						 assert((res == NULL ) && (arg1 !=NULL) && (arg2==NULL));
						 break;
		case LT :
						 assert((res != NULL ) && (arg1 !=NULL) && (arg2!=NULL));
						 break;
		case GT :
						 assert((res != NULL ) && (arg1 !=NULL) && (arg2!=NULL));
						 break;
		case LE :
						 assert((res != NULL ) && (arg1 !=NULL) && (arg2!=NULL));
						 break;
		case GE :
						 assert((res != NULL ) && (arg1 !=NULL) && (arg2!=NULL));
						 break;
		case EQ :
						 assert((res != NULL ) && (arg1 !=NULL) && (arg2!=NULL));
						 break;
		case NE :
						 assert((res != NULL ) && (arg1 !=NULL) && (arg2!=NULL));
						 break;
		case L_INDEX_ASSIGN :
						 assert((res != NULL ) && (arg1 !=NULL) && (arg2!=NULL));
		             break;
		case R_INDEX_ASSIGN :
						 assert((res != NULL ) && (arg1 !=NULL) && (arg2!=NULL));
		             break;
		case ADDR_OF :
						 assert((res != NULL ) && (arg1 !=NULL) && (arg2==NULL));
						 break;
		case LBL :
						 assert((res == NULL ) && (arg1 !=NULL) && (arg2==NULL));
						 break;
		default :
		             printf("Unsupported operator \n");
						 exit(1);
		             break;

  }

	return;
}

//! Returns True, if the quad is a binary operator
int
quad :: is_binary_op()
{
	if(( op == ADD ) || 
		( op == MUL ) || \
		( op == DIV ) || \
		( op == SUB ) ){

		return(1);
	}
	return(0);
}

//! Returns True, if the result is valid for CSE
int 
quad :: is_res_valid_for_cse()
{
	if (
			( op == ADD )|| 
			( op == MUL )||
			( op == DIV )||
			( op == SUB )||
			( op == UMINUS)||
			( op == ADDR_OF)||
			( op == ASSIGN )||
			( op == R_INDEX_ASSIGN ) 
		){
		return(1);
	}
	return(0);
}

//! Returns True, if the result is valid for Code Motion
int
quad :: is_res_valid_for_cm()
{
	if( 
	 ( op == ADD ) || 
    ( op == MUL ) ||
    ( op == DIV ) ||
    ( op == SUB ) ||
    ( op == UMINUS ) ||
	 ( op == ADDR_OF) ||
    ( op == ASSIGN ) ||
	 ( op == R_INDEX_ASSIGN) ){
	 	return(1);
	}
	return(0);
}

//! Returns True, if the symbol s is used in a quad
int
quad :: is_it_used( sym_tab_entry *s )
{
	int check=0;

	if (
	 ( op == PROC_BEGIN ) ||
	 ( op == PROC_END ) ||
	 ( op == RETRIEVE ) ||
	 ( op == CALL ) ||
	 ( op == LBL ) ||
    ( op == GOTO  ) ) {
	 	return(0);
	}

	 /* Three operands - Case 1 */
	 if (
    	( op == ADD )||
    	( op == MUL )||
    	( op == DIV )||
    	( op == SUB )||
	 	( op == R_INDEX_ASSIGN)
		){

		check=2;
	}


	 /* Two Operand - with result - Case 2 */
	 if (
    ( op == UMINUS )||
	 ( op == ADDR_OF )||
    ( op == ASSIGN )||
	 ( op == RETURN )||
	 ( op == PARAM ) ){
	 	check=1;
	}

	if(

    ( op == LT )||
    ( op == GT )||
    ( op == LE )||
    ( op == GE )||
    ( op == EQ )||
    ( op == NE )||
	 ( op == L_INDEX_ASSIGN)
	 ){
	 	check=3;
	}

	switch(check){
		case 3:
					if( res == s ){
						return(1);
					}
		case 2:
					if( arg2 == s ){
						return(1);
					}
		case 1:
					if( arg1 == s ){
						return(1);
					}
	}
	return(0);


}
	

//! Returns True, if the symbol s is defined in a quad
int
quad :: is_it_defined( sym_tab_entry *s )
{
	 /* res */

	if(
    ( op == ADD ) ||
    ( op == MUL ) ||
    ( op == DIV ) ||
    ( op == SUB ) ||
    ( op == UMINUS ) ||
	 ( op == ADDR_OF) ||
    ( op == ASSIGN ) ||
	 ( op == R_INDEX_ASSIGN) ){

	 	if( s == res ){
			return(1);
		}
		return(0);
	}

	 	

	if( op == RETRIEVE ){ /* arg1 */
	 	if( s == arg1 ){
			return(1);
		}
		return(0);
	}

	 /* None defined */
	 if(
    ( op == LT ) ||
    ( op == GT ) ||
    ( op == LE ) ||
    ( op == GE ) ||
    ( op == EQ ) ||
    ( op == NE ) ||
	 ( op == L_INDEX_ASSIGN) ||
	 ( op == PROC_BEGIN) ||
	 ( op == PROC_END) ||
	 ( op == RETURN) ||
	 ( op == PARAM) ||
	 ( op == CALL) ||
	 ( op == LBL) ||
    ( op == GOTO ) ){
	 	return(0);
	}

	return(0);
	 
}
	
	
//! Returns True, if the result is valid for CSE analysis
int
quad :: is_res_valid_for_gcse()
{
	if (
			( op == ADD )|| 
			( op == MUL )||
			( op == DIV )||
			( op == SUB )
		){
		return(1);
	}
	return(0);
}


//! Returns True, if the Operator is valid for Live Variable analysis
int 
quad :: is_op_valid_for_live_analysis()
{
	if ( 
			(op == GOTO) ||
			(op == CALL) ||
			(op == LBL) ||
			(op == PROC_BEGIN) ||
			(op == PROC_END) ||
			(op == GOTO)

		){ 

		return(0);
	}
	return(1);
}

//! Returns True, if the res is valid for Live Variable analysis
int
quad :: is_res_valid_for_live_analysis()
{
	if ( 
			(op  >= LT ) &&
			(op <= NE) 
		){ 

		return(0);
	}

	if( op == L_INDEX_ASSIGN ){
		return(0);
	}
	return(1);
}

//! Returns True, if the operator of the quad is valid for RD analysis
int quad :: is_op_valid_for_rd_analysis()
{
	if (
			( op == ADD )|| 
			( op == MUL )||
			( op == DIV )||
			( op == SUB )||
			( op == UMINUS)||
			( op == ADDR_OF)||
			( op == ASSIGN )||
			( op == R_INDEX_ASSIGN ) 
		){
		return(1);
	}
	return(0);
}

//! Returns True, if the operator of the quad is valid for du analysis
int quad :: is_op_valid_for_du_analysis()
{
	if( 
	 		(op==PROC_BEGIN ) ||
	 		(op==PROC_END ) ||
	 		(op==CALL ) ||
	 		(op==LBL ) ||
	 		(op==GOTO ) ) {

			return(0);
	}

	return(1);


}


//! Returns True, if the operator of the quad has an invalid res
int quad :: is_res_valid()
{
	/* There are two types here */
	/*
	1. In which res is NULL like PROC_BEGIN,PROC_END,GOTO etc
	2. In which res is NOn Null, but still res is not like a result
	   of the Operation. e.g. L_INDEX,LT and other comparison
		operators
	*/

	if( 
	 		(op==PROC_BEGIN ) ||
	 		(op==PROC_END ) ||
	 		(op==CALL ) ||
	 		(op==LBL ) ||
	 		(op==RETURN ) ||
	 		(op==RETRIEVE ) ||
	 		(op==PARAM ) ||
	 		(op==GOTO ) ) {

			return(0);
	}


	if ( 
			(op  >= LT ) &&
			(op <= NE) 
		){ 
			return(0);
	}

	if( op == L_INDEX_ASSIGN ){
		return(0);
	}

	return(1);
}


	

//! Returns the Operator of the Quad in string form
string 
quad :: get_op()
{
	string s;

	switch(op)
	{
		case ASSIGN : s="=";break;
		case ADD :s="+";break;
		case MUL :s="*";break;
		case DIV :s="/";break;
		case SUB :s="-";break;
		case UMINUS :s="-";break;
		case GOTO :s="goto";break;
		case PROC_BEGIN :s="proc_begin";break;
		case PROC_END :s="proc_end";break;
		case PARAM :s="param";break;
		case CALL :s="call";break;
		case RETRIEVE :s="retr";break;
		case RETURN :s="return";break;
		case LT :s="<";break;
		case GT :s=">";break;
		case LE :s="<=";break;
		case GE :s=">=";break;
		case EQ :s="==";break;
		case NE :s="!=";break;
		case L_INDEX_ASSIGN :s="[]=";break;
		case R_INDEX_ASSIGN :s="=[]";break;
		case ADDR_OF :s="&";break;
		case LBL :s="lbl";break;
		case UNKNOWN_OP :printf("unexpected operator \n");exit(1);
	}
	return s;
}


/* quad.cc end */
