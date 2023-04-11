#include "optimize.h"
#include <assert.h>

/*!
	@file optimize.cc
	@brief Optimization Implementation
*/

extern vector <quad *>quad_arr; /* defined in ic_gen */
extern int tac_not_valid; /* defined in ic_gen */
extern vector <quad *> *final_quad_arr_ptr; /* defined in ic_gen */

/* Global set by main() for examples
   has value 1 for ex1, 2 for ex2 and so on 
   has value of -1 if we dont compile with anything
	specific
*/

//! A function to fetch the example flag
extern unsigned short get_example_flag();
//! Variable representing chapter number and Example number
static	unsigned short chap_ex;
int generate_optimized_target_code();


//! Command line Optimizations Control

//! Local Optimization
#define LOCAL  0x01 
//! Global Common Subexpression Elimination
#define GCSE	0x02
//! Global Copy Propagation
#define GCP	   0x04
//! Global Loop Invariant Code Motion
#define GCM	   0x08
//! Global Peephole Optimization
#define GPO	   0x010
//! Global Dead Code Elimination
#define GDCE	   0x020


/* df_analysis.cc */
//! Input quads to optimization
static vector <quad *> *input_quad_arr_ptr;
//! Output quads of optimization
static vector <quad *> *output_quad_arr_ptr;
//! API to make entry in Literal Table
extern sym_tab_entry *make_const_sym_tab_entry(int val);

void Tokenize(const string& str,
                      vector<string>& tokens,
                      const string& delimiters );

//! Print the quads in a basic block
void
basic_block :: print_quads()
{
	int i;
	quad *quad_ptr;

	printf("BLOCK =%d \n",block_no);
	for(i=start;i<end;i++){
		quad_ptr=(*input_quad_arr_ptr)[i];
		printf("(%d) %s\n",i,quad_ptr->as_string().c_str());
	}
}

//! Print the details of a basic block
void
basic_block :: print()
{
	int i,n;
	string s;
	quad *quad_ptr;

	printf("========================================\n");
	printf("BLOCK =%d \n",block_no);
	printf("start quad=%d end = %d \n",start,(end-1));
	n=successors.size();
	printf("Sucessors :");
	for(i=0;i<n;i++){
		printf(" %d ",successors[i]);
	}
	printf("\n");

	n=predecessors.size();
	printf("Predecessors :");
	for(i=0;i<n;i++){
		printf(" %d ",predecessors[i]);
	}
	printf("\n");

	printf("rd_GEN[B%d]={",block_no);
	print_quad_set(&rd_GEN);
	printf("}\n");

	printf("rd_KILL[B%d]={",block_no);
	print_quad_set(&rd_KILL);
	printf("}\n");

	printf("rd_IN[B%d]={",block_no);
	print_quad_set(&rd_IN);
	printf("}\n");

	printf("rd_OUT[B%d]={",block_no);
	print_quad_set(&rd_OUT);
	printf("}\n");

	printf("e_GEN[B%d]={",block_no);
	print_expr_set(&e_GEN);
	printf("}\n");

	printf("e_KILL[B%d]={",block_no);
	print_expr_set(&e_KILL);
	printf("}\n");

	printf("e_IN[B%d]={",block_no);
	print_expr_set(&e_IN);
	printf("}\n");

	printf("e_OUT[B%d]={",block_no);
	print_expr_set(&e_OUT);
	printf("}\n");

	printf("live_DEF[B%d]={",block_no);
	print_v_set(live_DEF);
	printf("}\n");

	printf("live_USE[B%d]={",block_no);
	print_v_set(live_USE);
	printf("}\n");

	printf("live_IN[B%d]={",block_no);
	print_v_set(live_IN);
	printf("}\n");

	printf("live_OUT[B%d]={",block_no);
	print_v_set(live_OUT);
	printf("}\n");

	printf("du_DEFS[B%d]={",block_no);
	print_du_set(du_DEF);
	printf("}\n");

	printf("du_USE[B%d]={",block_no);
	print_du_set(du_USE);
	printf("}\n");

	printf("du_IN[B%d]={",block_no);
	print_du_set(du_IN);
	printf("}\n");

	printf("du_OUT[B%d]={",block_no);
	print_du_set(du_OUT);
	printf("}\n");

	printf("c_GEN[B%d]={",block_no);
	print_quad_set(&c_GEN);
	printf("}\n");

	printf("c_KILL[B%d]={",block_no);
	print_quad_set(&c_KILL);
	printf("}\n");

	printf("c_IN[B%d]={",block_no);
	print_quad_set(&c_IN);
	printf("}\n");

	printf("c_OUT[B%d]={",block_no);
	print_quad_set(&c_OUT);
	printf("}\n");

	print_du_chains();
	print_ud_chains();

	print_liveness();

	printf("dominators are D[B%d]={",block_no);
	print_quad_set(&dominators);
	printf("}\n");

	for(i=start;i<end;i++){
		quad_ptr=(*input_quad_arr_ptr)[i];
		printf("(%d) %s\n",i,quad_ptr->as_string().c_str());
	}

	printf("\n");
}

//! Get the quad number for the last evaluation of the given expression
int
basic_block :: get_last_evaluated_expr(int expr_no)
{
	sym_tab_entry *arg1,*arg2;
	quad *quad_ptr;
	int quad_no;
	int i;
	oprtr op;


	quad_no=optimizer_ptr->universal_expr.get_quad_no(expr_no);

	arg1=(*input_quad_arr_ptr)[quad_no]->arg1;
	arg2=(*input_quad_arr_ptr)[quad_no]->arg2;
	op = (*input_quad_arr_ptr)[quad_no]->op;

	for(i=(end-1);i>=start; i--){
		quad_ptr = (*input_quad_arr_ptr)[i];

		if(quad_ptr->is_res_valid_for_cse() != 1){
			continue;
		}

		if(quad_ptr->is_binary_op() == 1){
			if((quad_ptr->arg1 == arg1 ) &&
				(quad_ptr->arg2 == arg2 )  &&
				(quad_ptr->op == op )){
				return(i);
			}
		}
	}

	return(0);

}


//! The blocks containing an expression
set <int,less<int> > block_found;

//! Is the block already checked for GCSE
set<int,less<int> > already_checked;

//! Global Common Subexpression Elimination
void
basic_block :: g_cse_elimination()
{
	int i,n;
	int quad_no,marked_quad_no;
	expr_set :: iterator iter;
	int expr_quad_no;
	sym_tab_entry *expr_arg1,*expr_arg2, *res;
	quad *quad_ptr;
	int expr_no,expr_no1;
	vector<int> reaching_expr;
	set <int,less<int> > ::iterator iter1;
	basic_block *bb_ptr;
	int bno;

	gcse *gcse_ptr;

	/* Identifying the expressions that are reaching the quads
	   in this block
	*/
	for (iter=e_IN.begin();iter != e_IN.end();iter ++ ){

		expr_no = (*iter);
		expr_quad_no = optimizer_ptr->universal_expr.get_quad_no(expr_no);

		expr_arg1 = (*input_quad_arr_ptr)[expr_quad_no]->arg1;
		expr_arg2 = (*input_quad_arr_ptr)[expr_quad_no]->arg2;

		for(i=start;i<end;i++){
			quad_ptr = (*input_quad_arr_ptr)[i];

			if(quad_ptr->is_res_valid_for_cse() != 1){
				continue;
			}

			if(quad_ptr->is_binary_op() == 1){
				expr_no1 = optimizer_ptr->universal_expr.get_avail_expr_no(i);
				if(expr_no1 == expr_no ){
					reaching_expr.push_back(i);
					break;
				}
			}

			res=quad_ptr->res;

			if((res == expr_arg1) || (res == expr_arg2) ){
				break;
			}
		}
	}

	n=reaching_expr.size();


	for(i=0;i<n;i++){
		quad_no=reaching_expr[i];
		expr_no = optimizer_ptr->universal_expr.get_avail_expr_no(quad_no);

		block_found.clear();
		already_checked.clear();
		mark_g_cse_for_elim(expr_no);

		if(block_found.size() ){
			gcse_ptr=&optimizer_ptr->gcse_quads;
			for( iter1=block_found.begin(); iter1 != block_found.end();iter1++){
				bno=(*iter1);
				bb_ptr=optimizer_ptr->basic_blocks_arr[bno];
				marked_quad_no=bb_ptr->get_last_evaluated_expr(expr_no); 
//				printf("expr for quad=%d marked_quad_no=%d\n",quad_no,marked_quad_no);
				gcse_ptr->insert_marked_quads(marked_quad_no,quad_no);
			}
		}
	}

}

//! Identify Copy Propagation Statements
void
basic_block :: identify_copy_prop_stmts()
{
	int i;
	int ret;
	quad_set uses_reached_by_def;
	int quad_no,bno;
	basic_block *bb_ptr;
	quad *quad_ptr;
	quad_set :: iterator iter;
	quad_set tmp_c_IN;
	sym_tab_entry *arg1,*res;

	for(i=start;i<end;i++){
		quad_ptr = (*input_quad_arr_ptr)[i];
		arg1=quad_ptr->arg1;
		res=quad_ptr->res;

		if(quad_ptr->op != ASSIGN ){
			continue;
		}

		uses_reached_by_def= *(optimizer_ptr->quad_info_arr[i]->du_chains[0]);
		for(iter=uses_reached_by_def.begin();iter != uses_reached_by_def.end();iter++){

			quad_no = (*iter);
			bno = optimizer_ptr->get_block_no(quad_no);
			bb_ptr=optimizer_ptr->basic_blocks_arr[bno];
			tmp_c_IN=bb_ptr->c_IN;
			if(tmp_c_IN.find(i) != tmp_c_IN.end() ){
				ret=bb_ptr->check_for_killing_defs(quad_no,arg1,res);
				if(ret == 0){
					optimizer_ptr->copy_prop_quads.insert(i,quad_no,arg1,res);
					//printf("quad_no=%d is the only def reaching from %d\n",quad_no,i);
				}
			}
		}
	}
	
	return;
}

//! Check if the args have been killed before this quad
int
basic_block :: check_for_killing_defs(int quad_no,sym_tab_entry *inp_arg1, sym_tab_entry *inp_res)
{
	int i;
	int args_redefined;
	sym_tab_entry *res;
	quad *quad_ptr;

	args_redefined=0;
	for(i=start;i<quad_no;i++){
		quad_ptr=(*input_quad_arr_ptr)[i];
		res=quad_ptr->res;

		if(quad_ptr->is_res_valid_for_cse() != 1){
			continue;
		}


		if((res == inp_arg1) || (res == inp_res ) ){
			args_redefined=1;
			break;
		}

	}
	if(args_redefined == 1){
		return(1);
	}else{
		return(0);
	}
}


//! Mark ???
void 
basic_block :: mark_g_cse_for_elim(int expr_no)
{
	int pred;
	int i,n;
	basic_block *bb_ptr,*pred_ptr;
	expr_set *e_KILL_ptr;
	expr_set *e_IN_ptr;
	expr_set *e_GEN_ptr;


	bb_ptr = this;
	n= bb_ptr->predecessors.size();

	if(already_checked.find(block_no) != already_checked.end() ){
		return;
	}else{
		already_checked.insert(block_no);
	}


	for(i=0;i<n;i++){

		pred = bb_ptr->predecessors[i];

		/* does its e_KILL contain expr_no */
		pred_ptr = bb_ptr->optimizer_ptr->basic_blocks_arr[pred];

		e_KILL_ptr = &(pred_ptr->e_KILL);
		e_IN_ptr = &(pred_ptr->e_IN);
		e_GEN_ptr = &(pred_ptr->e_GEN);

//		printf("Searching for expr_no=%d in block=%d \n",expr_no,pred_ptr->block_no);

		/* e_KILL contains it,or e_IN does not contain it */
		/*
		if((e_KILL_ptr->find(expr_no) != e_KILL_ptr->end()) ||
		   (e_IN_ptr -> find(expr_no) == e_IN_ptr->end() ) ){
			*/

		if(e_GEN_ptr->find(expr_no) != e_GEN_ptr->end()){
			/* mark it */
			block_found.insert(pred_ptr->block_no);
		}else{
			pred_ptr->mark_g_cse_for_elim(expr_no);
		}
	}
	return;
}


//! Get the DAG node associated with Symbol Table Entry
dag_node *
basic_block :: get_dag_node(sym_tab_entry *s)
{
	dag_node *d;

	if(s == NULL ){
		return(NULL);
	}

	if((d=get_current(s)) == NULL) {

		/* create a leaf node */
		d=new dag_node(s,this);
		dag_arr.push_back(d);

		/* set the associated node */
		set_current(s,d);

		/* set the attached identifier */
	}

	return(d);
}

//! Get the DAG Node associated with the Address of Symbol Table Entry
dag_node *
basic_block :: get_addr_node(sym_tab_entry *s)
{
	dag_node *d;
	int i,n;
	dag_node *dag_node_ptr;

	d = NULL;
	n = addr_nodes.size();
	for(i=0;i<n;i++)
	{
		dag_node_ptr = addr_nodes[i];
		if(dag_node_ptr->id == s ) {
			d = dag_node_ptr;
			break;
		}
	}


	if(d == NULL) {

		/* create a leaf node */
		d=new dag_node(s,this);
		d->lval=1; /* mark it for lval */
		dag_arr.push_back(d);
		addr_nodes.push_back(d);

	}

	return(d);
}

//! Move the identifier from existing to the given DAG node
void
basic_block :: move_attached_identifier(sym_tab_entry *s,dag_node *d)
{
	dag_node *tmp;
	tmp = get_current(s);
	if(tmp != NULL ){
		tmp ->remove_attached_identifier(s);
	}
	set_current(s,d);
	d->add_attached_identifier(s);
}	


/* Construct dag */
//! Construct a DAG for a Basic Block
void 
basic_block::construct_dag()
{
	int j;
	quad *quad_ptr;
	dag_node *node_res,*node_arg1, *node_arg2;
	dag_node *n4;
	sym_tab_entry *arg1,*arg2,*res;
	oprtr op;

	if(dead==1){
		return;
	}

	for(j=start;j<end;j++){

		quad_ptr = (*input_quad_arr_ptr)[j];

		arg1 = quad_ptr ->arg1;
		arg2 = quad_ptr ->arg2;
		res = quad_ptr ->res ;
		op = quad_ptr ->op;

		node_arg1=node_arg2=node_res=NULL;


		/* Step 1 */
		if(op == ADDR_OF ){
			node_arg1=get_addr_node(arg1);
		}else{
			node_arg1 = get_dag_node(arg1);
		}


		/* Step 2 */
		node_arg2 = get_dag_node(arg2);

		/* Step 3 */
		if( op == ASSIGN ){
			/* Sure that there is an associated node for arg1 */
			move_attached_identifier(res,node_arg1);

		} else if(IS_RES_VALID(op)){

			node_res = find_dag_node(op,node_arg1,node_arg2);

			if(node_res == NULL){

				/* No dag node with matching l child and r child */
				node_res = new dag_node(op,node_arg1,node_arg2,NULL,this);
				dag_arr.push_back(node_res);
			}
			move_attached_identifier(res,node_res);

		}else{

			/*
			These are  operators does not have a "result" 
			from a CSE perspective

			LT,GT,LE,GE,EQ,NE,L_INDEX_ASSIGN - 3 args
			PROC_BEGIN,PROC_END,RETURN,
			RETRIEVE,PARAM,CALL,LBL,GOTO - 1 arg

			*/
			node_res = get_dag_node(res);

			n4 = new dag_node(op,node_arg1,node_arg2,node_res,this);
			dag_arr.push_back(n4);


		}

		/* The array handling code is required always except 
			in the  case of ex2, where we show the side effects 
			of not having this code */

		chap_ex=get_example_flag();
		if(chap_ex == 0x0702 ){ /* chap7 example 2 */
			continue;
		}


		#if 0
		dag_node *p,*q,*b;
		/* Read using a Pointer */
		if((op == R_INDEX_ASSIGN) ||
		((op == PARAM) && (arg1->type->gettype() == POINTER)) ){
			if(node_arg1->type_of_node  == LEAF ){
				/* ambigious pointer */
				all_locals_used();
			}else{

				b = node_arg1->find_base_addr_node();

				/* The base address could be array or variable */
				p = get_current(b->left_child->id);

				if(p != NULL ){
					/* variable */
					live_OUT.insert(b->left_child->id);
				}else{
					/* array - Nothing to do*/

				}
			}
		}
		
		if((op == L_INDEX_ASSIGN ) ||
		((op == PARAM) && (arg1->type->gettype() == POINTER)) ){
			/* Write into memory using pointer */
			if(node_arg1 -> type_of_node == LEAF ){ 
				/* ambiguous ptr */
				kill_all_nodes();
			}else{
				b = node_arg1->find_base_addr_node();
				p = get_current(b->left_child->id);

				if( p != NULL ){
					/* Variable */
					q = p;
				}else{
					/* array */
					q = b;
				}

				q -> kill_parents();

			}
		}
		#endif

		if(op == L_INDEX_ASSIGN ){
			kill_all_nodes();
		}
		
		if(op == CALL ){
			//kill_all_g_nodes();
			kill_all_nodes();
		}
	}
}

/*
void
basic_block :: all_locals_used()
{
	sym_tab_entry *sptr;
	list <sym_tab_entry *> all_vars;
	list <sym_tab_entry *> :: iterator iter;

	optimizer_ptr->sym_tab_ptr->get_list(&all_vars);; 

//	printf("All locals called n=%d\n",all_vars.size() );

	iter = all_vars.begin();

	while( iter != all_vars.end() )
	{
		sptr = (*iter);
		live_OUT.insert(sptr);
		iter ++ ;
	}
	return;
}
*/
	



//! Print the DAG for the Basic Block
void 
basic_block:: print_dag()
{
	int i,n;
	dag_node *dag_node_ptr;

	n = dag_arr.size();
	for(i=0;i<n;i++)
	{
		dag_node_ptr = dag_arr[i];
		dag_node_ptr->print();
	}
}



//! Find the DAG node with a given Left and Right Operands
dag_node * 
basic_block::find_dag_node(oprtr op,dag_node *l, dag_node *r )
{
	int i,n;

	n = dag_arr.size();
	for(i=0;i<n;i++)
	{
		if(
			(dag_arr[i] ->label == op ) && 
			(dag_arr[i] ->left_child == l ) &&
			(dag_arr[i] ->right_child == r ) 
			
			){

				if(dag_arr[i] ->killed ){
					continue;
				}

				return(dag_arr[i]);
		}
	}

	return(NULL);

}

//! Kill all nodes in a DAG
void
basic_block::kill_all_nodes()
{
	int i,n;

	n = dag_arr.size();
	for(i=0;i<n;i++)
	{
			dag_arr[i] ->killed=1; 
	}

}

//! Kill all nodes in a DAG belonging to Global
void
basic_block :: kill_all_g_nodes()
{
	list<sym_tab_entry *>all_globals;
	list<sym_tab_entry *>::iterator iter;
	extern sym_tab *global_sym_tab_ptr;
	sym_tab_entry *sptr;
	dag_node *dag_node_ptr;

	global_sym_tab_ptr->get_list(&all_globals);

	iter = all_globals.begin();

	while( iter != all_globals.end() )
	{
		sptr = (*iter);

		dag_node_ptr=get_current(sptr) ;

		if(dag_node_ptr != NULL ){
			dag_node_ptr->killed=1;
			dag_node_ptr->kill_parents();
		}

		iter ++ ;
	}
	return;
}



//! Get the DAG node corresponding to a symbol table entry
dag_node * 
basic_block::get_current(sym_tab_entry *s)
{
	identifier_to_dag_node :: iterator it;

	it = associated_dag_nodes.find(s);

	if(it == associated_dag_nodes.end() ){
		return(NULL);
	}else{
		return((*it).second );
	}
}

//! Destructor 
basic_block::~basic_block()
{
	int i,n;
	dag_node *dag_node_ptr;

	n = dag_arr.size();
	for(i=0;i<n;i++)
	{
		dag_node_ptr = dag_arr[i];
		delete(dag_node_ptr);
	}
}



//! Associate a Identifier with a DAG node
void
basic_block::set_current(sym_tab_entry *s, dag_node *d)
{
	associated_dag_nodes[s]=d;	
	return;
}

//! Re-construct the optimized TAC from the DAG
void
basic_block::reconstruct_tac()
{
	int i,n;
   dag_node *dag_node_ptr;

   n = dag_arr.size();
   for(i=0;i<n;i++)
   {
      dag_node_ptr = dag_arr[i];

		/* Operator */
		if((dag_node_ptr->type_of_node == LEAF ) &&
			(dag_node_ptr->lval == 0 ) ){
			leaves.push_back(dag_node_ptr);
		}
   }


   n = dag_arr.size();
   for(i=0;i<n;i++)
   {
      dag_node_ptr = dag_arr[i];

		/* Operator */
		dag_node_ptr->regenerate_quad();
   }

}

//! Eliminate Dead Code from the DAG
void
basic_block::remove_dead_code()
{
	int i,n;
   dag_node *dag_node_ptr;

   n = dag_arr.size();
   for(i=0;i<n;i++)
   {
      dag_node_ptr = dag_arr[i];

		/* Operator */
		dag_node_ptr->elim_dead_code();
   }

}

//! Constant Folding Transformation on a DAG
void
basic_block::constant_folding()
{
	int i,n;
   dag_node *dag_node_ptr;

   n = dag_arr.size();
   for(i=0;i<n;i++)
   {
      dag_node_ptr = dag_arr[i];

		/* Operator */
		dag_node_ptr->perform_constant_folding();
   }

}

//! Create the rd GEN
void
basic_block::create_GEN_set()
{
	int i;
	quad *quad_ptr;
	for(i=start;i<end;i++){
		quad_ptr=(*input_quad_arr_ptr)[i];
		if(quad_ptr->is_op_valid_for_rd_analysis() == 0 ){
			continue;
		}
		if(quad_ptr->res != NULL){
			rd_GEN.insert(i);
		}
	}
}

/*
A copy statement s: x := y is generated in the block B, if s 
occurs in B and there is no subsequent assignment to y within B */

//! Variable to Quad Number Mapping
typedef  map<sym_tab_entry*, int , less<sym_tab_entry *> > var_quad_map;

//! Create the c_GEN Set
void
basic_block::create_c_GEN_set()
{
	int i;
	quad *quad_ptr;
	quad_set tmp_set;
	vset rhs;
	var_quad_map vq;
	int quad_no;

	for(i=start;i<end;i++){
		quad_ptr=(*input_quad_arr_ptr)[i];

		/* Assignment x := y */
		if(quad_ptr -> op == ASSIGN){

			tmp_set.insert(i);
			vq[quad_ptr->arg1]=i;
			rhs.insert(quad_ptr->arg1);
		}

		/* is there any subsequent assignment to y
		   within the Block B */
		if(quad_ptr->is_res_valid_for_cse() ){
			if(rhs.find(quad_ptr->res) != rhs.end() ){
				quad_no=vq[quad_ptr->res];
				tmp_set.erase(quad_no);
			}
		}
	}
	c_GEN=tmp_set;
}

/* A statement s : x :=y is killed, in B , if x or y is assigned 
   in B and s is NOT in B */

//! Create the c_KILL Set
void
basic_block::create_c_KILL_set(quad_set univ_copy_stmts)
{
	quad_set other_assign_stmts;
	var_quad_set_map vq;
	var_quad_set_map :: iterator iter1;
	quad_set kill_set;
	quad_set *p;
	int quad_no;
	quad *quad_ptr;
	quad_set :: iterator iter;
	sym_tab_entry *arg1,*res;
	int i;
	int op;

	/* s is NOT in B */
	for(iter=univ_copy_stmts.begin();iter !=univ_copy_stmts.end();iter++){
		quad_no = (*iter);

		if( !((quad_no >= start) && (quad_no < end)) ){
			other_assign_stmts.insert(quad_no);
		}
	}

	/* create references for each of the assign statements */
	for(iter=other_assign_stmts.begin();
	        iter !=other_assign_stmts.end();
			    iter++){
		quad_no=(*iter);
		quad_ptr=(*input_quad_arr_ptr)[quad_no];

		arg1=quad_ptr->arg1;
		res = quad_ptr->res;

		if(arg1->is_it_a_constant() != 1 ){
			p=vq[arg1];
			if(p == NULL  ){
				p = new quad_set();
				vq[arg1]=p;
			}
			p->insert(quad_no);
		}

		p=vq[res];
		if(p == NULL ){
			p = new quad_set();
			vq[res]=p;
		}
		p->insert(quad_no);
	}


	/* x or y is assigned in B  */
	for(i=start;i<end;i++){
		quad_ptr = (*input_quad_arr_ptr)[i];
		op = quad_ptr->op;
		if(quad_ptr->is_res_valid_for_cse() != 1 ){
			continue;
		}

		res=quad_ptr->res;

		p = vq[res];

		if( p != NULL){
			make_union(kill_set,*p,kill_set);
		}
	}

	c_KILL = kill_set;

}

//! ???
void
basic_block :: add_to_univ_copy_stmts(quad_set &univ_copy_stmts)
{
	quad *quad_ptr;
	int i;
	int op;

	for(i=start;i<end;i++){

		quad_ptr = (*input_quad_arr_ptr)[i];
		op=quad_ptr->op;

		if( op == ASSIGN ){
			univ_copy_stmts.insert(i);
		}
	}
}
	



//! Create du_DEF and du_USE sets
void
basic_block::create_du_DEF_USE_sets(du_set all_uses)
{
	int i;
	quad *quad_ptr;
	sym_tab_entry *arg1,*arg2,*res;
	du_set :: iterator iter;
	quad_var_pair qv;
	int quad_no;
	sym_tab_entry *s;

	vset tmp_set;

	du_set defs; /* is a use of x not in B and B contains 
	                  a definition of x*/
	du_set uses; /* use of 'x' in B and there is no definition 
	                  of x between the beginning of B and s*/

	for(i=start;i<end;i++){
		quad_ptr = (*input_quad_arr_ptr)[i];
		res=quad_ptr->res;

		if(quad_ptr->is_res_valid_for_cse() != 1 ){
			continue;
		}

		if( (res != NULL ) &&
		   (res->is_it_a_constant() != 1) &&
			(res->is_tmp() != 1 ) ){
			tmp_set.insert(res);
		}

	}

	/* tmp_set contains all the variables that have
	   definitions in the current block */

	for(iter = all_uses.begin();iter != all_uses.end();iter ++){
		qv=(*iter);
		quad_no = qv.first;
		s = qv.second ;

		/* is a use of x not in B */
		if(!((quad_no >= start) && (quad_no < end ) )){

			/* B contains a definition of x */
			if(tmp_set.find(s) != tmp_set.end() ){
				defs.insert(qv);
			}
		}
	}

	/*
	uses :
	s is a use of x in B and there is no definition of x 
	between the beginning of B and s
	*/
	for(i=(end-1);i>=start;i--){
		quad_ptr = (*input_quad_arr_ptr)[i];

		if(quad_ptr->is_op_valid_for_live_analysis() != 1 ){
			continue;
		}

		arg1=quad_ptr->arg1;
		arg2=quad_ptr->arg2;
		res=quad_ptr->res;

		/* The order in which we look at res first and then
		   at arg1 & arg2 is important in order to deal with
			scenarios like x:= x + a
		*/

		/* remove it, if we find a def for it */
		if( (res != NULL ) &&
			(quad_ptr->is_res_valid_for_cse() == 1) &&
		   (res->is_it_a_constant() != 1) &&
			(res->is_tmp() != 1 ) ){

			for(iter=uses.begin();iter != uses.end();){
				qv=(*iter);
				s=qv.second;
				if(s == res ){
					uses.erase(iter++);
				}else{
					++iter;
				}
			}
		}


		/* usage of x */
		if( (arg1 != NULL ) &&
		   (arg1->is_it_a_constant() != 1) &&
			(arg1->is_tmp() != 1 ) ){
			uses.insert(make_pair(i,arg1));
		}

		/* usage of x */
		if(arg2 != arg1 ){
			if( (arg2 != NULL ) && 
				(arg2->is_it_a_constant() != 1) &&
				(arg2->is_tmp() != 1 ) ){
				uses.insert(make_pair(i,arg2));
			}
		}

	}

	du_DEF=defs;
	du_USE=uses;

}

//! Create live_USE and live_DEF sets
void
basic_block::create_DEF_USE_sets()
{
	int i;
	quad *quad_ptr;
	sym_tab_entry *arg1,*arg2,*res;

	vset defs; /* definitely assigned values before use */
	vset uses; /* used before assigning values */

	for(i=start;i<end;i++){
		quad_ptr=(*input_quad_arr_ptr)[i];

		if(quad_ptr->is_op_valid_for_live_analysis() == 0 ){
			continue;
		}

		arg1=quad_ptr -> arg1;
		arg2=quad_ptr -> arg2;
		res=quad_ptr->res;

		if( (arg1 != NULL) && (arg1->is_it_a_constant() != 1) ){

			if(defs.find(arg1) == defs.end() ){
				uses.insert(arg1);
			}
		}

		if((arg2 != NULL) && (arg2->is_it_a_constant() != 1) ){
			if(defs.find(arg2) == defs.end() ){
				uses.insert(arg2);
			}
		}
			
		if((res != NULL) && (quad_ptr->is_res_valid_for_live_analysis() == 1)){
			if(uses.find(res) == uses.end() ){
				defs.insert(res);
			}
		}

	}
	live_USE = uses;
	live_DEF = defs;
}

//! Print a Variable set
void
basic_block::print_v_set(vset qs)
{
	vset :: iterator iter;
	sym_tab_entry *s;

	for(iter=qs.begin();iter != qs.end();iter++){
		s=(*iter);	
		printf(" %s ",s->name.c_str());
	}
}


//! Print a Variable set
void
print_v_set(vset qs)
{
	vset :: iterator iter;
	sym_tab_entry *s;

	for(iter=qs.begin();iter != qs.end();iter++){
		s=(*iter);	
		printf(" %s ",s->name.c_str());
	}
}



//! Print a du_set 
void
basic_block::print_du_set(du_set ds)
{
	du_set :: iterator iter;
	sym_tab_entry *s;
	quad_var_pair qv;
	int quad_no;

	for(iter=ds.begin();iter != ds.end();iter++){
		qv=(*iter);	
		s=qv.second;
		quad_no=qv.first;
		printf(" (%d,%s) ",quad_no,s->name.c_str());
	}
}


//! Create rd_KILL set
void
basic_block::create_KILL_set(quad_set all_kill_set)
{
	int quad_no;
	int tmp_quad_no;

	quad_set ::iterator iter;
	quad_set ::iterator iter1;

	iter = all_kill_set.begin();

	for(iter=all_kill_set.begin();iter != all_kill_set.end();iter++){
		quad_no=(*iter);
		if( !((quad_no >= start) && (quad_no < end) ) ){
			for(iter1=rd_GEN.begin();iter1 != rd_GEN.end();iter1++){
				tmp_quad_no=(*iter1);
				if((*input_quad_arr_ptr)[quad_no]->res == (*input_quad_arr_ptr)[tmp_quad_no]->res ){ 
					rd_KILL.insert(quad_no);
					break;
				}
			}
		}
	}
}

//! Build UD Chain
void
basic_block :: build_ud_chains()
{
	int i;
	quad *quad_ptr;
	sym_tab_entry *arg1,*arg2,*res;
	quad_set tmp_IN;
	quad_set *qs;

	tmp_IN = rd_IN;

	for(i=start;i<end;i++){
		quad_ptr=(*input_quad_arr_ptr)[i];
		arg1= quad_ptr -> arg1;
		arg2=quad_ptr -> arg2;
		res = quad_ptr -> res;

		if(quad_ptr->is_op_valid_for_live_analysis() == 0 ){
			continue;
		}

		if((arg1 != NULL) && (arg1->is_it_a_constant() != 1) ){
			qs = new quad_set();
			*qs=get_defs_for_id(arg1,tmp_IN);
			optimizer_ptr->quad_info_arr[i]->ud_chains[0]=qs;
		}

		if((arg2 != NULL) && (arg2->is_it_a_constant() != 1) ){
			if(arg2 != arg1 ){
				qs = new quad_set();
				*qs=get_defs_for_id(arg2,tmp_IN);
				optimizer_ptr->quad_info_arr[i]->ud_chains[1]=qs;
			}
		}

		if(res != NULL){
			override_defs(res,tmp_IN,i);
		}
	}
}

//! Get the definitions of a given variable in a set of quads
quad_set
basic_block :: get_defs_for_id(sym_tab_entry *arg,quad_set tmp_IN)
{
	quad_set tmp;
	quad_set :: iterator iter;
	int quad_no;
	quad *quad_ptr;


	for(iter = tmp_IN.begin(); iter != tmp_IN.end(); iter ++ ){
		quad_no = (*iter);
		quad_ptr = (*input_quad_arr_ptr)[quad_no];
		if(quad_ptr -> res == arg ){
			tmp.insert(quad_no);
		}
	}

	return(tmp);
}

//! ???
void
basic_block :: override_defs(sym_tab_entry *res,quad_set &tmp_IN,int curr_quad)
{
	quad_set tmp;
	int quad_no;
	quad *quad_ptr;
	quad_set :: iterator iter;

	for(iter = tmp_IN.begin(); iter != tmp_IN.end(); iter ++ ){
		quad_no = (*iter);
		quad_ptr = (*input_quad_arr_ptr)[quad_no];
		if(quad_ptr -> res == res ){
			tmp.insert(quad_no);
		}
	}

	for(iter = tmp.begin(); iter != tmp.end(); iter ++ ){
		quad_no = (*iter);
		tmp_IN.erase(quad_no);
	}

	tmp_IN.insert(curr_quad);
}

//! Print the UD Chain
void
basic_block :: print_ud_chains()
{
	sym_tab_entry *arg1,*arg2;
	int i;
	quad *quad_ptr;
	quad_set *qs;

	for(i=start;i<end;i++){
		quad_ptr=(*input_quad_arr_ptr)[i];
		arg1= quad_ptr -> arg1;
		arg2=quad_ptr -> arg2;

		if((arg1 != NULL )  && ((qs=optimizer_ptr->quad_info_arr[i]->ud_chains[0]) != NULL) ){
			printf("ud_chain(%d,%s) :",i,arg1->name.c_str() );
			print_quad_set(qs);
			printf("\n");
		}
		if((arg2 != NULL )  && ((qs=optimizer_ptr->quad_info_arr[i]->ud_chains[1]) != NULL) ){
			printf("ud_chain(%d,%s) :",i,arg2->name.c_str() );
			print_quad_set(qs);
			printf("\n");
		}
	}
}

//! Print the Quad Set
void
basic_block :: print_quad_set ( quad_set *qs)
{
	quad_set :: iterator iter;
	int quad_no;

	for(iter = qs->begin(); iter != qs->end(); iter ++ ){
		quad_no = (*iter);
		printf(" %d ",quad_no);
	}
}


//! Print the Quad Set
void
print_quad_set ( quad_set *qs)
{
	quad_set :: iterator iter;
	int quad_no;

	printf("{");
	for(iter = qs->begin(); iter != qs->end(); iter ++ ){
		quad_no = (*iter);
		printf(" %d ",quad_no);
	}
	printf("} ");
}

//! Print the Expr Set
void
basic_block :: print_expr_set ( expr_set *es)
{
	expr_set :: iterator iter;
	int exp_no;
	string s;

	for(iter = es->begin(); iter != es->end(); iter ++ ){
		exp_no = (*iter);
		s=optimizer_ptr->universal_expr.as_string(exp_no);
		printf(" %s ",s.c_str());
	}
}

//! Get all the expressions in a Basic Block
quad_set
basic_block :: get_all_expr()
{
	sym_tab_entry *arg1,*arg2,*res;
	int i;
	quad *quad_ptr;
	quad_set all_expr;

	for(i=start;i<end;i++){
		quad_ptr=(*input_quad_arr_ptr)[i];

		arg1= quad_ptr -> arg1;
		arg2=quad_ptr -> arg2;
		res = quad_ptr -> res ;


		if(quad_ptr->is_binary_op() != 1){
			continue;
		}
		all_expr.insert(i);
	}

	return(all_expr);
}

//! Crate e_GEN set
void
basic_block :: create_e_GEN_set()
{
	sym_tab_entry *res;
	int i;
	quad *quad_ptr;
	int quad_no;
	expr_set gen;
	quad_set :: iterator iter;
	expr_set tmp;
	int exp_no;

	for(i=start;i<end;i++){
		quad_ptr=(*input_quad_arr_ptr)[i];

		/* Eliminate quads like PROC_BEGIN,PROC_END 
		   comparison operators like LT,
			pick only the ones for which res is valid 
		*/
		if(quad_ptr->is_res_valid_for_cse() != 1){
			continue;
		}

		if(quad_ptr->is_res_valid_for_gcse() == 1){
			/* Add the expression 'y+z' to the set e_GEN */
			exp_no = optimizer_ptr->universal_expr.get_avail_expr_no(i);
			gen.insert(exp_no);
		}

		res = quad_ptr -> res ;

		/* Delete from e_GEN any expression that uses 'res' */
		tmp.clear();
		for(iter = gen.begin();
				iter != gen.end(); iter++){

			exp_no=(*iter);
			quad_no = optimizer_ptr->universal_expr.get_quad_no(exp_no);

			if(((*input_quad_arr_ptr)[quad_no]->arg1 == res) ||
				((*input_quad_arr_ptr)[quad_no]->arg2 == res) ){
				tmp.insert(exp_no);
			}
		}

		if(tmp.size() > 0 ){
			for(iter = tmp.begin();
				iter != tmp.end(); iter++){
				exp_no = (*iter);
				gen.erase(exp_no);
			}
		}
	}
	e_GEN = gen ;
}

//! Crate e_KILL set
void
basic_block :: create_e_KILL_set()
{
	quad *quad_ptr;
	int quad_no,exp_no;
	sym_tab_entry *res;
	sym_tab_entry *exp_arg1,*exp_arg2;
	int i;
	expr_set all_kill_set,kill_set;
	expr_set :: iterator iter,iter1;

	all_kill_set = optimizer_ptr->universal_expr.get_expr_set();

	for(i=start;i<end;i++){
		quad_ptr=(*input_quad_arr_ptr)[i];

		/* Eliminate quads like PROC_BEGIN,PROC_END 
		   comparison operators like LT,
			pick only the ones for which res is valid 
		*/
		if(quad_ptr->is_res_valid_for_cse() != 1){
			continue;
		}

		res = quad_ptr -> res ;

		for(iter=all_kill_set.begin();iter != all_kill_set.end();iter++){
			exp_no=(*iter);
			quad_no=optimizer_ptr->universal_expr.get_quad_no(exp_no);
			exp_arg1=(*input_quad_arr_ptr)[quad_no]->arg1;
			exp_arg2=(*input_quad_arr_ptr)[quad_no]->arg2;

			/* Either y or z are defined in the block */
			if((exp_arg1 == res ) || (exp_arg2 == res ) ){

				/* 'y+z' is not generated by the block */
				if(e_GEN.find(exp_no) == e_GEN.end() ){
					kill_set.insert(exp_no);
				}
			}
		}
	}


	e_KILL=kill_set;

}

//! Build DU Chain
void
basic_block :: build_du_chains()
{

	int i;
	sym_tab_entry *arg1,*arg2,*res;
	quad_set *qs;
	quad_var_pair qv;
	quad *quad_ptr;
	du_set tmp_OUT;

	tmp_OUT=du_OUT;

	for(i=(end-1);i >= start; i--){
		quad_ptr=(*input_quad_arr_ptr)[i];

		if(quad_ptr->is_op_valid_for_live_analysis() != 1 ){
			continue;
		}



		arg1=quad_ptr->arg1;
		arg2=quad_ptr->arg2;
		res = quad_ptr->res;

		if(res != NULL ){
			if(quad_ptr->is_res_valid_for_cse() == 1 ){
				qs = new quad_set();
				*qs = get_uses_for_def(tmp_OUT,res);
				optimizer_ptr->quad_info_arr[i]->du_chains[0]=qs;
			}
		}

		if(arg1 != NULL ){
			qv=make_pair(i,arg1);
			tmp_OUT.insert(qv);
		}

		if((arg2 != arg1 ) && (arg2 != NULL ) ){
			qv=make_pair(i,arg2);
			tmp_OUT.insert(qv);
		}

	}

}

//! Get the uses for a given identifier
quad_set
basic_block :: get_uses_for_def(du_set &all_uses,sym_tab_entry *s)
{
	du_set :: iterator iter,tmp_iter;
	quad_var_pair qv;
	quad_set uses;

	for(iter=all_uses.begin();iter != all_uses.end(); ){
		qv= (*iter);

		if(qv.second == s ){
			/*
			tmp_iter = iter++;
			iter--;
			*/
			uses.insert(qv.first);
			all_uses.erase(iter++); // invalidates iter
//			iter = tmp_iter ;
		}else{
			iter ++;
		}
	}
	return(uses);
}

//! Print the DU Chain
void
basic_block :: print_du_chains()
{
	int i;
	quad *quad_ptr;
	quad_set *qs;
	sym_tab_entry *res;

	for(i=start;i<end;i++){
		quad_ptr=(*input_quad_arr_ptr)[i];
		res= quad_ptr -> res;

		if((res != NULL )  && ((qs=optimizer_ptr->quad_info_arr[i]->du_chains[0]) != NULL) ){
			printf("du_chain(%d,%s) :",i,res->name.c_str() );
			print_quad_set(qs);
			printf("\n");
		}
	}
}

//! Print the live_OUT set
void
basic_block :: print_liveness()
{
	int i;
	vset vs;

	for(i=start;i<end;i++){
		vs=optimizer_ptr->quad_info_arr[i]->live_out;
		printf("live_OUT(%d) ={ ",i );
		print_v_set(vs);
		printf("}\n");
	}
}

//! Set the dominators
void
basic_block :: set_dominators(quad_set *d)
{
	dominators = *d;
}

//! Get the Back Edges 
vector <edge_t>
basic_block :: get_back_edges()
{
	int i,n;
	vector <edge_t> bedges;
	int head,tail;

	n = successors.size();

	tail = block_no;
	for(i=0;i<n;i++){
		head= successors[i];
		if(dominators.find(head) != dominators.end() ){
			bedges.push_back(make_pair(head,tail));
		}
	}
	return(bedges);
}

//! Extrapolate live_OUT to individual quads
void
basic_block :: xtrapolate_liveness()
{

	int i;
	quad *quad_ptr;
	sym_tab_entry *arg1,*arg2,*res;
	vset defs; /* definitely assigned values before use */
	vset uses; /* used before assigning values */
	vset in;
	vset out;
	vset tmp;

	vset *vs;
	vset :: iterator iter;
	sym_tab_entry *v;


	/* adjust live_OUT to take care of Globals */
	find_all_vars_used_in_block();
	vs=&all_vars_used;

	for(iter=vs->begin();iter !=vs->end() ; iter ++){
		v=(*iter);
		if(v->is_global() == 1 ){
			live_OUT.insert(v);
		}
	}

	out=live_OUT;

	/* The iteration begins with 'out' already populated */
	for(i=(end-1);i>=start;i--){
		quad_ptr=(*input_quad_arr_ptr)[i];

		if(quad_ptr->is_op_valid_for_live_analysis() == 0 ){
			optimizer_ptr->quad_info_arr[i]->live_out=out;
			continue;
		}

		/* remember 'out' is already populated */
		optimizer_ptr->quad_info_arr[i]->live_out=out;
		in.clear();
		defs.clear();
		uses.clear();
		tmp.clear();

		arg1=quad_ptr -> arg1;
		arg2=quad_ptr -> arg2;
		res=quad_ptr->res;

		if( (arg1 != NULL) && (arg1->is_it_a_constant() != 1) ){

			if(defs.find(arg1) == defs.end() ){
				uses.insert(arg1);
			}
		}

		if((arg2 != NULL) && (arg2->is_it_a_constant() != 1) ){
			if(defs.find(arg2) == defs.end() ){
				uses.insert(arg2);
			}
		}
			
		if((res != NULL) && (quad_ptr->is_res_valid_for_live_analysis() == 1)){
			if(uses.find(res) == uses.end() ){
				defs.insert(res);
			}
		}

		/* in[I] = ( out[I] - def[I] ) U use[I] */
		make_diff(out,defs,tmp); 
		make_union(uses,tmp,in);

		/* for the next iteration */
		out=in;
	}
}
	

int dag_node :: dag_counter;


//! DAG node constructor
dag_node::dag_node(oprtr op,dag_node *l, dag_node *r,dag_node *a,basic_block *b)
{
	label = op;
	left_child = l;
	right_child = r;
	id = NULL;
	lval=0;
	selected_identifier = NULL;
	type_of_node = INTERIOR;
	killed=0;
	addl_child=a;
	if(r != NULL ){
		r->parent.insert(this);
	}
	if(l != NULL ){
		l->parent.insert(this);
	}
	if(a != NULL ){
		a->parent.insert(this);
	}

	bb_ptr=b;

	dag_no = dag_counter++;

	eliminated=0;
	processed=0;
}
//! DAG node constructor
dag_node::dag_node(sym_tab_entry *e,basic_block *b)
{
	label = UNKNOWN_OP;
	id = e;
	lval=0;
	selected_identifier = NULL;
	left_child = right_child = NULL;
	type_of_node = LEAF;
	killed=0;
	addl_child=NULL;
	parent.clear();
	eliminated=0;
	processed=0;
	bb_ptr=b;
	dag_no = dag_counter++;
}

//! Mark the parents as killed
void 
dag_node::kill_parents()
{
	dag_node *d;
	set<dag_node *> :: iterator iter;

	for(iter=parent.begin();
			iter != parent.end();iter++){
		d=(*iter);
		d->killed=1;
	}
}

//! Print the DAG node
void 
dag_node::print()
{
	int i,n;
	string str;
	set<dag_node *> :: iterator iter;

	printf("(%p)",this);
	printf("No=%d ",dag_no);
	if(type_of_node == LEAF ){
		printf("(Leaf) sym=%s :",id->name.c_str());
	}else{
		printf("(Operator) : %s ",get_operator_as_string().c_str());
		printf("left:");
		if(left_child==NULL){
			printf("NULL ");
		}else{
			printf("%p ",left_child);
		}
		printf("right:");
		if(right_child==NULL){
			printf("NULL ");
		}else{
			printf("%p ",right_child);
		}
		printf("addl:");
		if(addl_child==NULL){
			printf("NULL ");
		}else{
			printf("%p ",addl_child);
		}
	}
	if(killed){
		printf("(killed)  ");
	}
	n=attached_identifiers.size();
	if(n){
		printf("attached ");
	}
	for(i=0;i<n;i++){
		printf("%s ",attached_identifiers[i]->name.c_str() );
	}
	if(eliminated == 1 ){
			printf("dead store");
	}

	n=parent.size();
	if(n){
		printf("parent ");
		for(iter=parent.begin();iter != parent.end();iter++){
			printf(" %p",(*iter));
		}
		printf("\n");
	}

	printf("\nQuads generated ={ ");
	n=quads_generated.size();
	for(i=0;i<n;i++){
		printf("%d ",quads_generated[i] );
	}
	printf("}\n");

	printf("values visible outside block ={ ");
	n=values_visible_outside_block.size();
	for(i=0;i<n;i++){
		printf("%d ",values_visible_outside_block[i] );
	}
	printf("}\n");

	printf("\n");
}

//! Find the Base Address node
dag_node *
dag_node :: find_base_addr_node()
{
	dag_node *x;

	if(label == ADDR_OF){
		return(this);
	}

	if(left_child != NULL ){
		x=left_child->find_base_addr_node();
		if(x != NULL ){
			return(x);
		}
	}
	if(right_child != NULL ){
		x=right_child->find_base_addr_node();
		if(x != NULL ){
			return(x);
		}
	}
	if(addl_child != NULL ){
		x=addl_child->find_base_addr_node();
		if(x != NULL ){
			return(x);
		}
	}

	return(NULL);
}


//! Get the associated Symbol Table Entry with the DAG
sym_tab_entry *
dag_node :: get_sym_tab_entry()
{

	assert((type_of_node == INTERIOR) || (type_of_node == LEAF) );

	if(type_of_node == INTERIOR){
		return(selected_identifier);
	}else if(type_of_node == LEAF){
		return(id);
	}
	return(NULL);

}

//! Get the Operator of a DAG node in string form
string 
dag_node::get_operator_as_string()
{
	string s;

   /* Three operands - Case 1 */
	switch(label){
    case ADD : s += "ADD" ; break;
    case MUL : s += "MUL" ; break;
    case DIV : s += "DIV" ; break;
    case SUB : s += "SUB" ; break;

    /* Two Operand - with result - Case 2 */
    case UMINUS : s += "UMINUS" ; break;
    case ADDR_OF: s += "ADDR_OF" ; break;
    case ASSIGN : s += "ASSIGN" ; break;

    /* One operand  - with no result - Case 2 */
    case PROC_BEGIN: s += "PROC_BEGIN" ; break;
    case PROC_END: s += "PROC_END" ; break;
    case RETURN: s += "RETURN" ; break; 
	 case RETRIEVE: s += "RETRIEVE" ; break;
    case PARAM: s += "PARAM" ; break;
    case CALL: s += "CALL" ; break;
    case LBL: s += "LBL" ; break;
    case GOTO : s += "GOTO" ; break;

    /* Three Operands - but cannot do cse */
    case L_INDEX_ASSIGN: s += "L_INDEX_ASSIGN" ; break;
    case R_INDEX_ASSIGN: s += "R_INDEX_ASSIGN" ; break;

    /* Two Operand - No result - Case 1 , with result undefined */
    case LT : s += "LT" ; break;
    case GT : s += "GT" ; break;
    case LE : s += "LE" ; break;
    case GE : s += "GE" ; break;
    case EQ : s += "EQ" ; break;
    case NE : s += "NE" ;break;
	 default : s += "UNKNOWN OPERATOR" ;break;
	}

	return(s);

}

//! Add the given identifier to the liast of attached identifiers
void 
dag_node::add_attached_identifier(sym_tab_entry *e)
{
	int i,n;

	/* attached identifiers cannot be CONSTANTS */
	if(e->type->gettype() == CONSTAN){
		assert(0);
		return;
	}

	/* just in case it is already present ? - no duplicates */
	n=attached_identifiers.size();

	for(i=0;i<n;i++){
		if(attached_identifiers[i] == e ){
			return;
		}
	}
	attached_identifiers.push_back(e);

}

//! Remove an identifier from the list of attached identifiers
void 
dag_node::remove_attached_identifier(sym_tab_entry *e)
{
	int found;
	vector <sym_tab_entry *> :: iterator iter,start,end;

	start = attached_identifiers.begin();
	end = attached_identifiers.end();


	found=0;
	for(iter=start;iter < end;iter ++){
		if((*iter) == e ){
			found=1;
			break;
		}
	}
	if(found==1){
		attached_identifiers.erase(iter);
	}
}

//! Create a temporary
sym_tab_entry *
dag_node :: create_temporary()
{
	sym_tab_entry *s,*res;
	sym_tab  *stab;
	
	s = (*input_quad_arr_ptr)[0]->arg1; /* PROC_BEGIN */
	stab=s->sym_tab_ptr;

	res=stab->make_temporary();
	return(res);

}
//! Identify an identifier to be represent the DAG node (aka selected_identifier)
void
dag_node::populate_selected_identifier()
{
	int i,n;
	sym_tab_entry *res;


		selected_identifier = NULL;

		n=attached_identifiers.size();
		if(!n){

			/* NO identifier is attached to this node */
			selected_identifier=create_temporary();
			attached_identifiers.push_back(selected_identifier);
			return;
		}

		/* There is atleast one attached identifier */


		/* Take the first one as the selected one */
		selected_identifier = attached_identifiers[0];


		/* in case, you find a live or global variable - take it */

		for(i=0;i<n;i++){
			res = attached_identifiers[i];
			/*
			if(bb_ptr->live_OUT.find(res) != bb_ptr->live_OUT.end() ){
				selected_identifier = res;
				break;
			}
			*/

			if(res->is_global() == 1){
				selected_identifier = res; 
				break;
			}
				

			if(res ->is_tmp() != 1){
				selected_identifier = res; // Prefer a Non temp
			}
		}
}


//! Regenarate the quads from a DAG node
void
dag_node::regenerate_quad()
{
	sym_tab_entry *arg1,*arg2,*res;

	if(eliminated == 1){
		return;
	}

	processed=1;

	if(type_of_node == LEAF ){
		gen_assign_statements();
		return;
	}

	/* Interior Node */

	arg1=(left_child==NULL)?NULL:left_child->get_sym_tab_entry();
	arg2=(right_child==NULL)?NULL:right_child->get_sym_tab_entry();

	if(!(IS_RES_VALID(label))){

		res = (addl_child == NULL)?NULL:addl_child->get_sym_tab_entry();

		/* quad w.r.t Operator */
		add_to_quad_arr(label, arg1, arg2, res);

		return;

	}

	/* This an operator among 
 	ADD MUL DIV SUB UMINUS ADDR_OF R_INDEX_ASSIGN
	*/
	assert(IS_RES_VALID(label));

	/* populate selected identifier */
	populate_selected_identifier();

	res = selected_identifier;

	/* checking if we are overstepping the leaf */
	chap_ex=get_example_flag();
	if(chap_ex != 0x0702 ){
		check_for_overwrite(res);
	}

	/* quad w.r.t Operator */
	add_to_quad_arr(label, arg1, arg2, res);

	gen_assign_statements();

}


//! Generate the extra assign statements, if there are multiple live ids
void
dag_node::gen_assign_statements()
{
	int i,n;
	sym_tab_entry *arg1,*arg2,*res ;
	int selected;

	/* in case there are multiple 'live' identifiers tagged 
	   on to the same node , generate assign statements */

	n=attached_identifiers.size();

	if(type_of_node == LEAF ){
		arg1=id;
	}else{
		arg1 = selected_identifier;
	}

	arg2 = NULL ;

	for(i=0;i<n;i++){
		res = attached_identifiers[i];

		if(arg1 == res ){
			continue;
		}

		selected=0;

		if( !selected){
			//if((bb_ptr->live_OUT.find(res) != bb_ptr->live_OUT.end()) ||
			if((res ->is_tmp() != 1) ||
				(res->is_global() == 1)){
				selected=1;
			}
		}

		if(selected){
			chap_ex=get_example_flag();
			if(chap_ex != 0x0702 ){
				check_for_overwrite(res);
			}
			add_to_quad_arr(ASSIGN, arg1, arg2, res);
		}

	}
}

//! Check for Overwriting of leaves
void
dag_node :: check_for_overwrite(sym_tab_entry *res)
{
	int i,n;
	dag_node *dag_node_ptr,*d;
	set<dag_node *> :: iterator iter;
	sym_tab_entry *sptr;
	int gen_assign=0;


	n = bb_ptr->leaves.size();

	dag_node_ptr=NULL;

	for(i=0;i<n;i++){
		d=bb_ptr->leaves[i];
		if(d->id == res ){
			dag_node_ptr = d;
			break;
		}
	}

	if(dag_node_ptr){
		gen_assign=0;
		/* even if one of the parents is not processed 
		   then generate assign statements */
		n=dag_node_ptr->parent.size();
		for(iter=dag_node_ptr->parent.begin();
				iter != dag_node_ptr->parent.end();iter++){
			d=(*iter);
			if(d->processed == 0 ){
				gen_assign=1;
				break;
			}
		}

		if(gen_assign){
			sptr=create_temporary();	
			add_to_quad_arr(ASSIGN, res, NULL, sptr);
			/* replace the 'id' with temporary */
			dag_node_ptr->id = sptr;
		}
	}
}
				
	
//! Eliminate the DAG node, if there are no parents and attached ids are temporary
void
dag_node::elim_dead_code()
{
	int i,n;
	sym_tab_entry *id;

	if (parent.size()==0){ /* No Parent */
		n=attached_identifiers.size();
		if(n > 0 ){
			for(i=0;i<n;i++){
				id=attached_identifiers[i];
				/* if it's not live or a global */
				if (id->is_tmp() != 1){
					/* even if there is one user defined var
					   we cannot eliminate it */
					return;
				}
			}
			/* No live variable associated with it */
			//printf("removing dag no=%d \n",dag_no);	
			eliminated=1;
		}
	}

}

//! Perform Constant Folding on a DAG node
void
dag_node :: perform_constant_folding()
{
	int val1,val2,res;
	sym_tab_entry *sptr;

	if((label == MUL) || (label == ADD) ){
		if(
			(left_child ->type_of_node == LEAF ) &&
			(right_child -> type_of_node == LEAF ) ){

			if( (left_child->id ->type->gettype() == CONSTAN ) &&
				 (right_child->id ->type->gettype() == CONSTAN ) ){

				 val1=atoi(left_child->id->name.c_str());
				 val2=atoi(right_child->id->name.c_str());

				 if(label == ADD){
				 	res=val1+val2;
				 }else if(label == MUL){
				 	res=val1*val2;
				 }

				sptr=make_const_sym_tab_entry(res);


				label = UNKNOWN_OP;
				id = sptr;
				left_child = right_child = NULL;
				type_of_node = LEAF;
				addl_child=NULL;

	//				 printf("Can fold a constant\n");

			}
		}
	}

	if((label == EQ ) ){
		if(
			(left_child ->type_of_node == LEAF ) &&
			(right_child -> type_of_node == LEAF ) ){

			if( (left_child->id ->type->gettype() == CONSTAN ) &&
				 (right_child->id ->type->gettype() == CONSTAN ) ){

				 val1=atoi(left_child->id->name.c_str());
				 val2=atoi(right_child->id->name.c_str());

				 if(val1 == val2){
				 	label=GOTO;
					id = NULL;
					left_child = addl_child;
					right_child = NULL;
					type_of_node = INTERIOR;
					addl_child=NULL;
				}else{
					eliminated=1;
				}
			}
		}
	}



}

//! Add a quad to the regenarated quads
quad *
dag_node :: add_to_quad_arr(oprtr lbl,
                             sym_tab_entry *arg1,
									  sym_tab_entry *arg2,
									  sym_tab_entry *res)
{
	int curr_quad_no;
	quad *quad_ptr;

	quad_ptr=new quad(lbl, arg1, arg2, res);

	quad_ptr->check_sanity();

	curr_quad_no = (*output_quad_arr_ptr).size();

	/* record quad no's generated for this dag node */
	quads_generated.push_back(curr_quad_no); 

	output_quad_arr_ptr->push_back(quad_ptr);

	return(quad_ptr);
}

//! AE clear
void
avail_expr :: clear()
{
	avail_expr_set.clear();
	expr_to_quads.clear();
}

//! Get the AE, given the quad number
int 
avail_expr :: get_avail_expr_no(int quad_no)
{
	int i;
	int n;
	quad *quad_ptr,*quad_ptr1;
	int q_no;

	n = avail_expr_set.size();

	quad_ptr1=(*input_quad_arr_ptr)[quad_no];
	for(i=0;i<n;i++){
		q_no=avail_expr_set[i];
		quad_ptr=(*input_quad_arr_ptr)[q_no];

		if((quad_ptr -> arg1 == quad_ptr1->arg1) &&
			(quad_ptr -> arg2 == quad_ptr1->arg2) && 
			(quad_ptr -> op == quad_ptr1->op)){
			return(i);
		}

	}
	return(-1);

}

//! Get the AE, given the quad number
int
avail_expr :: get_quad_no(int exp_no)
{
	int q_no;

	q_no=avail_expr_set[exp_no];

	return(q_no);
}


//! Insert an AE into the list of AE's
int
avail_expr :: insert_avail_expr(int quad_no)
{
	int n;
	int x;
	quad_set *qs_ptr;

	x=get_avail_expr_no(quad_no) ;

	if(x != -1 ){
		expr_to_quads[x]->insert(quad_no);
		return(x); /* already exists */
	}
		
	n = avail_expr_set.size();
	avail_expr_set.push_back(quad_no);
	qs_ptr= new quad_set();
	expr_to_quads.push_back(qs_ptr);
	qs_ptr->insert(quad_no);

	return(n);

}

//! Print all the AE's
void
avail_expr :: print()
{
	int i,j;
	int n;
	quad *quad_ptr;
	int quad_no;
	quad_set *qs;
	quad_set :: iterator iter;

	n = avail_expr_set.size();

	printf("Available Expressions :\n");
	for(i=0;i<n;i++){
		quad_no=avail_expr_set[i];
		quad_ptr=(*input_quad_arr_ptr)[quad_no];
		printf("%d. %s ",i,(quad_ptr->as_string()).c_str());
		qs=expr_to_quads[i];
		printf("{ ");
	 	for(iter=qs->begin();iter != qs->end();iter++){
			j=(*iter);
			printf(" %d",j);
		}
		printf(" }\n");

	}

	return;

}

//! Get the AE set
expr_set
avail_expr :: get_expr_set()
{
	expr_set e;
	int i,n;
	n = avail_expr_set.size();
	for(i=0;i<n;i++){
		e.insert(i);	
	}
	return(e);
}

//! Print the AE in Human readbale format
string
avail_expr :: as_string(int exp_no)
{
	quad *quad_ptr;
	int q_no;
	string s;

	q_no=avail_expr_set[exp_no];
	quad_ptr = (*input_quad_arr_ptr)[q_no];

	s = quad_ptr->arg1->name + quad_ptr->get_op() + quad_ptr->arg2->name;

	return(s);
}

	

//! Attach UD chain info for a given quad
int 
ud_chain :: insert_ud_chain(int quad_no,sym_tab_entry *s,quad_set *qs)
{
   var_quad_set_map *m;

   m=ud_chain_vector[quad_no];

   if(m == NULL ){
      m = new var_quad_set_map();
      ud_chain_vector[quad_no]=m;
   }

   (*m)[s]=qs;

	return(0);
}


//! Get the UD chain info for a given quad and Identifier
quad_set * 
ud_chain :: get_ud_chain(int quad_no,sym_tab_entry *s)
{
   var_quad_set_map *m;
   quad_set *qs;

   m=ud_chain_vector[quad_no];

   if(m == NULL ){
      return(NULL);
   }

   qs=(*m)[s];

   return(qs);
}

/*  optimize.cc */

static vector <quad *>prog_quads;
		

/*
	1.Common subexpression elimination
	2.Copy propagation
	3.Dead-code elimination
	4.Constant folding

	1.Use of algebraic identities
*/

/* Entry point to Optimization */

//! Entry point to IC Optimization
int optimize_intermediate_code(int optimize,int verbose)
{
	optimize_ic *ic_optimizer;
	quad *quad_ptr;
	int i,n;
	int i1,n1;
	vector <quad *>tmp_input_proc_quads;
	vector <quad *>tmp_output_proc_quads;

	i=0;

	tmp_input_proc_quads.clear();
	tmp_output_proc_quads.clear();
	prog_quads.clear();

	n = quad_arr.size();

	while ( i < n ){

		ic_optimizer = new optimize_ic() ;

		for(;i<n;i++){
			quad_ptr = quad_arr[i];
			tmp_input_proc_quads.push_back(quad_ptr);
			if(quad_ptr->op == PROC_END ){
				break;
			}
		}
		i++;

		ic_optimizer -> optimize_proc(&tmp_input_proc_quads,optimize,verbose);
		ic_optimizer -> copy_quads(&tmp_output_proc_quads);

		n1= tmp_output_proc_quads.size();
		for(i1=0;i1<n1;i1++){
			quad_ptr = tmp_output_proc_quads[i1];
			prog_quads.push_back(quad_ptr);
		}


		tmp_input_proc_quads.clear();
		tmp_output_proc_quads.clear();

	}

	final_quad_arr_ptr = &prog_quads ;

	return(0);

}

//! IC Optimization
optimize_ic::optimize_ic( )
{
	universal_expr.clear();
	quad_info_arr.clear();
	no_of_blocks=0;
}

//! Copy the Output Quads
void
optimize_ic:: copy_quads( vector <quad *>*outp_q_ptr)
{
		int n,i;
		quad *quad_ptr;

		n= output_quad_arr_ptr->size();
		for(i=0;i<n;i++){
			quad_ptr = (*output_quad_arr_ptr)[i];
			outp_q_ptr ->push_back(quad_ptr);
		}
		return;
}

//! Optimize the IC one procedure at a time
int
optimize_ic:: optimize_proc( vector <quad *>*inp_q_ptr,int optimize,int verbose)
{

	int change=0;
	int i=1;
	vector <quad *> *before_opt;
	vector <quad *> *after_opt;;


	change=1;
	orig_input_quads=input_quad_arr_ptr = inp_q_ptr;

	if(verbose){
		printf("TAC Before optimization \n");
		print_tac(orig_input_quads);
	}

	while(change == 1) {
	
		change=0;


		before_opt=input_quad_arr_ptr;



		/* This splits into BB, does liveness analysis, 
		   available expr_analysis, rd analysis etc */

		/* input=input_quad_arr_ptr, no output quads */
		//do_data_flow_analysis();

		/* Local Optimizations */
		perform_local_optmizations();

		if(verbose){
			printf("TAC After Local Optimization \n");
			print_tac(&quad_arr_after_local_opt);
		}

		if(optimize == LOCAL ){
			return(0);
		}


		input_quad_arr_ptr = &quad_arr_after_local_opt;


		/* Do Global Optimization */
		perform_global_optmizations(optimize);

		after_opt=output_quad_arr_ptr;

		change=compare_quad_arrays(before_opt,after_opt);

		/*
		printf("iteration %d \n",i);
		printf("before_opt \n");
		print_tac(before_opt);
		printf("after_opt \n");
		print_tac(after_opt);
		*/


		/* Maximum of 5 iterations */
		if ((change == 0 ) || (i >= 1) ){
			break;
		}

		i++;

		input_quad_arr_ptr=output_quad_arr_ptr;

	}


	input_quad_arr_ptr=output_quad_arr_ptr;
	output_quad_arr_ptr = new vector <quad *> ;

	remove_unneccesary_labels();

	if(verbose){
		printf("TAC After (Local and Global) optimization \n");
		print_tac(output_quad_arr_ptr);
	}

	return(0);

}

/* Clear all the DS related to Data Flow Analysis */

//! Clear all the DS related to Data Flow Analysis 
void
optimize_ic :: clear_all()
{
	int i;
	basic_block *bb_ptr;

	for(i=0;i<no_of_blocks;i++){
		bb_ptr = basic_blocks_arr[i];
		delete(bb_ptr);
	}
	basic_blocks_arr.clear();

	orig_input_quads=NULL;
	no_of_blocks=0;
	universal_copy_stmts.clear();
	quad_info_arr.clear();
	loop_info_arr.clear();
	block_starts.clear();

	universal_expr.clear();
	gcse_quads.clear();
	copy_prop_quads.clear();

}

//! Compare Two quad arrays
int
optimize_ic::compare_quad_arrays(vector <quad *> *a,vector <quad *> *b)
{
	int i;
	int n1,n2;
	quad *quad_ptr1,*quad_ptr2;

	n1=a->size();
	n2=b->size();

	if(n1 != n2 ){
		return(1);
	}

	for(i=0;i<n1;i++){
		quad_ptr1=(*a)[i];
		quad_ptr2=(*b)[i];

		if(quad_ptr1->op != quad_ptr2->op ){
			return(1);
		}

		if(quad_ptr1->arg1 != quad_ptr2->arg1 ){
			return(1);
		}

		if(quad_ptr1->arg2 != quad_ptr2->arg2 ){
			return(1);
		}
	}

	return(0);
}


//! Get the Block number, given the quad number
int
optimize_ic::get_block_no(int quad_no)
{
	int i,n;
	int blk_st,next_blk_start;


	//copy(block_starts.begin(), block_starts.end(), ostream_iterator<int>(cout, "\n"));
	n= block_starts.size();
	for(i=0;i<(n-1);i++){
		blk_st=block_starts[i];
		next_blk_start=block_starts[i+1];

		if(( quad_no >=blk_st ) && (quad_no < next_blk_start ) ){
			return(i);
		}
	}

	assert(0);

	return(0);
}

/* Convert to basic blocks */

//! Split the quads into basic blocks
void 
optimize_ic::split_into_basic_blocks()
{
	int n,i;
	string quad_str;
	quad *current_quad,*previous_quad;
	int bno;
	int start;
	basic_block *bb_ptr;
	int leader;
	string s;
	lbl_blk_map l1;
	sym_tab_entry *sptr;

	if(tac_not_valid){
		return;
	}

	n = input_quad_arr_ptr->size();

	block_starts.clear();
	sptr = (*input_quad_arr_ptr)[0]->arg1; /* PROC_BEGIN */
   sym_tab_ptr=sptr->sym_tab_ptr;

	/* Identify Leaders */

	/* The first statement is a leader */
	bno = 0;
	start = 0;
	bb_ptr = new basic_block(bno,start,this);
	block_starts.push_back(start);

	previous_quad = NULL;


	for(i=1;i<n;i++) {
		leader=0;
		current_quad = (*input_quad_arr_ptr)[i] ;

		/* Any statement that is a target of conditional 
		or unconditional goto is a leader */
		if(current_quad -> op == LBL ){
			leader=1;
			l1[current_quad ->arg1]=bno+1;
		}

		/* Any statement that follows a goto statement or a 
		conditional goto statement is a leader */
		if( (previous_quad != NULL ) && (
			(previous_quad->op == GOTO) ||
			((previous_quad->op >= LT) && (previous_quad->op <= NE))) ){

			leader=1;

		}
		if(leader==1){
			bb_ptr -> end = i ;
			basic_blocks_arr.push_back(bb_ptr);

			bno ++;
			bb_ptr = new basic_block(bno,i,this);
			block_starts.push_back(i);
		}

		previous_quad = current_quad;
	}

	/* adding the last block */
	bb_ptr->end=n;
	basic_blocks_arr.push_back(bb_ptr);
	bno ++;

	/* updating the number of blocks */
	no_of_blocks = bno;

	build_succ_pred_relationships(l1);

	/* Mark dead blocks */
	mark_dead_blocks();

	/* check if it is an example */
	chap_ex=get_example_flag();
	if(chap_ex == 0x0701 ){ /* chap7, example 1 */
		int i;
		basic_block *bb_ptr;

		printf("Quads After Splitting into Basic Blocks \n");
		for(i=0;i<no_of_blocks;i++){
			bb_ptr = basic_blocks_arr[i];
			bb_ptr->print_quads();
		}
		exit(0);
	}
}


//! Mark the Dead Blocks
void 
optimize_ic::mark_dead_blocks()
{
	basic_block *bb_ptr;
	int i;

	/* identify blocks which dont have a successor nor
	   predeccesor */

	for(i=0;i<no_of_blocks;i++){
		bb_ptr = basic_blocks_arr[i];
	 	if(bb_ptr->predecessors.size() == 0 ){
			if((*input_quad_arr_ptr)[bb_ptr->start]->op != PROC_BEGIN){
				bb_ptr->dead=1;
			}
		}
	 	if(bb_ptr->successors.size() == 0 ){
			if((*input_quad_arr_ptr)[bb_ptr->end-1]->op != PROC_END){
				bb_ptr->dead=1;
			}
		}
	}

}

//! Build the Successor and Predecessor Relationship
void 
optimize_ic::build_succ_pred_relationships(lbl_blk_map l1 )
{
	int i;
	basic_block *bb_ptr;
	basic_block *target_bb_ptr;
	int quad_no;
	quad *quad_ptr;
	int blk;

	for(i=0;i<no_of_blocks;i++){
		bb_ptr = basic_blocks_arr[i];
		quad_no=bb_ptr->end - 1;
		quad_ptr = (*input_quad_arr_ptr)[quad_no];

		if(quad_ptr->op == GOTO ){

			blk=l1[quad_ptr->arg1];
			(bb_ptr->successors).push_back(blk);
			target_bb_ptr=basic_blocks_arr[blk];
			(target_bb_ptr->predecessors).push_back(i);

		} else if ((quad_ptr->op >= LT) && (quad_ptr->op <= NE)){

			blk=l1[quad_ptr->res];
			bb_ptr->successors.push_back(blk);
			target_bb_ptr=basic_blocks_arr[blk];
			(target_bb_ptr->predecessors).push_back(i);


			bb_ptr->successors.push_back(i+1);
			target_bb_ptr=basic_blocks_arr[i+1];
			(target_bb_ptr->predecessors).push_back(i);
		}else{

			if((i+1) < no_of_blocks ){
				bb_ptr->successors.push_back(i+1);
				target_bb_ptr=basic_blocks_arr[i+1];
				(target_bb_ptr->predecessors).push_back(i);
			}
		}
	}

}


//! Print the Blocks
void 
optimize_ic::print_blocks()
{
	int i;
	basic_block *bb_ptr;

	for(i=0;i<no_of_blocks;i++){
		bb_ptr = basic_blocks_arr[i];
		bb_ptr->print();
	}
}

//! Make the DAGS from Basic Blocks
void 
optimize_ic::make_dags_from_basic_blocks()
{
	int i;
	basic_block *bb_ptr;

//	dag_node :: dag_counter=0;

	for(i=0;i<no_of_blocks;i++){
		bb_ptr = basic_blocks_arr[i];
		bb_ptr->construct_dag();
	}
}

//! Print DAGS
void 
optimize_ic::print_dags()
{
	int i;
	basic_block *bb_ptr;

	for(i=0;i<no_of_blocks;i++){
		bb_ptr = basic_blocks_arr[i];
		printf("BLOCK =%d \n",i);
		bb_ptr->print_dag();
	}


}

/* input=input_quad_arr_ptr, output=quad_arr_after_local_opt*/

//! Perform Local Optimization
void
optimize_ic::perform_local_optmizations()
{
	int i;
	basic_block *bb_ptr;
	int change;

	output_quad_arr_ptr = &quad_arr_after_local_opt;

	quad_arr_after_local_opt.clear();

	/* already split into basic blocks in data flow analysis */

	/* clear all the Data structures */
	clear_all();

	/* split into basic blocks */
	split_into_basic_blocks();

	/* construct dag for each of the blocks */
	make_dags_from_basic_blocks();


	for(i=0;i<no_of_blocks;i++){
		bb_ptr = basic_blocks_arr[i];
		bb_ptr->constant_folding();
		bb_ptr->remove_dead_code();
	}


	/* build the tac after eliminating 
	   common sub expressions */
	build_tac_from_dags();

	/* checking if any changes took place due to local opt */
	change=compare_quad_arrays(input_quad_arr_ptr,output_quad_arr_ptr);

	/*
	if(change){
		printf("xformed in local optimization\n");
		print_tac(output_quad_arr_ptr);
	}
	*/

	//print_dags();

	/* delete */
	for(i=0;i<no_of_blocks;i++){
		bb_ptr = basic_blocks_arr[i];

		delete(bb_ptr);
	}
	no_of_blocks=0;


}



/* input=input_quad_arr_ptr, output=output_quad_arr_ptr
   the output is malloced by new() */

//! Perform Global Optimization
void
optimize_ic::perform_global_optmizations(int optimize)
{

	/* Global common subexpression elimination */

	if(optimize & GCSE ){
		do_data_flow_analysis();
		output_quad_arr_ptr = new vector <quad *> ;
		global_cse_elimination();
		input_quad_arr_ptr=output_quad_arr_ptr;
	}

	if(optimize & GCP ){
		do_data_flow_analysis();
		output_quad_arr_ptr = new vector <quad *> ;
		global_copy_propagation();
		input_quad_arr_ptr=output_quad_arr_ptr;
	}

	/* loop related optimization */
	if(optimize & GCM ){
		do_data_flow_analysis();
		output_quad_arr_ptr = new vector <quad *> ;
		code_motion();
		input_quad_arr_ptr=output_quad_arr_ptr;
	}

	/* Dead Code Elimination */
	if(optimize & GDCE ){
		do_data_flow_analysis();
		output_quad_arr_ptr = new vector <quad *> ;
		dead_code_elimination();
		input_quad_arr_ptr=output_quad_arr_ptr;
	}


	/* Peephole optimization */
	if(optimize & GPO ){
		output_quad_arr_ptr = new vector <quad *> ;
		peephole_optimization();
	}


	return;
}

//! Global Dead Code Elimination
void
optimize_ic :: dead_code_elimination()
{
	int i,n;
	quad *quad_ptr;
	vset vs;
	sym_tab_entry *res;

	n=input_quad_arr_ptr->size();
	for(i=0;i<n;i++){

		/* get the quad */
		quad_ptr=(*input_quad_arr_ptr)[i];

		if(quad_ptr->is_op_valid_for_live_analysis() == 0 ){
			output_quad_arr_ptr->push_back(quad_ptr);
			continue;
		}

		res=quad_ptr->res;

		if((res != NULL) && (quad_ptr->is_res_valid_for_live_analysis() == 1)){
			vs=quad_info_arr[i]->live_out;

			/* 
				eliminate a quad, if the live_out does not contain
			   the res
			*/
			if(vs.find(res) != vs.end() ){
				output_quad_arr_ptr->push_back(quad_ptr);
			}

		}else{
			output_quad_arr_ptr->push_back(quad_ptr);
		}

	}
}



//! Peephole Optimization
void
optimize_ic :: peephole_optimization()
{
	int i,n;
	quad *quad_ptr,*quad_ptr1;
	quad *quad1;
	int change,add_identity,mul_identity;
	sym_tab_entry *arg1,*arg2,*res;
	sym_tab_entry *const_arg,*other_arg;
	sym_tab_entry *target_lbl;
	int op,val;
	int xformed=0;

	i=0;
	n = input_quad_arr_ptr->size();
	xformed=0;
	while(i < n){
		/* get the quad */
		quad_ptr=(*input_quad_arr_ptr)[i];
		op=quad_ptr->op;
		arg1=quad_ptr->arg1;
		arg2=quad_ptr->arg2;
		res=quad_ptr->res;

		change=0;

		/* replace y = x + 0 with y = x */
		if(op == ADD){
			add_identity=0;
			if(arg1->type->gettype() == CONSTAN){
				val=atoi(arg1->name.c_str());
				const_arg=arg1;
				other_arg=arg2;
				add_identity=1;
			} else if( (arg2->type->gettype() == CONSTAN)){
				val=atoi(arg2->name.c_str());
				const_arg=arg2;
				other_arg=arg1;
				add_identity=1;
			}

			if(add_identity==1){
				if(val == 0 ){
					quad_ptr1= new quad(ASSIGN,other_arg,NULL,res);
					change=1;
				}
			}
		}

		/* replace y = x * 0 with y = 0 */
		/* replace y = x * 1 with y = x */

		if(op == MUL){
			mul_identity=0;
			if(arg1->type->gettype() == CONSTAN){
				val=atoi(arg1->name.c_str());
				const_arg=arg1;
				other_arg=arg2;
				mul_identity=1;
			} else if( (arg2->type->gettype() == CONSTAN)){
				val=atoi(arg2->name.c_str());
				const_arg=arg2;
				other_arg=arg1;
				mul_identity=1;
			}

			if(mul_identity==1){
				if(val == 0 ){
					quad_ptr1= new quad(ASSIGN,const_arg,NULL,res);
					change=1;
				}else if(val == 1) {
					quad_ptr1= new quad(ASSIGN,other_arg,NULL,res);
					change=1;
				}

			}
		} 

		/* replace 
				goto lbl1
				label lbl1

				with
				label lbl1
		*/
		if(quad_ptr->op == GOTO ){
			target_lbl=quad_ptr->arg1;
			if((i+1) < n ){
				quad1=(*input_quad_arr_ptr)[i+1];
				if(quad1->op == LBL ){
					if(target_lbl == quad1->arg1 ){
						quad_ptr1=NULL;
						change=1;
					}
				}
			}
		}

		if(change==1){
			if(quad_ptr1 != NULL ){
				(*output_quad_arr_ptr).push_back(quad_ptr1);
			}
			xformed=1;
		}else{
			(*output_quad_arr_ptr).push_back(quad_ptr);
		}
		i++;
	}

	/*
	if(xformed==1){
		printf("xformed in peep hole optimization\n");
		print_tac(output_quad_arr_ptr);
	}
	*/
}

//! Remove unnecessary labels in the IC
void
optimize_ic :: remove_unneccesary_labels()
{
	int i,n;
	sym_tab_entry *target_lbl;
	set<sym_tab_entry *> vq;
	quad *quad_ptr;
	int xformed=0;

	n=input_quad_arr_ptr->size();
	for(i=0;i<n;i++){
		quad_ptr=(*input_quad_arr_ptr)[i];
		if((quad_ptr->op >= LT ) && (quad_ptr->op <=NE) ){
			target_lbl=quad_ptr->res;
			vq.insert(target_lbl);
		}else if(quad_ptr->op == GOTO){
			target_lbl=quad_ptr->arg1;
			vq.insert(target_lbl);
		}
	}


	for(i=0;i<n;i++){
		quad_ptr=(*input_quad_arr_ptr)[i];

		if(quad_ptr->op == LBL ){
			if(vq.find(quad_ptr->arg1) != vq.end() ){
				output_quad_arr_ptr->push_back(quad_ptr);
			}else{
				xformed=1;
			}
		}else{
			output_quad_arr_ptr->push_back(quad_ptr);
		}
	}

	/*
	if(xformed==1){
		printf("xformed in remove unnecesary labels \n");
		print_tac(output_quad_arr_ptr);
	}
	*/

}


//! Code Motion Optimization
void
optimize_ic :: code_motion()
{

	int i,n;
	loop_info *loop_info_ptr;
	quad_set tmp_set;
	int change;
	int ret;

	change=1;
	while(change){
		change=0;
		n= loop_info_arr.size();

		/* Mark the Loop Invariants */
		for(i=0;i<n;i++){
			loop_info_ptr=loop_info_arr[i];
			tmp_set = loop_info_ptr->loop_invariants ;
			mark_loop_invariants(loop_info_ptr);
			if(loop_info_ptr->loop_invariants != tmp_set){
				change=1;
			}
		}
	}

	/* we have identified loop invariants, now check the
	   conditions */
	check_conds_for_code_motion();

	/* xform the input */
	ret=xform_input_for_code_motion();

	/*
	if(ret){
		printf("xformed in code_motion \n");
		print_tac(output_quad_arr_ptr);
	}
	*/
}

//! Transform the input for Loop Invariant Code motion 
int
optimize_ic :: xform_input_for_code_motion()
{
	typedef list<quad *> quad_list_t;
	map <int, quad_list_t *,less <int> > oldquad2_new_quad;
	quad_list_t *ql;
	quad_set code_motion_marked_quads;
	quad_set code_motion_remove_quads;
	quad_set :: iterator iter1;
	quad_list_t :: iterator iter2;
	int hdr_blk;
	basic_block *bb_ptr;
	int i,n;
	loop_info *loop_info_ptr;
	int st;
	int quad_no;
	int xformed=0;


	quad *quad_ptr,*quad_ptr1;
	sym_tab_entry *arg1;

	extern sym_tab_entry *newlbl();

	n= loop_info_arr.size();

	/* Preheaders */
	for(i=0;i<n;i++){
		loop_info_ptr=loop_info_arr[i];

		if(loop_info_ptr->can_be_moved_to_preheader.size() <= 0 ){
			continue;
		}

		hdr_blk=loop_info_ptr->hdr_blk;
		bb_ptr = basic_blocks_arr[hdr_blk];

		st=bb_ptr->start;
		code_motion_marked_quads.insert(st);

		ql = new quad_list_t;
		arg1=newlbl();
		quad_ptr = new quad(LBL,arg1,NULL,NULL);
		ql->push_back(quad_ptr);


		for(iter1=loop_info_ptr->can_be_moved_to_preheader.begin();
				iter1!=loop_info_ptr->can_be_moved_to_preheader.end();
				iter1++){

			quad_no=(*iter1);
			code_motion_remove_quads.insert(quad_no);
			quad_ptr=(*input_quad_arr_ptr)[quad_no];
			ql->push_back(quad_ptr);
		}

		oldquad2_new_quad[st]=ql;

	}

	n=input_quad_arr_ptr->size();
	for(i=0;i<n;i++){

		/* get the quad */
		quad_ptr=(*input_quad_arr_ptr)[i];

		/* marked quads */
		if(code_motion_marked_quads.find(i) != code_motion_marked_quads.end() ){
			ql=oldquad2_new_quad[i];
			for(iter2=ql->begin();iter2 != ql->end();iter2++){
				quad_ptr1=(*iter2);
				(*output_quad_arr_ptr).push_back(quad_ptr1);
			}
			(*output_quad_arr_ptr).push_back(quad_ptr);
		}else if(code_motion_remove_quads.find(i) != code_motion_remove_quads.end() ){
		}else{
			(*output_quad_arr_ptr).push_back(quad_ptr);
		}
	}

	if((code_motion_marked_quads.size() > 0 ) ||
		(code_motion_remove_quads.size() > 0 ) ){
		xformed=1;
	}
	return(xformed);
}

//! Check the Conditions for moving a Loop Invariant Quad to the Pre-header
void
optimize_ic::check_conds_for_code_motion()
{
	int i,n;
	quad_set qs;
	int cond1,cond2,cond3;
	int quad_no;
	int ret;
	quad_set :: iterator iter1;
	loop_info *loop_info_ptr;

	n=loop_info_arr.size();
	for(i=0;i<n;i++){
		loop_info_ptr=loop_info_arr[i];
		qs=loop_info_ptr->loop_invariants;

		for(iter1=qs.begin();iter1 != qs.end();iter1++){
			quad_no=(*iter1);
			cond1=cond2=cond3=0;

			ret=does_quad_dominate_all_exits(quad_no,loop_info_ptr);
			if(ret==1){
				cond1=1;
			}

			ret=any_other_assign(quad_no,loop_info_ptr);
			if(ret==0){
				cond2=1;
			}

			ret=is_this_the_only_def(quad_no,loop_info_ptr);
			if(ret==1){
				cond3=1;
			}

			/*
			printf(" quad=%d cond1=%d cond2=%d cond3=%d \n",quad_no,cond1,cond2,cond3);
			*/

			if((cond1==1) && (cond2==1) && (cond3==1) ){
				if(loop_info_ptr->check_for_dependent_quads(quad_no,loop_info_ptr->can_be_moved_to_preheader) == 1){
					loop_info_ptr->can_be_moved_to_preheader.insert(quad_no);
				}
			}

		}
	}
	
}

//! One of the Conditions for Loop Invariant Code Motion
int
optimize_ic :: is_this_the_only_def(int quad_no,loop_info *loop_info_ptr)
{
	
	quad_set all_blocks;
	quad *quad_ptr;
	quad_set :: iterator iter;
	int i;
	int blk_no;
	sym_tab_entry *inv_res;
	basic_block *bb_ptr;
	int rd_quad_no;
	quad_set rdefs;
	quad_set :: iterator iter1;
	int ret;


	inv_res=(*input_quad_arr_ptr)[quad_no]->res;

	all_blocks=loop_info_ptr->natural_loop;
	for(iter=all_blocks.begin(); iter != all_blocks.end();iter++){
		blk_no=(*iter);
		bb_ptr=basic_blocks_arr[blk_no];
		for(i=bb_ptr->start;i<bb_ptr->end;i++){

			quad_ptr=(*input_quad_arr_ptr)[i];

			ret=quad_ptr->is_it_used(inv_res);
			if(ret == 0 ){
				continue;
			}

			if(quad_ptr->arg1 == inv_res ){
				rdefs=*(quad_info_arr[i]->ud_chains[0]);
				if(rdefs.size() != 1){
					return(0);
				}
				iter1=rdefs.begin();
				rd_quad_no=(*iter1);

				if(rd_quad_no == quad_no ){
					return(1);
				}
				return(0);
			}

			if(quad_ptr->arg2 == inv_res ){
				rdefs=*(quad_info_arr[i]->ud_chains[1]);
				if(rdefs.size() != 1){
					return(0);
				}
				iter1=rdefs.begin();
				rd_quad_no=(*iter1);

				if(rd_quad_no == quad_no ){
					return(1);
				}
				return(0);
			}

		}
	}

	/* No use in the loop */
	/* We can assume that this is the only reaching definition
	   for a non-existent use !!!
	*/
	return(1);
}
	


//! Another Condition for Loop Invariant Code Motion
int
optimize_ic :: any_other_assign(int quad_no,loop_info *loop_info_ptr)
{
	
	quad_set all_blocks;
	quad *quad_ptr;
	quad_set :: iterator iter;
	int i;
	int blk_no;
	sym_tab_entry *inv_res;
	basic_block *bb_ptr;


	inv_res=(*input_quad_arr_ptr)[quad_no]->res;

	all_blocks=loop_info_ptr->natural_loop;
	for(iter=all_blocks.begin(); iter != all_blocks.end();iter++){
		blk_no=(*iter);
		bb_ptr=basic_blocks_arr[blk_no];
		for(i=bb_ptr->start;i<bb_ptr->end;i++){
			quad_ptr=(*input_quad_arr_ptr)[i];

			if(i == quad_no ){
				continue;
			}

			if(quad_ptr->is_it_defined(inv_res) == 1 ){
				return(1);
			}

		}
	}
	return(0);
}

//! Another Condition for Loop Invariant Code Motion
int
optimize_ic::does_quad_dominate_all_exits(int quad_no,loop_info *loop_info_ptr)
{
	int blk_no;
	quad_set all_exits;
	int exit_blk;
	quad_set :: iterator iter;
	basic_block *bb_ptr;

	blk_no=get_block_no(quad_no);
	all_exits=loop_info_ptr->exits;

	for(iter=all_exits.begin(); iter != all_exits.end();iter++){
		exit_blk=(*iter);
		bb_ptr=basic_blocks_arr[exit_blk];
		if(bb_ptr->dominators.find(blk_no) == bb_ptr->dominators.end() ){
			/* The stmt belongs to a block which is NOT dominating
			   one of the exits
			*/
			return(0);
		}
	}
	return(1);
}
			




//! Identify Quads which are loop Invariants
void
optimize_ic :: mark_loop_invariants(loop_info *loop_info_ptr)
{
	basic_block *bb_ptr;
	int start,end;
	sym_tab_entry *arg1,*arg2;
	int bno;
	loop_t :: iterator iter;
	quad_set :: iterator iter1;
	int i;
	quad *quad_ptr;
	quad_set *rdefs1;
	quad_set *rdefs2;

	int ret;
	int quad_no;
	int dependent_quad_for_arg1,dependent_quad_for_arg2;


	iter = loop_info_ptr->natural_loop.begin();

	/* For every block in the natural loop */
	while(iter != loop_info_ptr->natural_loop.end() ){

		bno=(*iter);

		assert(bno < no_of_blocks);

		bb_ptr=basic_blocks_arr[bno];

		start=bb_ptr->start;
		end=bb_ptr->end;

		for(i=start;i<end;i++){
			quad_ptr = (*input_quad_arr_ptr)[i];

			if(quad_ptr->is_res_valid_for_cse() != 1 ){
				continue;
			}

			/* already present in the loop_invariants */
			if(loop_info_ptr->loop_invariants.find(i) != loop_info_ptr->loop_invariants.end() ){
				continue;
			}
			dependent_quad_for_arg1=dependent_quad_for_arg2=0;

			arg1=quad_ptr->arg1;
			arg2=quad_ptr->arg2;

			/* arguments are constants */
			ret=are_args_const(quad_ptr);
			if(ret == 1 ){
				loop_info_ptr->loop_invariants.insert(i);
				continue;
			}

			/* ud_chain for arg1 */
			if(arg1 != NULL ){
				rdefs1=quad_info_arr[i]->ud_chains[0];


				/* is reaching def for arg1 outside the loop */
				ret=are_rdefs_from_outside_loop(rdefs1,loop_info_ptr->natural_loop);

				/* reaching from inside */
				if(ret == 0 ){
					/*  One reaching definition from another loop invariant 
						 quad 
					 */
					ret=is_it_from_loop(rdefs1,loop_info_ptr->loop_invariants);
					if(ret == 0 ){
						continue;
					}

					/* remember that this can be moved to pre-header only
					   if the dependent quad also moves */
					dependent_quad_for_arg1=1;

				}
			}

			if(arg2 != NULL ){
				rdefs2=quad_info_arr[i]->ud_chains[1];

				/* is reaching def for arg2 outside the loop */
				ret=are_rdefs_from_outside_loop(rdefs2,loop_info_ptr->natural_loop);

				/* reaching from inside */
				if(ret == 0 ){
					ret=is_it_from_loop(rdefs2,loop_info_ptr->loop_invariants);
					if(ret == 0 ){
						continue;
					}
					dependent_quad_for_arg2=1;
				}
			}

			/* All the NON-NULL arguments definitions are reaching from 
			   either (a) outside the loop or (b) One RD from inside 
				the loop
			*/
			loop_info_ptr->loop_invariants.insert(i);

			if(dependent_quad_for_arg1){
				iter1=rdefs1->begin();
				quad_no=(*iter1);
				loop_info_ptr->add_dependent_quads(i,quad_no);
			}

			if(dependent_quad_for_arg2){
				iter1=rdefs2->begin();
				quad_no=(*iter1);
				loop_info_ptr->add_dependent_quads(i,quad_no);
			}
				
		}
		iter++;
	}

}

/* returns 1, if rdefs is only 1 and that too from another
   loop invariant statement */

//! Is the reaching def from another quad within the Loop
int
optimize_ic::is_it_from_loop(quad_set *rdefs,quad_set lp_invariants)
{
	quad_set :: iterator iter;
	int quad_no;

	if(rdefs->size() != 1 ){
		return(0);
	}

	iter=rdefs->begin();

	quad_no=(*iter);

	if(lp_invariants.find(quad_no) != lp_invariants.end() ){
		return(1);
	}

	return(0);
}


//! Is the reaching def from Outside the Loop
int
optimize_ic::are_rdefs_from_outside_loop(quad_set *rdefs,loop_t lp)
{
	
	quad_set :: iterator iter1;
	int blk_no;
	int quad_no;

	/* There is no reaching def, which means that either it is 
	   un-initialized or it comes as a param or it is a global.
		In all of these cases, there is no reaching def within 
		loop - hence it is loop invariant 
	*/


	if(rdefs == NULL ){
		return(1);
	}

	/* check the definition reaching here is from within the loop */
	if(rdefs != NULL ){
		for(iter1=rdefs->begin();iter1 != rdefs->end();iter1++){
			quad_no=(*iter1);
			blk_no=get_block_no(quad_no);
			if(lp.find(blk_no) != lp.end() ){
				return(0);
			}
		}

	}
	return(1);
}

//! Does the quad have constant args
int
optimize_ic::are_args_const(quad *quad_ptr)
{
	sym_tab_entry *arg1,*arg2;
	int no_of_args,no_of_const_args;


	arg1=quad_ptr->arg1;
	arg2=quad_ptr->arg2;

	no_of_args=no_of_const_args=0;

	if(arg1 != NULL ){
		no_of_args++;
		if( arg1->is_it_a_constant() == 1){
			no_of_const_args++;
		}
	}

	if(arg2 != NULL ){
		no_of_args++;
		if( arg2->is_it_a_constant() == 1){
			no_of_const_args++;
		}
	}


	/* mark statements whose args are all constant */
	if(no_of_args == no_of_const_args){
		return(1);
	}

	return(0);
}



//! Perform Global Copy Propagation
void
optimize_ic::global_copy_propagation()
{
	int i;
	basic_block *bb_ptr;
	int ret;

	for(i=0;i<no_of_blocks;i++){
		bb_ptr = basic_blocks_arr[i];
		bb_ptr->identify_copy_prop_stmts();
	}

	ret=xform_input_for_copy_prop();

	/*
	if(ret){
		printf("xformed in copy prop\n");
		print_tac(output_quad_arr_ptr);
	}
	*/
}

//! Perform Global Subexpression Eliminaton
void
optimize_ic::global_cse_elimination()
{
	int i;
	basic_block *bb_ptr;
	int ret;

	for(i=0;i<no_of_blocks;i++){
		bb_ptr = basic_blocks_arr[i];
		bb_ptr->g_cse_elimination();
	}

	ret=xform_input_for_gcse();

	/*
	if(ret){
		printf("xformed in gcse\n");
		print_tac(output_quad_arr_ptr);
	}
	*/


}

//! Perform the Copy Propagation Transformation on the Input
int
optimize_ic :: xform_input_for_copy_prop()
{
	int i,n;
	quad *quad_ptr,*new_quad_ptr;
	quad_set tmp_set;
	int xformed=0;


	if((copy_prop_quads.marked_quad_set.size() > 0 ) ||
		(copy_prop_quads.followed_quad_set.size() > 0 ) ){
		xformed=1;
	}

	/* transform input quads to output quads */
	n=input_quad_arr_ptr->size();
	for(i=0;i<n;i++){

		/* get the quad */
		quad_ptr=(*input_quad_arr_ptr)[i];

		/* marked quads */
		if(copy_prop_quads.is_quad_in_marked_quad_set(i) == 1 ){

		} else if(copy_prop_quads.is_quad_in_followed_quad_set(i) == 1 ){ /* followed quads */
			new_quad_ptr=copy_prop_quads.replace(quad_ptr,i);
			(*output_quad_arr_ptr).push_back(new_quad_ptr);
		}else{
			(*output_quad_arr_ptr).push_back(quad_ptr);
		}
	}

	return(xformed);

}
	

//! Perform the GCSE Transformation on the Input
int
optimize_ic :: xform_input_for_gcse()
{
	int i,n;
	quad *quad_ptr,*new_quad_ptr;
	sym_tab_entry *tmp_res;
	quad_set tmp_set;
	int xformed=0;

	make_intersection(gcse_quads.marked_quad_set,
	                   gcse_quads.followed_quad_set,tmp_set);

	/* Sanity check */
	assert(tmp_set.size() == 0 );

	if( (gcse_quads.marked_quad_set.size() > 0 ) || 
	    (gcse_quads.followed_quad_set.size() > 0 ) ){
		 xformed=1;
	}

	//copy(gcse_quads.marked_quad_set.begin(), gcse_quads.marked_quad_set.end(), ostream_iterator<int>(cout, "\n"));

	/* create temporaries */
	gcse_quads.create_temporaries();

	/* transform input quads to output quads */
	n=input_quad_arr_ptr->size();
	for(i=0;i<n;i++){

		/* get the quad */
		quad_ptr=(*input_quad_arr_ptr)[i];

		/* marked quads */
		if(gcse_quads.is_quad_in_marked_quad_set(i) == 1 ){
			tmp_res=gcse_quads.get_temporary(i);
			new_quad_ptr= new quad(quad_ptr->op,quad_ptr->arg1,quad_ptr->arg2,tmp_res);
			(*output_quad_arr_ptr).push_back(new_quad_ptr);
			new_quad_ptr= new quad(ASSIGN,tmp_res,NULL,quad_ptr->res);
			(*output_quad_arr_ptr).push_back(new_quad_ptr);

		} else if(gcse_quads.is_quad_in_followed_quad_set(i) == 1 ){ /* followed quads */
			tmp_res=gcse_quads.get_temporary(i);
			new_quad_ptr= new quad(ASSIGN,tmp_res,NULL,quad_ptr->res);
			(*output_quad_arr_ptr).push_back(new_quad_ptr);
		}else{
			(*output_quad_arr_ptr).push_back(quad_ptr);
		}
	}

	return(xformed);

}


//! For all the basic blocks reconstruct the TAC from DAGS
void 
optimize_ic::build_tac_from_dags()
{
	int i;
	basic_block *bb_ptr;

	for(i=0;i<no_of_blocks;i++){
		bb_ptr = basic_blocks_arr[i];
		bb_ptr->reconstruct_tac();
	}
}


//! Print TAC from a given quad vector pointer
void 
optimize_ic::print_tac(vector <quad *> *qa_ptr)
{
	int i,n;
	string print_str;

	n = (*qa_ptr).size();

   for(i=0;i<n;i++) {
		(*qa_ptr)[i] ->check_sanity();
      print_str = (*qa_ptr)[i] -> as_string();
      printf("(%d) %s\n",i,print_str.c_str() );
   }
	printf("\n");

}

//! Perform Data Flow Analysis
void 
optimize_ic::do_data_flow_analysis()
{
	/* clear all the Data structures */
	clear_all();

	/* split into basic blocks */
	split_into_basic_blocks();

	/* Reaching Definitions Analysis */
	create_GEN_and_KILL_sets();
	compute_reaching_definitions();
	make_ud_chains();

	/* Available Expressions Analysis */
	create_e_GEN_and_e_KILL_sets();
	compute_available_expressions();

	/* Liveness Analysis */
	create_live_DEF_and_live_USE_sets();
	perform_liveness_analysis();

	/* def-use chaining */
	create_du_DEF_and_du_USE_sets();
	make_du_chains();

	/* copy propagation information */
	create_c_GEN_and_c_KILL_sets();
	compute_copy_propagation_info();

	/* create all the loop info */
	create_loop_info();

	//print_blocks();
	//print_loop_info();

}

//! Print Loop Info
void
optimize_ic :: print_loop_info()
{
	int i,n;
	loop_info *loop_info_ptr;

	n=loop_info_arr.size();
	for(i=0;i<n;i++){
		loop_info_ptr=loop_info_arr[i];
		loop_info_ptr->print();
	}
}

static loop_t loop;
static stack<int> stk;


//! Create Loop Info
void
optimize_ic :: create_loop_info()
{
	edge_t b;
	int i,n;
	string s;
	loop_info *loop_info_ptr;
	quad_set exits;

	/* Find Dominators */
	find_dominators();

	/* Identify back edges */
	identify_back_edges();

	/* Identify natural loops */
	n=loop_info_arr.size();
	for(i=0;i<n; i++ ){
		loop_info_ptr=loop_info_arr[i];
		b = loop_info_ptr->back_edge;
		identify_loops(b);
		loop_info_ptr->set_natural_loop(loop);
	}

	/* Identify loop exits */
	n=loop_info_arr.size();
	for(i=0;i<n; i++ ){
		loop_info_ptr=loop_info_arr[i];
		exits=identify_exits(loop_info_ptr->natural_loop);
		loop_info_ptr->set_exits(exits);
	}
}

//! Identify all the exit nodes
quad_set
optimize_ic :: identify_exits(quad_set lp)
{
	quad_set :: iterator iter;
	vector<int> succ;
	quad_set exit_nodes;
	int blk_no;
	int tmp,n,i;
	basic_block *bb_ptr;
	quad_set :: iterator iter1;

	for(iter=lp.begin();iter != lp.end() ; iter++){
		blk_no=(*iter);
		bb_ptr=basic_blocks_arr[blk_no];
		succ=bb_ptr->successors;

		n=succ.size();
		for(i=0;i <n;i++){
			tmp=succ[i];
			/* succesor is NOT in the natural loop */
			if(lp.find(tmp) == lp.end() ){
				exit_nodes.insert(blk_no);
				break;
			}
		}
	}
	return(exit_nodes);
}


//! Identify all the Back Edges
void
optimize_ic :: identify_back_edges()
{
	int i,n;
	basic_block *bb_ptr;
	vector <edge_t> tmp_vect;
	edge_t b;
	vector <edge_t> back_edges;
	loop_info *loop_info_ptr;

	/* Search for edges whose heads dominate their tails */
	for(i=0;i<no_of_blocks;i++){
		bb_ptr = basic_blocks_arr[i];
		tmp_vect=bb_ptr->get_back_edges();
		if(tmp_vect.size() > 0 ){
			back_edges.insert(back_edges.begin(),tmp_vect.begin(),tmp_vect.end());
		}
	}

	n=back_edges.size();
	for(i=0;i<n;i++){
		loop_info_ptr=new loop_info();
		b=back_edges[i];
		loop_info_ptr->set_back_edge(b);
		loop_info_arr.push_back(loop_info_ptr);
	}

}

//! Identify Loops in the program
void
optimize_ic :: identify_loops(edge_t bk_edg)
{
	int head;
	int tail;
	int m,n,i,p;
	vector<int> preds;
	basic_block *bb_ptr;

	/* loop clear */
	loop.clear();

	/* clearing stk */
	while(!stk.empty() ){
		stk.pop();
	}

	head=bk_edg.first;
	tail = bk_edg.second;

	loop.insert(head);

	insert_in_loop(tail);

	while (!stk.empty() ){
		m=stk.top();
		stk.pop();
		bb_ptr=basic_blocks_arr[m];
		preds=bb_ptr->predecessors;
		n=preds.size();
		for(i=0;i<n;i++){
			p=preds[i];
			insert_in_loop(p);
		}
	}

}

//! Insert a Flow graph node into the Loop
void
optimize_ic :: insert_in_loop(int m)
{
	if(loop.find(m) == loop.end() ){
		loop.insert(m);
		stk.push(m);
	}
}

	

//! Compute Global Copy Propagation Information
void 
optimize_ic :: compute_copy_propagation_info()
{
	int i;
	unsigned int j;
	basic_block *bb_ptr;
	quad_set tmp_set;
	quad_set ::iterator iter;
	quad_set new_IN,tmp_IN,tmp_OUT;
	int change;
	int bb_no;
	basic_block *bb_ptr1;
	quad_set old_c_OUT;
	quad_set univ_set;

	/* B0 does not have a c_IN */
	basic_blocks_arr[0]->c_OUT = basic_blocks_arr[0]->c_GEN;

	univ_set= universal_copy_stmts;
	/* for B1 thru Bn */
	for(i=1;i<no_of_blocks;i++){
		bb_ptr = basic_blocks_arr[i];
		tmp_set.clear();
		make_diff(univ_set, bb_ptr->c_KILL, tmp_set);
		bb_ptr->c_OUT = tmp_set;
	}

	change=1;

	while(change){
		change=0;

		for(i=1;i<no_of_blocks;i++){
			bb_ptr = basic_blocks_arr[i];

			new_IN.clear();
			tmp_IN.clear();

			if(bb_ptr->predecessors.size() > 0 ){
				bb_no=bb_ptr->predecessors[0];
				bb_ptr1=basic_blocks_arr[bb_no];
				new_IN=bb_ptr1->c_OUT;
			}else{
				new_IN.clear();
			}

			/* in[B] = U out[P] */
			for(j=0;j<bb_ptr->predecessors.size();j++){
				bb_no=bb_ptr->predecessors[j];
				bb_ptr1=basic_blocks_arr[bb_no];
				make_intersection(new_IN, bb_ptr1->c_OUT,tmp_IN); 
				new_IN.clear();
				new_IN=tmp_IN;
				tmp_IN.clear();
			}

			bb_ptr->c_IN.clear();
			bb_ptr->c_IN = new_IN;

			old_c_OUT = bb_ptr->c_OUT ;

			tmp_OUT.clear();
			make_diff(bb_ptr->c_IN, bb_ptr->c_KILL, tmp_OUT);

			bb_ptr->c_OUT.clear();
			make_union(tmp_OUT, bb_ptr->c_GEN,bb_ptr->c_OUT);

			if(old_c_OUT != bb_ptr->c_OUT ){
				change=1;
			}

		}

	}

}

//! Make DU Chains
void 
optimize_ic::make_du_chains()
{
	int i;
	unsigned int j;
	basic_block *bb_ptr;
	quad_set tmp_set;
	du_set ::iterator iter;
	du_set new_OUT,tmp_OUT;
	du_set tmp_IN,old_du_IN;
	int change;
	int bb_no;
	basic_block *bb_ptr1;

	change=1;

	while(change){
		change=0;

		for(i=0;i<no_of_blocks;i++){
			bb_ptr = basic_blocks_arr[i];

			new_OUT.clear();
			tmp_OUT.clear();

			for(j=0;j<bb_ptr->successors.size();j++){
				bb_no=bb_ptr->successors[j];
				bb_ptr1=basic_blocks_arr[bb_no];
				make_union(new_OUT, bb_ptr1->du_IN,tmp_OUT); 
				new_OUT.clear();
				new_OUT=tmp_OUT;
				tmp_OUT.clear();
			}


			bb_ptr->du_OUT.clear();
			bb_ptr->du_OUT = new_OUT;

			tmp_IN.clear();
			make_diff(bb_ptr->du_OUT, bb_ptr->du_DEF, tmp_IN);

			old_du_IN = bb_ptr->du_IN;

			bb_ptr->du_IN.clear();
			make_union(tmp_IN, bb_ptr->du_USE,bb_ptr->du_IN);

			if(old_du_IN != bb_ptr->du_IN ){
					change=1;
			}

		}

	}

	/* at this point du_OUT is populated */
	for(i=0;i<no_of_blocks;i++){
		bb_ptr = basic_blocks_arr[i];
		bb_ptr->build_du_chains();
	}

}

//! All the variables used in the Block
void
basic_block::find_all_vars_used_in_block()
{
	int i;
	quad *quad_ptr;
	vset *vs;
	sym_tab_entry *arg1,*arg2,*res;

	vs = &all_vars_used;

	/* Find all the variables used */
	for(i=start;i<end;i++){
		quad_ptr=(*input_quad_arr_ptr)[i];
		arg1=quad_ptr->arg1;
		arg2=quad_ptr->arg2;
		res=quad_ptr->res;
		add_to_vset(vs,arg1);
		add_to_vset(vs,arg2);
		add_to_vset(vs,res);
	}

	//print_v_set(*vs);

	return;
	
}

//! Perform Liveness Analysis
void 
optimize_ic::perform_liveness_analysis()
{
	int i;
	unsigned int j;
	basic_block *bb_ptr;
	quad_set tmp_set;
	vset ::iterator iter;
	vset new_OUT,tmp_OUT;
	vset tmp_IN,old_live_IN;
	int change;
	int bb_no;
	basic_block *bb_ptr1;

	change=1;

	while(change){
		change=0;

		for(i=0;i<no_of_blocks;i++){
			bb_ptr = basic_blocks_arr[i];

			new_OUT.clear();
			tmp_OUT.clear();

			for(j=0;j<bb_ptr->successors.size();j++){
				bb_no=bb_ptr->successors[j];
				bb_ptr1=basic_blocks_arr[bb_no];
				make_union(new_OUT, bb_ptr1->live_IN,tmp_OUT); 
				new_OUT.clear();
				new_OUT=tmp_OUT;
				tmp_OUT.clear();
			}


			bb_ptr->live_OUT.clear();
			bb_ptr->live_OUT = new_OUT;

			tmp_IN.clear();
			make_diff(bb_ptr->live_OUT, bb_ptr->live_DEF, tmp_IN);

			old_live_IN = bb_ptr->live_IN;

			bb_ptr->live_IN.clear();
			make_union(tmp_IN, bb_ptr->live_USE,bb_ptr->live_IN);

			if(old_live_IN != bb_ptr->live_IN ){
					change=1;
			}

		}

	}

	/* Extrapolate live_OUT information */
	for(i=0;i<no_of_blocks;i++){
		bb_ptr = basic_blocks_arr[i];
		bb_ptr->xtrapolate_liveness();
	}
		

}

//! Create e_GEN and e_KILL sets
void 
optimize_ic::create_e_GEN_and_e_KILL_sets()
{
	int i;
	basic_block *bb_ptr;
	quad_set tmp_set;
	quad_set all_expr_set;
	quad_set ::iterator iter;
	int quad_no;
	int x;


	/* Create Universal set of expressions */
	for(i=0;i<no_of_blocks;i++){
		bb_ptr = basic_blocks_arr[i];
		tmp_set.clear();
		tmp_set=bb_ptr->get_all_expr();
		make_union(all_expr_set,tmp_set,all_expr_set);
	}

	for(iter=all_expr_set.begin();iter != all_expr_set.end();iter++){
		quad_no=(*iter);
		x=universal_expr.insert_avail_expr(quad_no);
	}

	//universal_expr.print();


	/* e_GEN set */
	for(i=0;i<no_of_blocks;i++){
		bb_ptr = basic_blocks_arr[i];
		bb_ptr->create_e_GEN_set();
	}


	/* e_KILL set */
	for(i=0;i<no_of_blocks;i++){
		bb_ptr = basic_blocks_arr[i];
		bb_ptr->create_e_KILL_set();
	}

	return;
}

//! Create c_GEN and c_KILL sets
void 
optimize_ic::create_c_GEN_and_c_KILL_sets()
{
	int i;
	basic_block *bb_ptr;
	quad_set tmp_set;
	quad_set all_copy_set;
	quad_set ::iterator iter;


	/* Create Universal set of copy statements */
	for(i=0;i<no_of_blocks;i++){
		bb_ptr = basic_blocks_arr[i];
		bb_ptr->add_to_univ_copy_stmts(universal_copy_stmts);
	}

	/* c_GEN set */
	for(i=0;i<no_of_blocks;i++){
		bb_ptr = basic_blocks_arr[i];
		bb_ptr->create_c_GEN_set();
	}


	/* c_KILL set */
	for(i=0;i<no_of_blocks;i++){
		bb_ptr = basic_blocks_arr[i];
		bb_ptr->create_c_KILL_set(universal_copy_stmts);
	}

	return;
}


//! Create rd_GEN and rd_KILL sets
void 
optimize_ic::create_GEN_and_KILL_sets()
{
	int i;
	basic_block *bb_ptr;
	quad_set tmp_set;
	quad_set ::iterator iter;
	int quad_no;

	for(i=0;i<no_of_blocks;i++){
		bb_ptr = basic_blocks_arr[i];
		bb_ptr->create_GEN_set();
	}

	for(i=0;i<no_of_blocks;i++){
		bb_ptr = basic_blocks_arr[i];
		for(iter=bb_ptr->rd_GEN.begin();iter != bb_ptr->rd_GEN.end();iter++){
			quad_no= (*iter);
			tmp_set.insert(quad_no);
		}
	}

	for(i=0;i<no_of_blocks;i++){
		bb_ptr = basic_blocks_arr[i];
		bb_ptr->create_KILL_set(tmp_set);
	}

}


//! ??
void 
optimize_ic::create_live_DEF_and_live_USE_sets()
{
	int i;
	basic_block *bb_ptr;

	for(i=0;i<no_of_blocks;i++){
		bb_ptr = basic_blocks_arr[i];
		bb_ptr->create_DEF_USE_sets();
	}

}


//! create du_DEF and du_USE sets
void 
optimize_ic::create_du_DEF_and_du_USE_sets()
{
	basic_block *bb_ptr;
	int i,n;
	quad *quad_ptr;
	sym_tab_entry *arg1,*arg2;

	du_set tmp_set;

	/* Compute all the uses across all the blocks */
	n = (*input_quad_arr_ptr).size();

	for(i=0;i<n;i++){
		quad_ptr = (*input_quad_arr_ptr)[i];
		arg1=quad_ptr->arg1;
		arg2=quad_ptr->arg2;

		if(quad_ptr-> is_op_valid_for_du_analysis() == 0 ){
			continue;
		}

		if( (arg1 != NULL ) &&
		   (arg1->is_it_a_constant() != 1) &&
			(arg1->is_tmp() != 1 ) ){
			tmp_set.insert(make_pair(i,arg1));
		}

		if(arg2 != arg1 ){
			if( (arg2 != NULL ) && 
				(arg2->is_it_a_constant() != 1) &&
				(arg2->is_tmp() != 1 ) ){
				tmp_set.insert(make_pair(i,arg2));
			}
		}
	}

	/* tmp_set contains all the uses of ALL variables */
	for(i=0;i<no_of_blocks;i++){
		bb_ptr = basic_blocks_arr[i];
		bb_ptr->create_du_DEF_USE_sets(tmp_set);
	}

}

//! Compute rd_IN/rd_OUT
void
optimize_ic:: compute_reaching_definitions()
{
	int i;
	unsigned int j;
	basic_block *bb_ptr;
	quad_set tmp_set;
	quad_set ::iterator iter;
	quad_set new_IN,tmp_IN,tmp_OUT;
	int change;
	int bb_no;
	basic_block *bb_ptr1;

	for(i=0;i<no_of_blocks;i++){
		bb_ptr = basic_blocks_arr[i];
		bb_ptr->rd_OUT = bb_ptr->rd_GEN;
	}

	change=1;

	while(change){
		change=0;

		for(i=0;i<no_of_blocks;i++){
			bb_ptr = basic_blocks_arr[i];

			new_IN.clear();
			tmp_IN.clear();
			for(j=0;j<bb_ptr->predecessors.size();j++){
				bb_no=bb_ptr->predecessors[j];
				bb_ptr1=basic_blocks_arr[bb_no];
				make_union(new_IN, bb_ptr1->rd_OUT,tmp_IN); 
				new_IN.clear();
				new_IN=tmp_IN;
				tmp_IN.clear();
			}

			if(new_IN != bb_ptr->rd_IN ){
					change=1;
			}

			bb_ptr->rd_IN.clear();
			bb_ptr->rd_IN = new_IN;

			tmp_OUT.clear();
			make_diff(bb_ptr->rd_IN, bb_ptr->rd_KILL, tmp_OUT);

			bb_ptr->rd_OUT.clear();
			make_union(tmp_OUT, bb_ptr->rd_GEN,bb_ptr->rd_OUT);
		}

	}

}
/*
void
optimize_ic:: make_union(quad_set &A, quad_set &B, quad_set &C)
{
	set_union(A.begin(),A.end(),B.begin(),B.end(),inserter(C,C.begin() ) );
}
void
optimize_ic:: make_diff(quad_set &A, quad_set &B, quad_set &C)
{
	set_difference(A.begin(),A.end(),B.begin(),B.end(),inserter(C,C.begin() ) );
}

void
optimize_ic:: make_intersection(quad_set &A, quad_set &B, quad_set &C)
{
	set_intersection(A.begin(),A.end(),B.begin(),B.end(),inserter(C,C.begin() ) );
}
*/

//! Make UD Chain
void
optimize_ic:: make_ud_chains()
{
	int i,no_of_quads;
	basic_block *bb_ptr;
	quad_info *quad_info_ptr;


	no_of_quads = (*input_quad_arr_ptr).size();

	for(i=0;i<no_of_quads;i++){
		quad_info_ptr = new quad_info();
		quad_info_arr.push_back(quad_info_ptr);
	}

	for(i=0;i<no_of_blocks;i++){
		bb_ptr = basic_blocks_arr[i];
		bb_ptr->build_ud_chains();
//		bb_ptr->print_ud_chains();
	}
}


//! Compute e_IN/e_OUT
void
optimize_ic:: compute_available_expressions()
{
	int i;
	unsigned int j;
	basic_block *bb_ptr;
	quad_set tmp_set;
	quad_set ::iterator iter;
	quad_set new_IN,tmp_IN,tmp_OUT;
	int change;
	int bb_no;
	basic_block *bb_ptr1;
	expr_set old_e_OUT;
	expr_set univ_set;

	/* B0 does not have a e_IN */
	basic_blocks_arr[0]->e_OUT = basic_blocks_arr[0]->e_GEN;

	/* Initialize e_OUT for all blocks */
	univ_set=universal_expr.get_expr_set();
	/* for B1 thru Bn */
	for(i=1;i<no_of_blocks;i++){
		bb_ptr = basic_blocks_arr[i];
		tmp_set.clear();
		make_diff(univ_set, bb_ptr->e_KILL, tmp_set);
		bb_ptr->e_OUT = tmp_set;
	}

	change=1;

	while(change){
		change=0;

		for(i=1;i<no_of_blocks;i++){
			bb_ptr = basic_blocks_arr[i];

			new_IN.clear();
			tmp_IN.clear();

			/* e_IN[B]= intersect e_OUT[P] for all the 
			  predecessors P */
			if(bb_ptr->predecessors.size() > 0 ){
				bb_no=bb_ptr->predecessors[0];
				bb_ptr1=basic_blocks_arr[bb_no];
				new_IN=bb_ptr1->e_OUT;
			}
			
			for(j=0;j<bb_ptr->predecessors.size();j++){
				bb_no=bb_ptr->predecessors[j];
				bb_ptr1=basic_blocks_arr[bb_no];
				make_intersection(new_IN, bb_ptr1->e_OUT,tmp_IN); 
				new_IN.clear();
				new_IN=tmp_IN;
				tmp_IN.clear();
			}

			bb_ptr->e_IN.clear();
			bb_ptr->e_IN = new_IN;

			/* saving e_OUT to later check if we have 
			   reached steady state */
			old_e_OUT = bb_ptr->e_OUT ;

			/* computing e_OUT */
			tmp_OUT.clear();
			make_diff(bb_ptr->e_IN, bb_ptr->e_KILL, tmp_OUT);
			bb_ptr->e_OUT.clear();
			make_union(tmp_OUT, bb_ptr->e_GEN,bb_ptr->e_OUT);

			/* Checking for a steady state of e_OUT */
			if(old_e_OUT != bb_ptr->e_OUT ){
				change=1;
			}

		}

	}

}

//! Compute the Dominator Information
void
optimize_ic :: find_dominators()
{
	int 	i;
	quad_set *qs_ptr;
	quad_set all;
	quad_set tmp_set,tmp_set1,Dj,new_set;
	int change,j,n;
	int bno;
	basic_block *bb_ptr;
	vector <quad_set *>D; /* Dominators */

	for(i=0;i<no_of_blocks;i++){
		qs_ptr=new quad_set;
		D.push_back(qs_ptr);
	}

	/* Initial node is 0 */
	qs_ptr=D[0];
	qs_ptr->insert(0);

	for(i=0;i<no_of_blocks;i++){
		all.insert(i);
	}


	/* Having D[1]={0,1,2,...n},D[2]={0,1,...n} */
	for(i=1;i<no_of_blocks;i++){
		qs_ptr=D[i];
		*qs_ptr=all;
	}

	change=1;
	while(change ){
		change=0;

		for(i=1;i<no_of_blocks;i++){
			qs_ptr=D[i];

			tmp_set.clear();
			tmp_set.insert(i);

			bb_ptr=basic_blocks_arr[i];
			n=bb_ptr->predecessors.size();
			if(n){
				bno=bb_ptr->predecessors[0];
				tmp_set1= *D[bno];
			}
			for(j=0;j<n;j++){

				bno=bb_ptr->predecessors[j];
				Dj.clear();
				Dj=*D[bno];
				new_set.clear();

				make_intersection(tmp_set1,Dj,new_set);
				tmp_set1.clear();
				tmp_set1=new_set;
			}
			make_union(tmp_set,tmp_set1,new_set);

			tmp_set=*qs_ptr;
			*qs_ptr=new_set;

			if(tmp_set != *qs_ptr ){
				change=1;
			}
		}
	}

	for(i=0;i<no_of_blocks;i++){
		bb_ptr=basic_blocks_arr[i];
		bb_ptr->set_dominators(D[i]);
		delete (D[i]);
	}
		

	#if 0
	/* print dominators */
	for(i=0;i<no_of_blocks;i++){
		printf("D[%d] = ",i);
		tmp_set=*D[i];
		copy(tmp_set.begin(), tmp_set.end(), ostream_iterator<int>(cout, " "));
	}
	printf("\n");
	#endif

}

//! Compute the Dominator Information
void
gcse :: clear ()
{
	gcse_info.clear();
	marked_quad_set.clear();
	followed_quad_set.clear();
	stab=NULL;
}


//! Insert the Marked quads at the given quad number
void
gcse :: insert_marked_quads (int marked_quad_no, int quad_no)
{
	int i,n;
	int found=0;
	gcse_node *ptr;

	n=gcse_info.size();

	for(i=0;i<n;i++){
		ptr = gcse_info[i];
		if(ptr->marked_quad == marked_quad_no ){
			ptr->followed_quads.insert(quad_no);
			followed_quad_set.insert(quad_no);
			found=1;
		}
	}

	if(found==0){
		ptr=new gcse_node();
		ptr->marked_quad = marked_quad_no ;
		marked_quad_set.insert(marked_quad_no);
		ptr->followed_quads.insert(quad_no);
		followed_quad_set.insert(quad_no);
		gcse_info.push_back(ptr);
	}
}

//! Set the Symbol Table
void
gcse :: set_sym_tab(sym_tab *st)
{
	stab=st;
}

//! Create Temporaries for all the GCSE
void
gcse :: create_temporaries()
{
	int i,n,x;
	type_expr *tmp_typ;
	sym_tab_entry *s,*res;
	gcse_node *ptr,*tmp_ptr;
	int marked_quad;

	s = (*input_quad_arr_ptr)[0]->arg1; /* PROC_BEGIN */
	stab=s->sym_tab_ptr;

	n=gcse_info.size();

	for(i=0;i<n;i++){
		ptr = gcse_info[i];

		x=is_temp_known(i);
		if( x == -1 ){
			marked_quad=ptr->marked_quad;
			res=(*input_quad_arr_ptr)[marked_quad]->res;
			tmp_typ = res->type;
			s=stab->make_temporary(tmp_typ);
		}else{
			tmp_ptr= gcse_info[x];
			s = tmp_ptr->temporary;
		}

		ptr->temporary=s;
	}
}

//! ???
int
gcse :: is_temp_known(int gcse_index)
{
	int i;
	gcse_node *ptr1, *ptr2;
	set<int,less<int> > tmp_set;

	for(i=0;i<gcse_index;i++){
		ptr1=gcse_info[i];
		ptr2=gcse_info[gcse_index];
		make_intersection(ptr1->followed_quads,
		                      ptr2->followed_quads,tmp_set);
		if(tmp_set.size() > 0 ){
			return(i);
		}
	}
	return(-1);
}

//! Is the given quad a marked one
int
gcse :: is_quad_in_marked_quad_set(int quad_no)
{
	if(marked_quad_set.find(quad_no) != marked_quad_set.end() ){
		return(1);
	}else{
		return(0);
	}
}

//! Is the given quad in the followed quad set
int
gcse :: is_quad_in_followed_quad_set(int quad_no)
{
	if(followed_quad_set.find(quad_no) != followed_quad_set.end() ){
		return(1);
	}else{
		return(0);
	}
}

//! Get the temporary associated with the quad
sym_tab_entry *
gcse :: get_temporary(int quad_no )
{
	int n,i;
	gcse_node *ptr;
	if(marked_quad_set.find(quad_no) != marked_quad_set.end() ){
		
		n = gcse_info.size();

		for(i=0;i<n;i++){
			ptr = gcse_info[i];
			if(ptr->marked_quad == quad_no ){
				return(ptr->temporary);
			}
		}
	}
	if(followed_quad_set.find(quad_no) != followed_quad_set.end() ){
		
		n = gcse_info.size();

		for(i=0;i<n;i++){
			ptr = gcse_info[i];
			if(ptr->followed_quads.find(quad_no) != ptr->followed_quads.end() ){
				return(ptr->temporary);
			}
		}
	}
	return(NULL);
}

//! Add the quad number to the followed quads 
void
copy_prop_node :: add_to_followed_quads(int quad_no)
{
	followed_quads.insert(quad_no);
}

//! Clear all the Copy Propagation Data Structures
void
copy_prop :: clear()
{
	copy_prop_info.clear();
	marked_quad_set.clear();
	followed_quad_set.clear();
}

//! Add the quad to the copy propagation followed quads
void
copy_prop :: insert(int marked_quad_no,int quad_no,sym_tab_entry *arg1,sym_tab_entry *res)
{
	int i,n;
	int found=0;
	copy_prop_node *cp_node_ptr;

	n=copy_prop_info.size();

	found=0;
	for(i=0;i<n;i++){
		cp_node_ptr = copy_prop_info[i];
		if(cp_node_ptr -> marked_quad == marked_quad_no ){
			cp_node_ptr->add_to_followed_quads(quad_no);
			followed_quad_set.insert(quad_no);
			found=1;
			break;
		}
	}

	if(found == 0 ){
		cp_node_ptr = new copy_prop_node ;
		cp_node_ptr->marked_quad = marked_quad_no;
		cp_node_ptr->add_to_followed_quads(quad_no);
		cp_node_ptr->replacement=arg1;
		cp_node_ptr->src=res;
		copy_prop_info.push_back(cp_node_ptr);
		marked_quad_set.insert(marked_quad_no);
		followed_quad_set.insert(quad_no);
	}

}

//! Is the quad in marked quad for copy propagation
int 
copy_prop::is_quad_in_marked_quad_set(int quad_no)
{
	if(marked_quad_set.find(quad_no) != marked_quad_set.end() ){
		return(1);
	}else{
		return(0);
	}
}

//! Is the quad in followed quads for copy propagation
int 
copy_prop::is_quad_in_followed_quad_set(int quad_no)
{
	if(followed_quad_set.find(quad_no) != followed_quad_set.end() ){
		return(1);
	}else{
		return(0);
	}
}

//! Replace the quad in the followed quads
quad * 
copy_prop::replace(quad *qp,int followed_quad)
{
	int i,n;
	copy_prop_node *cp_node_ptr;
	quad *quad_ptr;
	sym_tab_entry *new_arg1,*new_arg2;

	n=copy_prop_info.size();

	for(i=0;i<n;i++){
		cp_node_ptr = copy_prop_info[i];
		if(cp_node_ptr -> followed_quads.find(followed_quad) != cp_node_ptr->followed_quads.end() ){
			if(qp->arg1 == cp_node_ptr->src){
				new_arg1=cp_node_ptr->replacement;
			}else{
				new_arg1 = qp->arg1;
			}

			if(qp->arg2 == cp_node_ptr->src){
				new_arg2=cp_node_ptr->replacement;
			}else{
				new_arg2 = qp->arg2;
			}
			quad_ptr = new quad(qp->op,new_arg1,new_arg2,qp->res);
			return(quad_ptr);
		}
	}

	assert(0);

	return(NULL);

}

/* loop_info */

//! Set the Natural loop 
void
loop_info :: set_natural_loop(loop_t lp )
{
	natural_loop = lp;
}

//! Set the Back Edge
void
loop_info :: set_back_edge(edge_t b)
{
	back_edge=b;
	hdr_blk=b.first;
}

//! Set the exiting quads
void
loop_info :: set_exits(quad_set e)
{
	exits=e;
}

//! Add the dependend quads
void
loop_info :: add_dependent_quads(int quad_no,int dep_quad_no)
{
	pair<int,int>  p;

	p = make_pair(quad_no,dep_quad_no);
	dependent_pairs.push_back(p);
}

/* returns 0, if atleast one of the dependent quads is NOT
   in the pre_header set
*/
//! Checks if all the dependent quads are in pre-header
int
loop_info :: check_for_dependent_quads(int quad_no,quad_set preheader)
{
	int i,n;
	int d_quad_no,d_dependent_quad_no;

	n= dependent_pairs.size();

	for(i=0;i<n;i++){
		d_quad_no=dependent_pairs[i].first;
		d_dependent_quad_no = dependent_pairs[i].second;

		if(d_quad_no == quad_no ){
			if(preheader.find(d_dependent_quad_no) == preheader.end() ){
				return(0);
			}
		}
	}
	return(1);
}

#if 0
void
loop_info :: create_quad_arr()
{
	sym_tab_entry *sptr;

	preheader_start=sptr= newlbl();

	preheader_quad_arr.push_back(sptr);

	for(iter = preheader.begin();iter != preheader.end();iter++){
		quad_no=(*iter);
		sptr=(*input_quad_arr_ptr)[quad_no];
		quad_arr.push_back(sptr);
	}
}
#endif
				

//! Print the Loop Information
void
loop_info :: print()
{
	string s;
	loop_t :: iterator iter;
	quad_set :: iterator iter1;
	int i,n;

	printf("\n========================\n");
	printf("back_edge %d-->%d loop={ ",back_edge.second,back_edge.first);

	for(iter=natural_loop.begin();iter !=natural_loop.end();iter++){
		printf("%d ",(*iter));
	}
	printf("}\n");

	printf("invariants ={ ");
	for(iter1=loop_invariants.begin();iter1 != loop_invariants.end();iter1++){
		printf("%d ",(*iter1) );
	}
	printf("}\n");
	printf("exits ={ ");
	for(iter1=exits.begin();iter1 != exits.end();iter1++){
		printf("%d ",(*iter1) );
	}
	printf("}\n");
	printf("can_be_moved_to_preheader ={ ");
	for(iter1=can_be_moved_to_preheader.begin();iter1 != can_be_moved_to_preheader.end();iter1++){
		printf("%d ",(*iter1) );
	}
	printf("}\n");

	printf("dependent pairs ={ ");

	n=dependent_pairs.size();
	for(i=0;i<n;i++){
		printf("(%d depends on %d) ",dependent_pairs[i].first,dependent_pairs[i].second );
	}
	printf("}\n");
	printf("\n========================\n");

}


/* optimization string */



//! Parse the Optimization String and return appropriately
int get_optim(char *optimi_str)
{
	int optimize=0x0;
	string str(optimi_str);
	string tok;
	int i,n;
	
		vector <string> tokens;

    	Tokenize(str, tokens, ",");

		n=tokens.size();

		i=0;

		while(i < n ){
			tok=tokens[i];

			if(tok == "local" ){
				optimize |= LOCAL;
			}else if (tok == "gcse" ){
				optimize |= GCSE;
			}else if (tok == "gcp" ){
				optimize |= GCP;
			}else if (tok == "gcm" ){
				optimize |= GCM;
			}else if (tok == "gpo" ){
				optimize |= GPO;
			}else if (tok == "gdce" ){
				optimize |= GDCE;
			}else if (tok == "all" ){
				optimize = LOCAL|GCSE|GCP|GCM|GPO|GDCE;
			} else {
				return(-1);
			}
			i++;
		}
	return(optimize);
}
				
				


//! Tokenize the String
void Tokenize(const string& str,
                      vector<string>& tokens,
                      const string& delimiters )
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);

    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }

}


/* Added for Generating optimized Target Code */

#include "target_code_gen.h"

/*
typedef set<x86_as_operand *> location_set;
*/

//! Live Next Use Information
class live_next_use
{
	public:
	//! Indicates Liveness
	int liveness; /* 1 for live, 0 for dead */
	//! Next uses
	quad_set next_uses;
};

//! Used for computing Next use
class next_use 
{
	public:
	//! Live and next use properties of arg1 of the quad
	live_next_use arg1;
	//! Live and next use properties of arg2 of the quad
	live_next_use arg2;
	//! Live and next use properties of res of the quad
	live_next_use res;
	next_use()
	{
		arg1.liveness=0;
		arg1.next_uses.clear();
		arg2.liveness=0;
		arg2.next_uses.clear();
		res.liveness=0;
		res.next_uses.clear();
	};
};

//! Mapping the Variable to the Liveness/Next Use Information
typedef map <sym_tab_entry *,live_next_use *,less<sym_tab_entry* > > live_next_use_t;

//! Macro to test if argument is a Id
#define arg_valid(arg) ((arg != NULL ) && (arg->is_it_a_constant() != 1 ) )


/* using vset for var set */

//! Set of registers
typedef set<reg_entry *> reg_set;

//! Variable Descriptor Entry
class var_desc_entry
{
	public:
	//! In memory or only in regs 
	int memory_status;
	//! In Register
	reg_set in_reg;
	//! Constructor
	var_desc_entry(int m)
	{
		memory_status=m;
	}
	~var_desc_entry()
	{
	}
};


//! Variable descriptor Table indexed by Variable
typedef map <sym_tab_entry *,var_desc_entry *,less<sym_tab_entry* > > var_desc_map;
//! Register descriptor Table indexed by Register
typedef map <reg_entry *,vset *,less<reg_entry* > >reg_desc_map;

//! Variable Descriptor Table and the associated methods
class var_desc_spec
{
	public:
	//! Variable Descriptor Table
	var_desc_map var_desc_tab;

	void clear_entry(sym_tab_entry *s,reg_entry *r);
	int check_entry(sym_tab_entry *s,reg_entry *r);
	reg_entry *get_single_entry(sym_tab_entry *s);
	reg_set *get_full_entry(sym_tab_entry *s);
	void add_entry(sym_tab_entry *s,reg_entry *r);
	var_desc_spec(vset *vs);
	void print_var_desc();

	private:
	reg_entry *get_first_reg(reg_set *rs);


};

//! Register Descriptor Table and the associated methods
class reg_desc_spec
{
	public:
	//! Register Descriptor Table
	reg_desc_map reg_desc_tab;
	int check_entry(reg_entry *r,sym_tab_entry *s);
	void clear_entry(reg_entry *r,sym_tab_entry *s);
	reg_entry *fetch_any_free_reg();
	reg_entry *fetch_any_reg();
	vset *get_full_entry(reg_entry *r);
	void add_entry(reg_entry *r,sym_tab_entry *s);
	void print_reg_desc();
	reg_desc_spec()
	{
	};
	~reg_desc_spec()
	{
	};
};

//! Print the Register Set
void
print_r_set(reg_set *rs)
{
	reg_set :: iterator iter;
	reg_entry *r;

	for(iter=rs->begin();iter != rs->end();iter++){
		r=(*iter);	
		printf(" %s ",r->get_name(4).c_str());
	}
}


//! Fetch any Register in the Register Descriptor
reg_entry *
reg_desc_spec :: fetch_any_reg()
{
	reg_entry *r;
	reg_desc_map :: iterator iter;

	r = NULL;

	for(iter=reg_desc_tab.begin();
	        iter != reg_desc_tab.end() ; iter ++ ){

		r=(*iter).first;
		break;
	}
	return(r);
}

//! Clear a given register Descriptor entry for an Id
void
reg_desc_spec :: clear_entry(reg_entry *r,sym_tab_entry *s)
{
	reg_desc_map :: iterator iter;
	vset *vs;
	vset :: iterator iter1;

	iter=reg_desc_tab.find(r);
	if( iter != reg_desc_tab.end() ){
		vs=(*iter).second;
		iter1=vs->find(s);
		if(iter1 != vs->end() ){
			vs->erase(iter1);
			return;
		}
	}
//	assert(0);

	return;
}

//! Fetch a Frre register in the register Descriptor Table
reg_entry *
reg_desc_spec ::fetch_any_free_reg()
{
	reg_entry *r;
	reg_desc_map :: iterator iter;
	vset *vs;

	r = NULL;

	for(iter=reg_desc_tab.begin();
	        iter != reg_desc_tab.end() ; iter ++ ){

		r=(*iter).first;
		vs=(*iter).second;

		if(vs->size() == 0 ){
			return(r);
		}
	}
	return(NULL);
	
}

//! Print the Register Descriptor Table
void
reg_desc_spec ::print_reg_desc()
{
	reg_entry *r;
	reg_desc_map :: iterator iter;
	vset *vs;

	for(iter=reg_desc_tab.begin();
	        iter != reg_desc_tab.end() ; iter ++ ){

		r=(*iter).first;
		vs=(*iter).second;
		printf(" %s ",r->get_name(4).c_str());
		print_v_set(*vs);
		printf("\n");
	}
}

//! Get a full entry  in register descriptor for a Register 
vset *
reg_desc_spec ::get_full_entry(reg_entry *r)
{
	reg_desc_map :: iterator iter;
	vset *vs;

	iter=reg_desc_tab.find(r);
	assert(iter != reg_desc_tab.end() );
	vs=(*iter).second;

	return(vs);
}


//! Check if the register r holds the variable s
int
reg_desc_spec ::check_entry(reg_entry *r,sym_tab_entry *s)
{
	reg_desc_map :: iterator iter;
	vset *vs;

	iter=reg_desc_tab.find(r);
	assert(iter != reg_desc_tab.end() );
	vs=(*iter).second;
	if( vs->find(s) != vs->end() ){
		return(1);
	}
	return(0);
}

//! Add the entry (r,s) in register descriptor Table
void
reg_desc_spec ::add_entry(reg_entry *r,sym_tab_entry *s)
{
	reg_desc_map :: iterator iter;
	vset *vs;

	iter=reg_desc_tab.find(r);
	vs=(*iter).second;
	vs->insert(s);

	return;

}

//! Constructor Variable Descriptor Specification 
var_desc_spec::var_desc_spec(vset *vs)
{
	;
}

//! Clear an entry (s,r) in Variable Descriptor Table
void 
var_desc_spec :: clear_entry(sym_tab_entry *s,reg_entry *r)
{
	var_desc_map :: iterator iter;
	reg_set *rs;
	var_desc_entry *ve;
	reg_set :: iterator iter1;


	iter=var_desc_tab.find(s);
	if( iter != var_desc_tab.end() ){
		ve=(*iter).second;
		rs=&ve->in_reg;

		iter1=rs->find(r);
		if(iter1 != rs->end() ){
			rs->erase(iter1);
			return;
		}
	}
//	assert(0);

}
//! Get an entry in the Variable Descriptor Table for a Id
reg_entry *
var_desc_spec :: get_single_entry(sym_tab_entry *s)
{
	var_desc_map :: iterator iter;
	reg_set *rs;
	var_desc_entry *ve;
	reg_set :: iterator iter1;
	reg_entry *r;

	r=NULL;

	iter=var_desc_tab.find(s);
	if( iter != var_desc_tab.end() ){
		ve=(*iter).second;
		rs=&ve->in_reg;

		r=get_first_reg(rs);
		return(r);

	}
	/*
	assert(0);
	*/

	return(r);

}

//! Get the first reg in register set
reg_entry *
var_desc_spec ::get_first_reg(reg_set *rs)
{
	reg_set :: iterator iter;
	reg_entry *r;

	r=NULL;
	for(iter=rs->begin();iter != rs->end();iter++){
		r=(*iter);
		break;
	}
	return(r);
}


//! Get the Full entry in the Variable Descriptor for a given Id
reg_set *
var_desc_spec ::get_full_entry(sym_tab_entry *s)
{
	var_desc_map :: iterator iter;
	var_desc_entry *ve;

	iter=var_desc_tab.find(s);
	if( iter != var_desc_tab.end() ){
		ve=(*iter).second;
		return(&ve->in_reg);
	}
	assert(0);
	return(NULL);
}

//! Check if an entry (s,r) exists in Variable Descriptor
int 
var_desc_spec :: check_entry(sym_tab_entry *s,reg_entry *r)
{
	var_desc_map :: iterator iter;
	reg_set *rs;
	var_desc_entry *ve;
	reg_set :: iterator iter1;

	iter=var_desc_tab.find(s);
	if( iter != var_desc_tab.end() ){
		ve=(*iter).second;
		rs=&ve->in_reg;

		iter1=rs->find(r);
		if(iter1 != rs->end() ){
			return(1);
		}
	}
	return(0);
}


//! Print the Variable Descriptor
void
var_desc_spec :: print_var_desc()
{
	var_desc_map :: iterator iter;
	reg_set *rs;
	var_desc_entry *ve;
	sym_tab_entry *s;

	printf("Variable Descriptor \n");
	for(iter=var_desc_tab.begin(); iter != var_desc_tab.end() ; iter ++){
		s=(*iter).first;
		ve=(*iter).second;
		rs=&ve->in_reg;
		if(rs->size() > 0 ){
			printf(" %s ",s->get_name().c_str() );
			print_r_set(rs);
			printf("\n");
		}
	}
}

//! Add the entry (s,r) in the variable descriptor Table
void
var_desc_spec ::add_entry(sym_tab_entry *s,reg_entry *r)
{
	var_desc_map :: iterator iter;
	var_desc_entry *ve;

	iter=var_desc_tab.find(s);
	if( iter != var_desc_tab.end() ){
		ve=(*iter).second;
		ve->in_reg.insert(r);
		return;
	}
	assert(0);
}

	
//! Target Code Optimization
class optimize_tc : public optimize_ic
{
	public:
	optimize_tc()
	{
	};
	int gen_tc(vector <quad *> *input_quad_ptr);
	x86_as_operand *make_x86_operand(sym_tab_entry *arg2);
	x86_as_operand *fetch_x86_operand(sym_tab_entry *arg2);
	x86_as_operand *fetch_x86_operand(reg_entry *r);
	void print_tc();
	int requires_dst(int op);
	int gen_tc_for_block(basic_block *bb_ptr);
	void gen_tc_for_quad(int index,quad *quad_ptr,int start,int end);
	x86_as_operand *get_dst(int quad_index,sym_tab_entry *arg1,int *same_loc);
	//! Regsiter Descriptor Table and the associated methods
	reg_desc_spec *reg_desc;
	//! Variable Descriptor Table and the associated methods
	var_desc_spec *var_desc;
	/* Used in the TC Generation */
	void gen_instr_for_the_quad(quad *quad_ptr,x86_as_operand *dst,int ind);
	void move_y_to_dst(sym_tab_entry *s, x86_as_operand *dst,int op,int same_loc);
	//! Next uses Info
	next_use *next_use_info;
	void init_register_desc();
	void init_variable_desc(vset *vs);
	int is_it_live_after_this_quad(int ind,sym_tab_entry *s);
	int does_reg_hold_only_this_var(reg_entry *r,sym_tab_entry *s);
	void next_use_analysis_for_block(basic_block *bb_ptr,vset *vs);
	void print_next_use_info(basic_block *bb_ptr);
	void spill_var(reg_entry *r,sym_tab_entry *s);
	void spill_reg(reg_entry *r);
	void check_sync_of_descs();
	void update_descs(sym_tab_entry *s, quad_set *next_uses,int liveness);
	void update_desc_for_res(quad *quad_ptr,x86_as_operand *dst);
	void spill_live_vars(int ind);
	void release_ds();
	int handle_special_case(sym_tab_entry *res,sym_tab_entry *arg1,int op);
	private:
	//! Current Basic Block Pointer
	basic_block *curr_bb_ptr;
	int need_to_spill(int op);
};
extern void emit_as (x86_as_oprtr mne, struct op_type op_ty, x86_as_operand *op1_ptr, x86_as_operand *op2_ptr,string cmt="" );
extern int init_before_tc_gen();


/* Entry point to Optimization */

//! Entry point to Target Code Optimization
int generate_optimized_target_code()
{
	optimize_ic *ic_optimizer;
	optimize_tc *tc_optimizer;
	quad *quad_ptr;
	int i,n;
	vector <quad *>tmp_input_proc_quads;

	i=0;

	init_before_tc_gen();

	ic_optimizer = new optimize_tc() ;

	tc_optimizer= (optimize_tc *)ic_optimizer;

	n = final_quad_arr_ptr->size();

	while ( i < n ){

		tmp_input_proc_quads.clear();

		for(;i<n;i++){
			quad_ptr = (*final_quad_arr_ptr)[i];
			tmp_input_proc_quads.push_back(quad_ptr);
			if(quad_ptr->op == PROC_END ){

				/* This is required for TC to happen */
				quad_ptr->arg1->sym_tab_ptr->calculate_local_space();
				break;
			}
		}

		input_quad_arr_ptr=&tmp_input_proc_quads;
		ic_optimizer -> do_data_flow_analysis();

		/* Takes the input quad arr as the param */
		tc_optimizer ->gen_tc(input_quad_arr_ptr);


		i++;

	}

	tc_optimizer->print_tc();


	return(0);

}

extern void print_as_quads();

//! Print the optimized Target Code
void
optimize_tc::print_tc()
{
//	print_as_quads();
}

//! Generate Target Code, given the input Quads
int
optimize_tc::gen_tc(vector <quad *> *input_quad_ptr)
{
	int i;
	basic_block *bb_ptr;

	for(i=0;i<no_of_blocks;i++){
		bb_ptr = basic_blocks_arr[i];
		gen_tc_for_block(bb_ptr);
	}

	return(0);

}


//! Generate Target Code for a Basic Block
int
optimize_tc :: gen_tc_for_block(basic_block *bb_ptr)
{
	int i;
	quad *quad_ptr;
	vset *vs;
	vset vars_used;

	int start,end;

	/* store our bb_ptr */
	curr_bb_ptr=bb_ptr;

	/* Find all the vars used in the block */
	vs=&vars_used;
	vars_used=bb_ptr->all_vars_used;


	/* Next use Analysis for the block */
	next_use_analysis_for_block(bb_ptr,vs);

	//print_next_use_info(bb_ptr);

	init_register_desc();
	init_variable_desc(vs);

	start=bb_ptr->start;
	end=bb_ptr->end;

	for(i=start;i<end;i++){
		quad_ptr=(*input_quad_arr_ptr)[i];
		gen_tc_for_quad(i,quad_ptr,start,end);
	}

	/* 
	spill at the end of the block .
	We can avoid spilling, if we have just processed the
	last block in the procedure
	*/
	if(quad_ptr -> op != PROC_END ){
		spill_live_vars((end-1));
	}

	/* Enable it later */
	release_ds();
	

	return(0);
}

//! Release Data Structures used in Target Code Optimization
void
optimize_tc :: release_ds()
{
	#if 0
	sym_tab_entry *v;
	vset *vs;
	reg_entry *r;
	reg_desc_map :: iterator iter;
	var_desc_map :: iterator iter1;

	/* deleting reg_desc */
	for (iter=reg_desc.begin();iter != reg_desc.end();iter++){
		r=(*iter).first;
		vs=(*iter).second;
		vs->clear();
		//delete(vs);
	}
	reg_desc.clear();
	#endif

	/* deleting var_desc */
	#if 0
	for(iter1=var_desc.begin();iter1 !=var_desc.end(); iter1++){
		v=(*iter1).first;
		ss=(*iter1).second;
		delete(ss);
	}
	var_desc.clear();

	delete var_desc;

	delete reg_desc ;


	#endif
	delete []next_use_info;
	next_use_info=NULL;
}

//! Spill all the Live Variables
void
optimize_tc :: spill_live_vars(int ind)
{
	sym_tab_entry *v;
	vset :: iterator iter;
	reg_entry *r;
	vset *vs;
	vset live_vars;

	live_vars=quad_info_arr[ind]->live_out;
	vs=&live_vars;

	printf("/* Spilling live vars */ \n");
	printf("/* Vars are \n");
	print_v_set(live_vars);
	printf("*/ \n");


	for(iter=vs->begin();iter != vs->end();iter++){
		v=(*iter);

		r=var_desc->get_single_entry(v);
		if( r!= NULL ){
			spill_var(r,v);
		}
	}

}


//! Initialize Variable Descriptor
void
optimize_tc :: init_variable_desc(vset *vs)
{
	vset :: iterator iter;
	sym_tab_entry *v;
	var_desc_entry *ve;
	
	var_desc=new var_desc_spec(vs);

	for(iter=vs->begin();iter != vs->end();iter++){
		v=(*iter);
		ve= new var_desc_entry(0);
		var_desc->var_desc_tab[v]=ve;
	}
}

extern reg_entry *reg_a,*reg_b,*reg_c,*reg_d,*reg_bp,*reg_sp,*reg_si,*reg_di;

//! Initialize Register Descriptor
void
optimize_tc :: init_register_desc()
{

	reg_desc = new reg_desc_spec;

	reg_desc->reg_desc_tab[reg_a]= new vset;
	reg_desc->reg_desc_tab[reg_b]= new vset;
	reg_desc->reg_desc_tab[reg_c]= new vset;
	reg_desc->reg_desc_tab[reg_d]= new vset;

}
	

//! Requires result
int 
optimize_tc ::requires_dst(int op)
{
	if(
	 (op==PROC_BEGIN) ||
	 (op==PROC_END)||
	 (op == RETURN ) ||
	 (op == RETRIEVE ) ||
	 (op == CALL ) ||
	 (op == LBL ) ||
	 (op == PARAM ) ||
    (op == GOTO  )
	 ){
	 	return(0);
	}
	return(1);

	 /*
	 Rags - TBD
    DIV ,
	 ADDR_OF,
    ASSIGN ,
	 L_INDEX_ASSIGN,
	 R_INDEX_ASSIGN,
	 */


}

//! Is there a need to Spill
int 
optimize_tc ::need_to_spill(int op)
{
	/* Any change in flow of control */
	if(
		((op >=LT ) && ( op <= NE ) ) ||
		( op == CALL ) ||
		( op == PROC_END ) ||
		( op == GOTO ) 
		){
		return(1);
	}
	return(0);
}

//! Generate Target Code for a Quad
void 
optimize_tc ::gen_tc_for_quad(int ind,quad *quad_ptr,int start,int end)
{
	sym_tab_entry *arg1,*arg2,*res;
	int op;
	int ret;
	x86_as_operand *dst;
	string quad_str;
	int rel_ind;
	int same_location;

	arg1=quad_ptr->arg1;
	arg2=quad_ptr->arg2;
	res=quad_ptr->res;
	op=quad_ptr -> op;
	rel_ind=(ind-start);

	quad_str = quad_ptr -> as_string();
	printf("/* %s */ \n",quad_str.c_str() );

	/* spill before the last instruction in a block*/
	ret=need_to_spill(op);
	if(ret == 1){
		/* 
		We need to know the live variables at this point
   	which is BEFORE processing the current statement.
		So we need to look at live variables for (ind-1)
		*/
		spill_live_vars((ind-1));
	}

	ret=handle_special_case(res,arg1,op);
	if(ret == 1 ){
		return;
	}

	

	ret=requires_dst(op);
	if(ret == 0 ){
		gen_instr_for_the_quad(quad_ptr,dst,ind);
		return;
	}

	same_location=0;

	/* Determine where we would store the result */
	dst=get_dst(rel_ind,arg1,&same_location);

	/* Move y to dst */
	move_y_to_dst(arg1,dst,op,same_location);

	gen_instr_for_the_quad(quad_ptr,dst,ind);

	/* update descriptors for 'res' */
	update_desc_for_res(quad_ptr,dst);

	/* free up regs if ther is no next use */
	update_descs(arg1,&next_use_info[rel_ind].arg1.next_uses,
                  next_use_info[rel_ind].arg1.liveness);
	
	update_descs(arg2,&next_use_info[rel_ind].arg2.next_uses,
                  next_use_info[rel_ind].arg2.liveness);

	//	var_desc->print_var_desc();
	check_sync_of_descs();
	return;
}

//! Handle ASSIGN case
int
optimize_tc :: handle_special_case(sym_tab_entry *res,sym_tab_entry *arg1,int op)
{
	reg_entry *r1;
	reg_set :: iterator iter;

	if( op != ASSIGN ){
		return(0);
	}

	/* res = arg1 */

	r1=var_desc->get_single_entry(arg1);
	if( r1 == NULL ){
		return(0);
	}

	/* 
	It is already in a register . We just change the
	descriptors
	*/

	reg_desc->add_entry(r1,res);
	var_desc->add_entry(res,r1);
		

	return(1);
}

//! Update Descriptors
void
optimize_tc :: update_descs(sym_tab_entry *s, quad_set *next_uses,int liveness)
{
	reg_entry *r;

	if(s == NULL ){
		return;
	}
	
	/* Is 's' in a register */
	r=var_desc->get_single_entry(s);

	if( r == NULL ){
		return;
	}

	/* if there is next use 's' */
	if(next_uses->size() > 0 ){
		return;
	}

	/* 's' is in register and there is no next use in 
	the block */

	if(liveness){
		spill_var(r,s);
	}

	reg_desc->clear_entry(r,s);
	var_desc->clear_entry(s,r);

}

//! Routine to check if the Variable Descriptor and Register Descriptor are in Sync
void
optimize_tc :: check_sync_of_descs()
{
	reg_set *rs;
	reg_set ::iterator iter1;
	reg_entry *r;
	vset *vs;
	vset :: iterator iter2;
	sym_tab_entry *v;
	int ret;

	var_desc_map :: iterator iter3;
	reg_desc_map :: iterator iter4;

	/* 
		for all entries in var desc check if reg desc
		has a same entry
	*/
	for(iter3=var_desc->var_desc_tab.begin(); iter3 != var_desc->var_desc_tab.end() ; iter3 ++){
		v=(*iter3).first;
		rs=var_desc->get_full_entry(v);

		for(iter1=rs->begin();iter1 != rs->end();iter1++){
			r=(*iter1);
			ret=reg_desc->check_entry(r,v);
			if(ret != 1 ){
				printf("Discrepancy in the entry (%s,%s)\n",
					v->get_name().c_str(),
					r->get_name(4).c_str());
			}
		}
	}

	for(iter4=reg_desc->reg_desc_tab.begin(); iter4 != reg_desc->reg_desc_tab.end() ; iter4 ++){
		r=(*iter4).first;
		vs=reg_desc->get_full_entry(r);
		for(iter2=vs->begin();iter2 != vs->end();iter2++){
			v=(*iter2);
			ret=var_desc->check_entry(v,r);
			if(ret != 1 ){
				printf("Discrepancy in the entry (%s,%s)\n",
					v->get_name().c_str(),
					r->get_name(4).c_str());
			}
		}
	}
}


//! Update the Descriptor Result
void
optimize_tc ::update_desc_for_res(quad *quad_ptr,x86_as_operand *dst)
{
	
	reg_entry *r;
	reg_set ::iterator iter;
	sym_tab_entry *res;
	int ret;

	/* RAGS : Check if res is valid for this quad */

	ret=quad_ptr->is_res_valid();
	if(ret == 0 ){
		return;
	}

	res=quad_ptr->res;

	assert(res != NULL );
	r=dst->reg_ptr;

	var_desc->add_entry(res,r);
	reg_desc->add_entry(r,res);

}

//! Make a assembly instruction operand 
x86_as_operand *
optimize_tc :: make_x86_operand(sym_tab_entry *arg2)
{
	reg_entry *r;
	x86_as_operand *z=NULL;

	/* Is 'arg2' in a register */
	r=var_desc->get_single_entry(arg2);

	/* in a register */
	if( r != NULL ){
		z = fetch_x86_operand(r);
	}else{
		z=fetch_x86_operand(arg2);
	}
	return(z);
}

//! Get the assembly instruction operand  for a register
x86_as_operand *
optimize_tc :: fetch_x86_operand(reg_entry *r)
{
	return(r->as_operand_ptr);
}

//! Get the assembly instruction operand  for a Id
x86_as_operand *
optimize_tc :: fetch_x86_operand(sym_tab_entry *arg2)
{
	x86_as_operand *z=NULL;
	x86_extension *e;

	z=NULL;
	e=(x86_extension *)arg2->extension;

	if( e != NULL ){
		z=e->as_operand_ptr;
	}

	if( z == NULL ){
		z = new x86_as_operand(arg2);
		e = new x86_extension(z);
		arg2->extension=(void *)e;
	}
	return(z);
}

extern x86_as_operand *as_reg_sp,*as_reg_si,*as_reg_di;
void
//! Get the assembly instructions for a Quad
optimize_tc ::gen_instr_for_the_quad(quad *quad_ptr,x86_as_operand *dst,int ind)
{
	x86_as_operand *as1,*as2,*as3;
	struct op_type op_typ;
	int ret;
	int ic_op;
	sym_tab_entry *arg1,*arg2,*res;

	arg1=quad_ptr->arg1;
	arg2=quad_ptr->arg2;
	res=quad_ptr->res;
	ic_op=quad_ptr -> op;

	/* defaults */
	op_typ.op1_mode = DIRECT ;
	op_typ.op2_mode = DIRECT ;
	op_typ.sz = 4;

	/* 
		Switch on the operator and generate
	   the instruction accordingly
	*/

	switch(ic_op)
	{
		case ASSIGN : 
						 break;
		case ADD :
						as2=make_x86_operand(arg2);
						emit_as(AS_ADD,op_typ,as2,dst);
						 break;
		case MUL :
						as2=make_x86_operand(arg2);
						emit_as(AS_MUL,op_typ,as2,dst);
						 break;
		case SUB :
						as2=make_x86_operand(arg2);
						emit_as(AS_SUB,op_typ,as2,dst);
						 break;
		case UMINUS :
						emit_as(AS_NEG,op_typ,dst,NULL);
						 break;
		case LT :
						as2=make_x86_operand(arg2);
						/* compare instruction */
						emit_as(AS_CMP,op_typ,as2,dst);

						/* jump instruction */
						as3 = fetch_x86_operand(res);
						emit_as(AS_JL,op_typ,as3,NULL);
						 break;
		case GT :
						as2=make_x86_operand(arg2);
						/* compare instruction */
						emit_as(AS_CMP,op_typ,as2,dst);

						/* jump instruction */
						as3 = fetch_x86_operand(res);
						emit_as(AS_JG,op_typ,as3,NULL);
						 break;
		case LE :
						as2=make_x86_operand(arg2);
						/* compare instruction */
						emit_as(AS_CMP,op_typ,as2,dst);

						/* jump instruction */
						as3 = fetch_x86_operand(res);
						emit_as(AS_JLE,op_typ,as3,NULL);
						 break;
		case GE :
						as2=make_x86_operand(arg2);
						/* compare instruction */
						emit_as(AS_CMP,op_typ,as2,dst);

						/* jump instruction */
						as3 = fetch_x86_operand(res);
						emit_as(AS_JGE,op_typ,as3,NULL);
						 break;
		case EQ :
						as2=make_x86_operand(arg2);
						/* compare instruction */
						emit_as(AS_CMP,op_typ,as2,dst);

						/* jump instruction */
						as3 = fetch_x86_operand(res);
						emit_as(AS_JE,op_typ,as3,NULL);
						 break;
		case NE :
						as2=make_x86_operand(arg2);
						/* compare instruction */
						emit_as(AS_CMP,op_typ,as2,dst);

						/* jump instruction */
						as3 = fetch_x86_operand(res);
						emit_as(AS_JNE,op_typ,as3,NULL);
						 break;
		case PARAM :
						as1=make_x86_operand(arg1);
						emit_as(AS_PUSH,op_typ,as1,NULL);
						break;
		case ADDR_OF :
						 break;
		case RETURN :
						/* if the returnable value is already
						in %eax , then dont generate any instruction.
						else, spill %eax and mov the returned 
						value there */

						ret=var_desc->check_entry(arg1,reg_a);
						if(ret == 1 ){
							break;
						}
						spill_reg(reg_a);

						as1=make_x86_operand(arg1);
						as2= fetch_x86_operand(reg_a);
						emit_as(AS_MOV,op_typ,as1,as2);
						break;
		/* Translation */
		case CALL :
						as1= fetch_x86_operand(arg1);
						emit_as(AS_CALL,op_typ,as1,NULL);
						as2 = fetch_x86_operand(arg2);
						as3 = fetch_x86_operand(reg_sp);
						emit_as(AS_ADD,op_typ,as2,as3);
						break;
		case LBL :
						as1= fetch_x86_operand(arg1);
						emit_as(AS_ALIGN,op_typ,NULL,NULL);
						emit_as(AS_LABEL,op_typ,as1,NULL);
						break;
		case GOTO :
						as1= fetch_x86_operand(arg1);
						emit_as(AS_JMP,op_typ,as1,NULL);
						break;
		case RETRIEVE :
						as2= fetch_x86_operand(reg_a);
						as1= fetch_x86_operand(arg1);
						emit_as(AS_MOV,op_typ,as2,as1);
						break;
		case PROC_BEGIN :
						/* Function entry */
						emit_as(AS_ALIGN,op_typ,NULL,NULL);
						as1 = fetch_x86_operand(arg1);
						emit_as(AS_GLOBAL,op_typ,as1,NULL);
						as1=fetch_x86_operand(arg1->sym_tab_ptr->entry_lbl);
						emit_as(AS_LABEL,op_typ,as1,NULL);
						as1 = fetch_x86_operand(reg_bp);
						emit_as(AS_PUSH,op_typ,as1,NULL);
						as2 = fetch_x86_operand(reg_sp);
						emit_as(AS_MOV,op_typ,as2,as1);
						as3 = fetch_x86_operand(arg1->sym_tab_ptr->local_space);
						emit_as(AS_SUB,op_typ,as3,as2);
					break;
		case PROC_END :
						as1 = fetch_x86_operand(reg_bp);
						as2 = fetch_x86_operand(reg_sp);
						emit_as(AS_MOV,op_typ,as1,as2);
						emit_as(AS_POP,op_typ,as1,NULL);
						emit_as(AS_RET,op_typ,NULL,NULL);
						break;
		case L_INDEX_ASSIGN :
						as2=make_x86_operand(arg2);
						emit_as(AS_ADD,op_typ,as2,dst);

						as3=make_x86_operand(res);
						emit_as(AS_MOV,op_typ,as3,fetch_x86_operand(reg_di));
	
						op_typ.op2_mode = INDIRECT;
						emit_as(AS_MOV,op_typ,fetch_x86_operand(reg_di),dst);

						 break;
		case R_INDEX_ASSIGN :
						as2=make_x86_operand(arg2);
						emit_as(AS_ADD,op_typ,as2,dst);

						emit_as(AS_MOV,op_typ,dst,fetch_x86_operand(reg_di));
	
						op_typ.op1_mode = INDIRECT;
						emit_as(AS_MOV,op_typ,fetch_x86_operand(reg_di),dst);
						 break;
		case DIV :
		default :
		             printf("Unsupported operator \n");
		             break;

  }
  return;


}

//! Generate Assembly instruction for moving y in x:= y + z to dest
void 
optimize_tc ::move_y_to_dst(sym_tab_entry *s, x86_as_operand *dst,int op,int same_location)
{
	reg_entry *r;
	x86_as_operand *y;
	struct op_type op_typ;

	if(same_location == 0 ){

		/* 
			if it is a literal , then we dont
		   have to check if it is in a register
		*/
		if(s->type->gettype() == CONSTAN ){
			y = fetch_x86_operand(s);
		}else{
			/* Is 's' in a register */
			r=var_desc->get_single_entry(s);


			/* in a register */
			if( r != NULL ){
				y = fetch_x86_operand(r);
			}else{
				y = fetch_x86_operand(s);
			}
		}


		op_typ.op1_mode = DIRECT ;
		op_typ.op2_mode = DIRECT ;
		op_typ.sz = 4;

		if(op == ADDR_OF ){
			emit_as(AS_LEA,op_typ,y,dst);
		}else{
			emit_as(AS_MOV,op_typ,y,dst);
		}
	}
		

}

//! Is the given Id live after this quad
int
optimize_tc :: is_it_live_after_this_quad(int ind,sym_tab_entry *s)
{
	int l;


	l=next_use_info[ind].arg1.liveness;

	return(l);
	
}

//! Does the register hold only this value
int
optimize_tc :: does_reg_hold_only_this_var(reg_entry *r,sym_tab_entry *s)
{
	vset *vs;
	vset :: iterator iter;

	vs = reg_desc->get_full_entry(r);

	if(vs->size() != 1 ){
		return(0);
	}

	if(vs->find(s) == vs ->end() ){
		return(0);
	}

	return(1);
}

//! Get the destination register
x86_as_operand  *
optimize_tc ::get_dst(int rel_ind,sym_tab_entry *s,int *same_location)
{
	reg_entry *r,*r1;
	x86_as_operand *l;

	r=var_desc->get_single_entry(s);

	/* 
	if y is already in register 'r' and y is dead after 
	this statement and 'r' holds no other variable
	then return 'r'
	*/
	if( r != NULL ){
		if(is_it_live_after_this_quad(rel_ind,s) == 0 ){
			if(does_reg_hold_only_this_var(r,s) == 1 ){
				printf("/* Releasing the register */\n");
				l = fetch_x86_operand(r);
				*same_location=1;
				reg_desc->clear_entry(r,s);
				var_desc->clear_entry(s,r);
				return(l);
			}
		}
	}

	/* 
	if any register 'r' is empty, return it 
	*/
	r=reg_desc->fetch_any_free_reg();
	if(r != NULL ){
		l = fetch_x86_operand(r);
		return(l);
	}

	/*
	Pick an arbitrary register and free it up
	*/
	r1=reg_desc->fetch_any_reg();
	spill_reg(r1);
	l = fetch_x86_operand(r1);
	return(l);
}

//! Spill the register r to all the variables
void
optimize_tc :: spill_reg(reg_entry *r)
{
	vset *vs;
	vset :: iterator iter;
	sym_tab_entry *v;
	vset tmp_set;
	vset *ts;
	
	vs=reg_desc->get_full_entry(r);
	tmp_set=*vs;
	ts=&tmp_set;

	for(iter=ts->begin();iter != ts->end();iter++){
		v=(*iter);
		spill_var(r,v);
	}
}


//! Spill the register r to a specific variable
void
optimize_tc :: spill_var(reg_entry *r,sym_tab_entry *s)
{
	x86_as_operand *as1,*as2;
	struct op_type op_typ;
	int sz;
	string cmt;

	op_typ.op1_mode = DIRECT ;
	op_typ.op2_mode = DIRECT ;

	as1 = fetch_x86_operand(r);
	as2 = fetch_x86_operand(s);

	sz=s->type->getsize();
	op_typ.sz = sz;

	cmt="Spilling Variable";
	emit_as(AS_MOV,op_typ,as1,as2,cmt);

	reg_desc->clear_entry(r,s);
	var_desc->clear_entry(s,r);

}

//! Computing Next Uses and Liveness
void
optimize_tc :: next_use_analysis_for_block(basic_block *bb_ptr,vset *vs)
{
	int i;
	quad *quad_ptr;
	vset *live_vs;
	vset :: iterator iter;
	live_next_use_t :: iterator iter1;
	live_next_use *ptr,*live_next_use_arr;
	int no_of_vars;

	sym_tab_entry *s;
	sym_tab_entry *arg1,*arg2,*res;
	live_next_use_t tmp_tbl;

	int arg1_valid,arg2_valid,res_valid=0;
	int start,end;
	int n;
	int rel_ind;

	live_vs=&bb_ptr->live_OUT;


	start=bb_ptr->start;
	end=bb_ptr->end;
	n=(end-start);

	no_of_vars=vs->size();

	next_use_info=new next_use[n];
	live_next_use_arr=new live_next_use[no_of_vars];

	/* Init the table */
	i=0;
	for(iter=vs->begin();iter != vs->end();iter++){
		s=(*iter);
		ptr=tmp_tbl[s]=&live_next_use_arr[i];
		/* Hack to add globals in live */
		if(live_vs->find(s) != live_vs->end()){
			ptr->liveness=1;
		}else{
			ptr->liveness=0;
		}
		i=i+1;
	}



	for(i=(end-1);i>=start;i--){
		quad_ptr=(*input_quad_arr_ptr)[i];
		arg1=quad_ptr->arg1;
		arg2=quad_ptr->arg2;
		res=quad_ptr->res;
		rel_ind=(i-start);

		arg1_valid=arg2_valid=res_valid=0;

		/* Attach to statement 'i' the info present */
		if(arg_valid(arg1)){
			next_use_info[rel_ind].arg1 = *tmp_tbl[arg1];
			arg1_valid=1;
		}
		if(arg_valid(arg2)){
			next_use_info[rel_ind].arg2 = *tmp_tbl[arg2];
			arg2_valid=1;
		}
		if(arg_valid(res)){
			next_use_info[rel_ind].res = *tmp_tbl[res];
			res_valid=1;
		}

		/* set 'x' as dead and no 'next use' */
		if(res_valid){
			tmp_tbl[res]->liveness=0;
			tmp_tbl[res]->next_uses.clear();
		}

		/* set 'y' as live and next use of 'y' as 'i' */
		if(arg1_valid){
			tmp_tbl[arg1]->liveness=1;
			tmp_tbl[arg1]->next_uses.insert(i);
		}

		/* set 'z' as live and next use of 'z' as 'i' */
		if(arg2_valid){
			tmp_tbl[arg2]->liveness=1;
			tmp_tbl[arg2]->next_uses.insert(i);
		}
	}

	delete []live_next_use_arr;
	
	printf("\n");

	return;
}

//! Print the Next use Info
void
optimize_tc :: print_next_use_info(basic_block *bb_ptr)
{
	int i,start,end;
	sym_tab_entry *arg1,*arg2,*res;
	quad *quad_ptr;
	int rel_ind;

	start=bb_ptr->start;
	end=bb_ptr->end;

	for(i=start;i<end;i++){
		quad_ptr=(*input_quad_arr_ptr)[i];
		arg1=quad_ptr->arg1;
		arg2=quad_ptr->arg2;
		res=quad_ptr->res;
		rel_ind=(i-start);
		printf("quad no=%s ",quad_ptr->as_string().c_str());
		if(arg_valid(arg1)){
			printf("next_uses[%s]=",arg1->name.c_str());
			print_quad_set(&next_use_info[rel_ind].arg1.next_uses);
			printf("liveness=%d  ",next_use_info[rel_ind].arg1.liveness);
		}
		if(arg_valid(arg2)){
			printf("next_uses[%s]=",arg2->name.c_str());
			print_quad_set(&next_use_info[rel_ind].arg2.next_uses);
			printf("liveness=%d  ",next_use_info[rel_ind].arg2.liveness);
		}
		if(arg_valid(res)){
			printf("next_uses[%s]=",res->name.c_str());
			print_quad_set(&next_use_info[rel_ind].res.next_uses);
			printf("liveness=%d  ",next_use_info[rel_ind].res.liveness);
		}
		printf("\n");
	}
}
	


//! Add the symbol tab entry into a Variable Set
void
basic_block :: add_to_vset(vset *vs,sym_tab_entry *arg)
{
	if(arg != NULL ){
		if(arg->is_it_a_constant() != 1 ){
			vs->insert(arg);
		}
	}
}
	

