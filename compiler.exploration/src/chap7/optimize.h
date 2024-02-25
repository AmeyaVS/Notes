#ifndef OPTIMIZE_H
#define OPTIMIZE_H

#include "stl.h"
#include "semantic_analysis.h"
#include "ic_gen.h"

/*!
	@file optimize.h
	@brief Optimization Header
*/

class dag_node;

//! Expression Set
typedef set <int,less<int> > expr_set;

//! Quad Set
typedef   set<int,less<int> > quad_set;

//! Variable Quad Mapping
typedef  map<sym_tab_entry*, quad_set *, less<sym_tab_entry *> > var_quad_set_map;

//! Quad to variable Mapping
typedef pair<int, sym_tab_entry *> quad_var;

//! variable to quad set Mapping
typedef  map<sym_tab_entry*, quad_set *, less<sym_tab_entry *> > var_quad_set_map;

//! variable set
typedef set<sym_tab_entry *,less<sym_tab_entry *> > vset;

/* du chaining */

//! Quad , variable pair
typedef pair<int, sym_tab_entry *> quad_var_pair;

//! du set
typedef set<quad_var_pair ,less<quad_var_pair > > du_set;

/* backedges */

//! edge
typedef pair<int, int> edge_t;

//! Loop
typedef set <int,less<int> > loop_t;

//! Identifier to DAG node
typedef map<sym_tab_entry*, dag_node *, less<sym_tab_entry* > > identifier_to_dag_node;

//! Identifier to DAG node
typedef	map <sym_tab_entry *,int,less <sym_tab_entry *> > lbl_blk_map;

//! Identifies a TWo Operand TAC
#define TWO_OPERAND_TAC(x) ( (x->op >= ADD) && (x ->op <= SUB) )

//! Is it ASSIGN TAC
#define ASSIGN_TAC(x) ( (x->op == ASSIGN) )

//! Make a Union of Two sets
#define make_union(A,B,C)  set_union((A).begin(),(A).end(),(B).begin(),(B).end(),inserter((C),(C).begin() ))

//! Difference of Two Sets
#define make_diff(A,B,C)  set_difference((A).begin(),(A).end(),(B).begin(),(B).end(),inserter((C),(C).begin() ))

//! Intersection of Two Sets
#define make_intersection(A,B,C)  set_intersection((A).begin(),(A).end(),(B).begin(),(B).end(),inserter((C),(C).begin() ))



class optimize_ic;

//! Represents a Basic Block
class basic_block
{
	public:

	//! Basic Block number
	int block_no;
	//! Start Quad number
	int start;
	//! End Quad number
	int end;
	//! Is it Dead
	int dead; /* dead block */
	//! Predecessors to the Block
	vector <int> predecessors;
	//! Succesors to the Block
	vector <int> successors;
	//! Handle to the IC optimizer
	optimize_ic *optimizer_ptr;
	//! Set of all the variables used in the Block
	vset all_vars_used;

	//! Constructor
	basic_block(int b_no,int st,optimize_ic *ptr)
	{
		block_no=b_no;
		start = st;
		optimizer_ptr = ptr;
		dead=0;
	}
	//! Destructor
	~basic_block();
	//! Array of all the DAG nodes
	vector <dag_node *> dag_arr ;
	//! All the leaves of the DAG nodes
	vector <dag_node *> leaves ;
	//! Address nodes in DAG array
	vector <dag_node *> addr_nodes ;

	void print();
	//void all_locals_used();
	void find_all_vars_used_in_block();
	void construct_dag();
	void print_dag();
	void reconstruct_tac();
	void remove_dead_code();
	void constant_folding();
	void identify_copy_prop_stmts();
	void set_dominators(quad_set *d);
	void print_quads();
	void print_liveness();
	void kill_all_g_nodes();

	vector <edge_t> get_back_edges();

	//! Dominators
	quad_set dominators;

	//! Reaching Definitions GEN
	quad_set rd_GEN;
	//! Reaching Definitions KILL
	quad_set rd_KILL;
	//! Reaching Definitions IN
	quad_set rd_IN;
	//! Reaching Definitions OUT
	quad_set rd_OUT;

	//! Available Expressions GEN
	expr_set e_GEN;
	//! Available Expressions KILL
	expr_set e_KILL;
	//! Available Expressions IN
	expr_set e_IN;
	//! Available Expressions OUT
	expr_set e_OUT;

	//! Live variable USE
	vset live_USE;
	//! Live variable DEF
	vset live_DEF;
	//! Live variable IN
	vset live_IN;
	//! Live variable OUT
	vset live_OUT;

	//! Def-Use chaining USE
	du_set du_USE;
	//! Def-Use chaining DEF
	du_set du_DEF;
	//! Def-Use chaining IN
	du_set du_IN;
	//! Def-Use chaining OUT
	du_set du_OUT;

	/* copy propagation */
	//! Copy Propagation GEN set
	quad_set c_GEN;
	//! Copy Propagation KILL set
	quad_set c_KILL;
	//! Copy Propagation IN set
	quad_set c_IN;
	//! Copy Propagation OUT set
	quad_set c_OUT;

	void create_GEN_set();
	void print_quad_set(quad_set *qs);
	void print_du_set(du_set ds);
	void print_v_set(vset qs);
	void create_KILL_set(quad_set all_kill_set);
	void build_ud_chains();
	void print_ud_chains();
	quad_set get_all_expr();
	void create_e_GEN_set();
	void create_e_KILL_set();
	void create_DEF_USE_sets();
	void create_du_DEF_USE_sets(du_set all_uses);
	void build_du_chains();
	void xtrapolate_liveness();
	void print_du_chains();
	void create_c_GEN_set();
	void create_c_KILL_set(quad_set univ_copy_stmts);
	void add_to_univ_copy_stmts(quad_set &univ_copy_stmts);
	void g_cse_elimination();
	int check_for_killing_defs(int quad_no,sym_tab_entry *inp_arg1, sym_tab_entry *inp_res);
	


	private:
	void add_to_vset(vset *vs,sym_tab_entry *arg);
	int get_last_evaluated_expr(int expr_no);
	void mark_g_cse_for_elim(int expr_no);
	dag_node * get_current(sym_tab_entry *s);
	dag_node * create_leaf_dag_node(oprtr op,sym_tab_entry *e);
	dag_node * find_dag_node(oprtr op,dag_node *l, dag_node *r );
	void kill_all_nodes();
	identifier_to_dag_node associated_dag_nodes;
	void move_attached_identifier(sym_tab_entry *s,dag_node *d);
	dag_node * get_dag_node(sym_tab_entry *s);
	dag_node * get_addr_node(sym_tab_entry *s);
	void set_current(sym_tab_entry *s, dag_node *d);
	quad_set get_defs_for_id(sym_tab_entry *arg,quad_set tmp_IN);
	void override_defs(sym_tab_entry *res,quad_set &tmp_IN,int curr_quad);
	void print_expr_set ( expr_set *es);
	quad_set get_uses_for_def(du_set &all_uses,sym_tab_entry *s);

	/* Added for Generating optimized Target Code */
public:
};

//! Represents the DAG node type
enum node_type
{
	LEAF,
	INTERIOR
};

//! Represents a Node in a DAG
class dag_node
{
	public:
		//! Label
		oprtr label;
		//! Left Child
		dag_node *left_child;
		//! Right Child
		dag_node *right_child;
		//! Additional Child
		dag_node *addl_child; /* used in case of some ops */
		//! All the parents
		set<dag_node *> parent; /* One of the parents */
		//! Selected identifier representing the result of the node
		sym_tab_entry *selected_identifier;
		//! Index in the DAG Array
		int dag_no;
		//! Running counter for all the DAG nodes created
		static int dag_counter;
		//! Is the DAG node killed
		int killed;
		//! represents lval
		int lval ; /* indicates if the node represents lval */
		//! Is it eliminated by Dead Code elimination Logic
		int eliminated;
		//! Is it already processed (used to prevent overwriting of leaves)
		int processed;
		//! Leaf or Interior
		node_type type_of_node; 
		//! Handle to the Basic Block that the DAG represents
		basic_block *bb_ptr;
		void check_for_overwrite(sym_tab_entry *res);

		//! Attached Identifiers
		vector <sym_tab_entry *> attached_identifiers; 

		//! Identifier represented by DAG node (for leaf)
		sym_tab_entry *id; /* Gets populated for a Leaf */
		void remove_attached_identifier(sym_tab_entry *e);
		void add_attached_identifier(sym_tab_entry *e);
		void print();
		void kill_parents();
		//! Constructor
		dag_node( sym_tab_entry *e,basic_block *b);
		//! Constructor
		dag_node(oprtr op,dag_node *l, dag_node *r,dag_node *a,basic_block *b);
		void regenerate_quad();
		string get_operator_as_string();
		sym_tab_entry * create_temporary();
		void populate_selected_identifier();
		sym_tab_entry * get_sym_tab_entry() ;
		dag_node * find_base_addr_node();
		void elim_dead_code();
		void perform_constant_folding();
		void gen_assign_statements();
		//! Quads generated for this DAG node
		vector <int> quads_generated;
		//! Values visible outside the Block (not used)
		vector <int> values_visible_outside_block;
		quad * add_to_quad_arr(oprtr lbl,
                             sym_tab_entry *arg1,
									  sym_tab_entry *arg2,
									  sym_tab_entry *res);

};

//! Is the res in res := arg1 + arg2  valid
#define IS_RES_VALID(opr)  \
		  ( \
        ( opr == ADD ) || \
        ( opr == MUL ) || \
        ( opr == DIV ) || \
        ( opr == SUB ) || \
        ( opr == UMINUS ) || \
        ( opr == ADDR_OF ) ||\
        ( opr == R_INDEX_ASSIGN ) \
		  )


//! Information at Quad Level
class quad_info
{
	public:
	//! All the reaching definitions for arg1 and arg2
	quad_set *ud_chains[2];	
	//! All the uses reached by the definition of 'res'
	quad_set *du_chains[1];	
	//! Live variables after this quad
	vset live_out;

	//! Constructor
	quad_info()
	{
		ud_chains[0]=NULL;
		ud_chains[1]=NULL;

		du_chains[0]=NULL;
	};
};



//! UD chain
class ud_chain
{
	public:
	//! UD chain
	vector <var_quad_set_map *> ud_chain_vector;

	//! Constructor
	ud_chain(int no_of_quads)
	{
		fill_n(back_inserter(ud_chain_vector), no_of_quads, (var_quad_set_map *)0);
	}

	int insert_ud_chain(int quad_no,sym_tab_entry *s,quad_set *qs);
	quad_set * get_ud_chain(int quad_no,sym_tab_entry *s);
};


//! Available Expression
class avail_expr
{
	public:
	//! Constructor
	avail_expr()
	{
	}
	//! Available Expression Set
	vector <int> avail_expr_set;

	//! AE mapped to quads
	vector <quad_set *>expr_to_quads;

	int get_avail_expr_no(int quad_no);
	int insert_avail_expr(int quad_no);
	int get_quad_no(int exp_no);
	void print();
	expr_set get_expr_set();
	string as_string(int exp_no);
	void clear();
};



//! Global Common Subexpression Node
class gcse_node
{
	public:
	//! Marked Quad
	int marked_quad;
	//! Followed Quads
	set <int,less<int> > followed_quads;
	//! Temporary
	sym_tab_entry *temporary;
};

//! Global Common Subexpression Elimination
class gcse
{
	public:
	//! GCSE Information
	vector <gcse_node  *>gcse_info;
	void insert_marked_quads(int marked_quad_no,int quad_no);
	int is_quad_in_marked_quad_set(int quad_no);
	int is_quad_in_followed_quad_set(int quad_no);
	void create_temporaries();
	int is_temp_known(int gcse_index);
	sym_tab_entry *get_temporary(int quad_no);

	//! Marked Quad Set
	set<int,less<int> > marked_quad_set;
	//! Followed Quad Set
	set<int,less<int> > followed_quad_set;

	void clear();
	private:
	void set_sym_tab(sym_tab *st);
	sym_tab *stab;
};	

//! Copy Propagation Information
class copy_prop_node
{
	public:
	//! Marked Quad
	int marked_quad;
	//! Replacement temporary
	sym_tab_entry *replacement;
	//! source
	sym_tab_entry *src;
	//! Followed Quads
	quad_set followed_quads;
	void add_to_followed_quads(int quad_no);
};

//! Copy Propagation
class copy_prop
{
	public:
	//! Copy Propagation Information
	vector <copy_prop_node *>copy_prop_info;
	//! Marked Quad Set
	set<int,less<int> > marked_quad_set;
	//! Followed Quad Set
	set<int,less<int> > followed_quad_set;
	void insert(int marked_quad_no,int quad_no,sym_tab_entry *arg1,sym_tab_entry *res);
	quad *replace(quad *qp,int quad_no);
	int is_quad_in_followed_quad_set(int quad_no);
	int is_quad_in_marked_quad_set(int quad_no);
	void  clear();
};

//! All the information about a Loop
class loop_info
{
	public:
	//! Back Edge
	edge_t back_edge;
	//! Header Block for the Loop
	int hdr_blk;
	//! Natural Loop
	loop_t natural_loop;
	//! Quads that can be moved to the Pre-Header
	quad_set can_be_moved_to_preheader;
	//! Exits
	quad_set exits;
	//! Loop Invariant Quads
	quad_set loop_invariants;
	void set_back_edge(edge_t b);
	void set_natural_loop(loop_t lp);
	void set_exits(quad_set exs);
	void print();
	//! ???
	vector <pair <int,int> > dependent_pairs;
	void add_dependent_quads(int quad_no,int dep_quad_no);
	int check_for_dependent_quads(int quad_no,quad_set preheader);
	//! Pre-Header Quads
	vector<sym_tab_entry *>preheader_quad_arr;
	//! ???
	sym_tab_entry *preheader_start;
};


//! Intermediate Code Optimization
class optimize_ic
{
	public:

	optimize_ic();
	//! Basic Blocks
	vector <basic_block *>basic_blocks_arr;
	//! Quad array after Local Optimization
	vector <quad *> quad_arr_after_local_opt;
	//! Original input quads 
	vector <quad *> *orig_input_quads;
	//! Number of Blocks
	int no_of_blocks;
	//! Symbol Table 
	sym_tab *sym_tab_ptr;
	void split_into_basic_blocks();
	int compare_quad_arrays(vector <quad *> *a,vector <quad *> *b);
	int get_block_no(int quad_no);
	void global_copy_propagation();
	int xform_input_for_copy_prop();
	int xform_input_for_code_motion();
	void print_blocks();
	void make_dags_from_basic_blocks();
	void build_tac_from_dags();
	void print_tac(vector <quad *> *qa_ptr);
	void print_dags();
	void perform_local_optmizations();
	void perform_global_optmizations(int optimize);
	void check_conds_for_code_motion();
	void mark_dead_blocks();
	void dead_code_elimination();
	void peephole_optimization();
	void remove_unneccesary_labels();
	void do_data_flow_analysis();
	void create_GEN_and_KILL_sets();
	void create_loop_info();
	void identify_back_edges();
	void make_ud_chains();
	void create_e_GEN_and_e_KILL_sets();
	void create_c_GEN_and_c_KILL_sets();
	int are_args_const(quad *quad_ptr);
	void create_live_DEF_and_live_USE_sets();
	void perform_liveness_analysis();
	void create_du_DEF_and_du_USE_sets();
	void make_du_chains();
	int is_it_from_loop(quad_set *rdefs,quad_set lp_invariants);
	int optimize_proc(vector <quad *> *quad_arr_ptr,int opt,int verbose);
	void copy_quads( vector <quad *>*outp_q_ptr);
	void global_cse_elimination();
	void find_dominators();
	int are_rdefs_from_outside_loop(quad_set *rdefs,loop_t lp);

	//! Universal Copy Statements
	quad_set universal_copy_stmts;
	//! Information for all quads
	vector <quad_info *>quad_info_arr;

	//! Loop Information
	vector <loop_info *> loop_info_arr;


	//! Block Start Quad numbers
	vector <int >block_starts;
	//! Universal Expression
	avail_expr universal_expr;
	//! GCSE quads
	gcse gcse_quads;
	int xform_input_for_gcse();
	void code_motion();
	void print_loop_info();
	//! Copy Propagation Information
	copy_prop copy_prop_quads;
	void identify_loops(edge_t bk_edg);
	quad_set identify_exits(quad_set lp);
	void insert_in_loop(int m);


	private:
	void clear_all();
	int is_this_the_only_def(int quad_no,loop_info *loop_info_ptr);
	int any_other_assign(int quad_no,loop_info *loop_info_ptr);
	void mark_loop_invariants(loop_info *ptr);
	int does_quad_dominate_all_exits(int quad_no,loop_info *loop_info_ptr);
	void build_succ_pred_relationships(lbl_blk_map l1 );
	void compute_reaching_definitions();
	void compute_available_expressions();
	void compute_copy_propagation_info();
//	vector <quad_set *>D; /* Dominators */
//	void make_union(quad_set &A, quad_set &B, quad_set &C);
//	void make_diff(quad_set &A, quad_set &B, quad_set &C);
//	void make_intersection(quad_set &A, quad_set &B, quad_set &C);

};


#endif
