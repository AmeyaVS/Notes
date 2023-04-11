#include "lalr.h"
#include "c-stmt-lr-parse.h"

void
lalr:: condense_can_coll()
{
	int i,j,k;
	lr1item_set *base_set_ptr , *tmp_set_ptr;;

	memset(new_state,-1,sizeof(new_state) );
	i=j=k=0;

	for(i=0;i<no_of_sets_in_can_coll;i++){
		if(new_state[i] != -1 ){
			continue;
		}
		base_set_ptr = canonical_collection[i] ;
		tmp_set_ptr = new lr1item_set ;
		*(tmp_set_ptr) = *(base_set_ptr);
		condensed_canonical_collection[k] = tmp_set_ptr ;
		new_state[i]=k;


		for(j=i+1;j<no_of_sets_in_can_coll;j++){
			if(lr0_are_equal(base_set_ptr,canonical_collection[j]) == 1 ){
				make_union(tmp_set_ptr,canonical_collection[j]);
				new_state[j]=k;
			}
		}
		k++;
	}
	no_of_condensed_sets=k;
}

void
lalr::print_merged_sets()
{
	int i;

	printf("\nNo of condensed sets = %d \n",no_of_condensed_sets);

	for(i=0;i<no_of_sets_in_can_coll;i++){
		printf("old state=%d new state = %d \n",i,new_state[i]);
	}

	printf("\n******Refined Canonical Collection ******* \n");
	printf("No of sets in condensed collection = %d \n",no_of_condensed_sets);

	for(i=0;i<no_of_condensed_sets;i++){
		printf("I(%d) = ",i);
		print_lr1item_set(condensed_canonical_collection[i]);
	}

}
int
lalr:: lr0_are_equal(lr1item_set *p1 , lr1item_set *p2 )
{
	int cnt,max_cnt;
	lr1item *tmp_item_ptr;
	lr1item_set :: iterator iter;

	 max_cnt = p1->size();
	 cnt = 0;
	 iter = p1->begin();
	 while(cnt < max_cnt ){
		 tmp_item_ptr = (*iter);
		 if(my_find(tmp_item_ptr,p2) == 0 ) {
			 return(0);
			}
		 cnt ++;
		 iter ++;
	 }
   return (1);
}

void
lalr:: make_union(lr1item_set *p1 , lr1item_set *p2 )
{
   lr1item_set C;
   set_union (p1->begin (), p1->end (), p2->begin (),
                    p2->end (), inserter (C, C.begin ()),
                    lr1item_lessthan ());

	 *p1 = C;

	 return;
}

int
lalr:: my_find(lr1item *p, lr1item_set *s)
{
	int cnt,max_cnt;
	lr1item_set :: iterator iter;
	lr1item *tmp_item_ptr;

	max_cnt = s->size();
	cnt = 0;
	iter = s->begin();

	while(cnt < max_cnt ){
		tmp_item_ptr = (*iter);

		if((tmp_item_ptr -> prod_ptr == p->prod_ptr ) && 
				(tmp_item_ptr-> dot == p->dot ) ){

			return(1);
		}
		cnt ++;
		iter ++;
	}
	return(0);
}

void
lalr:: construct_lalr_parsing_table()
{
	int i;
	lr1item_set *lr1item_set_ptr;
	int cnt,max_cnt;
	lr1item_set ::iterator iter;
	lr1item *lr1item_ptr;

	gen_prod_tab_entries(); 
	memset(action_table,0,sizeof(action_table) );
	memset(goto_table,0,sizeof(goto_table) );

	lalr_type=1;
	nstate=new_state;

	for(i=0;i< no_of_sets_in_can_coll; i++){
		lr1item_set_ptr = canonical_collection[i];
		max_cnt = lr1item_set_ptr ->size();
		cnt=0;
		iter = lr1item_set_ptr->begin();
		while(cnt < max_cnt){
			lr1item_ptr = (*iter);
			fill_up_action_table(lr1item_ptr,lr1item_set_ptr, new_state[i]);
			iter ++;
			cnt++;
		}
		fill_up_reductions(lr1item_set_ptr,new_state[i]);

	}
	print_symbol_row(action_table,goto_table,no_of_condensed_sets);
}
