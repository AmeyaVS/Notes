#ifndef IC_GEN_H
#define IC_GEN_H

/*!
	@file ic_gen.h
	@brief Intermediate Code Generation Header.
*/

#include "semantic_analysis.h"


/* backpatch.h begins */

//! List of the Indexes of the quads in a quad array
typedef list <int >   quad_list ;
quad_list * makelist(int index);
void quadlist_merge (quad_list *p3,quad_list *p1 , quad_list *p2 );
void backpatch(quad_list *p, sym_tab_entry *label );
void quadlist_print (quad_list *p1 );

/* backpatch.h ends */

/* quad.h begins */

//! All the Operators in the Intermediate Code
enum oprtr
{
    /* Three operands - Case 1 */
    ADD ,
    MUL ,
    DIV ,
    SUB ,

    /* Two Operand - with result - Case 2 */
    UMINUS ,
    ADDR_OF,
    ASSIGN ,

    /* Two Operand - No result - Case 1 , with result undefined */
    LT ,
    GT ,
    LE ,
    GE ,
    EQ ,
    NE ,

	 /* Three Operands - but cannot do cse */
	 L_INDEX_ASSIGN,
	 R_INDEX_ASSIGN,

	 /* One operand  - with no result - Case 2 */
	 PROC_BEGIN,
	 PROC_END,
	 RETURN,
	 RETRIEVE,
	 PARAM,
	 CALL,
	 LBL,
    GOTO ,
	 

	 /* Unknown Operator */
	 UNKNOWN_OP=-1


};

//void invalidate_tac();

//! Represents a quad in the Intermediate Code
class quad
{
public:
	//! Operator of the quad
	oprtr op; 
	//! arg1 of the quad
	sym_tab_entry *arg1; 
	//! arg2 of the quad
	sym_tab_entry *arg2;
	//! res of the quad
	sym_tab_entry *res;
	string as_string();
	//! Size of the Operand
	int size_of_op;
	quad(oprtr opr, sym_tab_entry *argmt1,sym_tab_entry *argmt2,sym_tab_entry *result);
	void check_sanity();
	int is_binary_op();
	int is_res_valid();
	int is_res_valid_for_cse();
	int is_it_used(sym_tab_entry *s);
	int is_it_defined(sym_tab_entry *s);
	int is_res_valid_for_cm();
	int is_res_valid_for_gcse();
	int is_op_valid_for_live_analysis();
	int is_op_valid_for_du_analysis();
	int is_res_valid_for_live_analysis();
	int is_op_valid_for_rd_analysis();
	string get_op();
};

quad *emit(oprtr op,sym_tab_entry *arg1,sym_tab_entry *arg2,sym_tab_entry *res);
//quad *gen(oprtr op,sym_tab_entry *arg1,sym_tab_entry *arg2,sym_tab_entry *res);

/* quad.h begins */

#endif
