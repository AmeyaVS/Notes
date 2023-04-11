#include "grammar.h"
#include "item.h"

class slr : public grammar
{
 public:
	 slr(char *fname);
	 ~slr() { ; }
   void construct_parsing_table();
   int build_canonical_collection ();
   void print_canonical_sets ();
	void print_symbol_row();

 protected:
   int goto_set (item_set * J, item_set * I, symbol * X);
   int closure_set (item_set * J, item_set * I);
   int create_and_add_items_with_lhs (item_set * J, symbol * sym_ptr);
   void print_item_set (item_set * item_set_ptr);



   int is_it_present_in_can_coll (item_set * p);
   void add_in_can_coll (item_set * p);
   int are_equal (const item_set * p1, const item_set * p2);
   item_set *create_initial_set ();

   int find_in_item_set (item_set * item_set_ptr,
                         item * search_item_ptr);

   item_set *canonical_collection[MAX_CANONICAL_COLLECTION];
   int fill_up_action_table(item *item_ptr, item_set *item_set_ptr, int cnt);
   int get_index_in_can_coll(item_set *item_set_ptr);
   int fill_up_reductions(item_set *item_set_ptr,int current_state);
	int gen_prod_tab_entries () ;
   int no_of_sets_in_can_coll;

};
