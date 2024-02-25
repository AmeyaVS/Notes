#ifndef	LALR_H
#include "canonical_lr.h"

class lalr : public canonical_lr
{
	public:
	lalr (char *fname) : canonical_lr (fname) { ; };
  lr1item_set *condensed_canonical_collection[MAX_CANONICAL_COLLECTION];
	int new_state[MAX_CANONICAL_COLLECTION];
	void condense_can_coll();
	int no_of_condensed_sets;
	void print_merged_sets();
	int lr0_are_equal(lr1item_set *p1 , lr1item_set *p2 );
	void make_union(lr1item_set *p1 , lr1item_set *p2 );
	int my_find(lr1item *p, lr1item_set *s);
	void construct_lalr_parsing_table();
};
#define	LALR_H 1
#endif
