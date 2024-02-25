#include "grammar.h"
#include "lr1item.h"

typedef	int ( *tab_ptr) [300];

class canonical_lr : public grammar
{
 public:
	 canonical_lr(char *fname);
	 ~canonical_lr() { ; }
   void construct_canonical_parsing_table();
   int build_canonical_collection ();
   void print_canonical_sets ();
   void print_symbol_row (tab_ptr a, tab_ptr got , int no_of_rows);

	/* used in lalr */
	int *nstate;
	int lalr_type;

 protected:
   int goto_set (lr1item_set * J, lr1item_set * I, symbol * X);
   int closure_set (lr1item_set * J, lr1item_set * I);
   int create_and_add_lr1items_with_lhs (lr1item_set * J, symbol * sym_ptr,lr1item *j);
   void print_lr1item_set (lr1item_set * lr1item_set_ptr);



   int is_it_present_in_can_coll (lr1item_set * p);
   void add_in_can_coll (lr1item_set * p);
   int are_equal (const lr1item_set * p1, const lr1item_set * p2);
   lr1item_set *create_initial_set ();

   int find_in_lr1item_set (lr1item_set * lr1item_set_ptr,
                         lr1item * search_lr1item_ptr);

   lr1item_set *canonical_collection[MAX_CANONICAL_COLLECTION];
   int fill_up_action_table(lr1item *lr1item_ptr, lr1item_set *lr1item_set_ptr, int cnt);
   int get_index_in_can_coll(lr1item_set *lr1item_set_ptr);
   int fill_up_reductions(lr1item_set *lr1item_set_ptr,int current_state);
	int gen_prod_tab_entries () ;
   int no_of_sets_in_can_coll;
	 symbol *end_of_input_sym_ptr; 

};
