#include "canonical_lr.h"
#include "c-stmt-lr-parse.h"
#include <algorithm>

prod_entry prod_table[MAX_PROD_TAB_SIZE];
int action_table[MAX_TAB_SIZE][MAX_TAB_SIZE];
int goto_table[MAX_TAB_SIZE][MAX_TAB_SIZE];
char *tname[MAX_TERM];
extern	int new_state[MAX_CANONICAL_COLLECTION];

typedef	int ( *tab_ptr) [MAX_TAB_SIZE];

canonical_lr::canonical_lr (char *fname) : grammar (fname)
{
}
int 
canonical_lr::goto_set (lr1item_set * J, lr1item_set * I, symbol * X) 
{
   int cnt = 0, max_cnt;

   lr1item_set::iterator iter;
   lr1item * lr1item_ptr, *tmp;
   prod * prod_ptr;
   lr1item_set tmp_set;
   int dot;
	 symbol *sym_ptr;

   max_cnt = I->size ();
   iter = I->begin ();
   while (cnt < max_cnt) {
      lr1item_ptr = (*iter);
      if (lr1item_ptr->can_add (X)) {
         prod_ptr = lr1item_ptr->prod_ptr;
         dot = lr1item_ptr->dot;
				 sym_ptr = lr1item_ptr ->term;
         tmp = new lr1item (prod_ptr, dot + 1,sym_ptr);
         if (!find_in_lr1item_set (&tmp_set, tmp)) {
            tmp_set.insert (tmp);
         }
      }
      iter++;
      cnt++;
   }
   closure_set (J, &tmp_set);
   return (0);
}
int 
canonical_lr::closure_set (lr1item_set * J, lr1item_set * I) 
{
   int cnt = 0, max_cnt;

   symbol * sym_ptr;
   lr1item_set::iterator iter;
   lr1item * lr1item_ptr;
   int added = 0;

   lr1item_set tmp_set;
   tmp_set=(*J) = (*I);
   
   do {
      added = 0;
      cnt = 0;
      max_cnt = J->size ();
      iter = J->begin ();
      while (cnt < max_cnt) {
         lr1item_ptr = (*iter);
         if ((sym_ptr =
               lr1item_ptr->symbol_after_dot_is_non_terminal ()) !=
              NULL) {
					  
            added +=
               create_and_add_lr1items_with_lhs (&tmp_set, sym_ptr,lr1item_ptr);
         }
         iter++;
         cnt++;
      }
      *J = tmp_set;
   } while (added);
   return (1);
}
int 
canonical_lr::create_and_add_lr1items_with_lhs (lr1item_set * J,
                                        symbol * sym_ptr,
																				lr1item *orig) 
{
   int cnt = 0,i;
	 int max;
	 symbol *tmp_sym_ptr;

   prod * prod_ptr;
   lr1item * lr1item_ptr;
   lr1item tmp_lr1item;
   symbol_set::iterator iter; 
   int added = 0;


   for (cnt = 0; cnt < no_of_productions; cnt++) {
      prod_ptr = productions[cnt];
      if (prod_ptr->lhs->symbol_name == sym_ptr->symbol_name) {
				 i=0;
				 max=orig->FIRST_for_rhs.size();
				 iter = orig->FIRST_for_rhs.begin();
				 while(i<max){
					 tmp_sym_ptr = (*iter);
					 tmp_lr1item.prod_ptr = prod_ptr;
					 tmp_lr1item.dot = 0;
					 tmp_lr1item.term = tmp_sym_ptr;
					 if (!find_in_lr1item_set (J, &tmp_lr1item)) {
							
							lr1item_ptr = new lr1item (prod_ptr, START_OF_LHS,tmp_sym_ptr);
							J->insert (lr1item_ptr);
							added++;
					 }
					 iter ++;
					 i++;
				 }
      }
   }
   return (added);
}
void 
canonical_lr::print_lr1item_set (lr1item_set * lr1item_set_ptr) 
{
   lr1item * lr1item_ptr;
   int max_cnt, cnt = 0;

   lr1item_set::iterator iter;

   max_cnt = lr1item_set_ptr->size ();
   iter = lr1item_set_ptr->begin ();
   while (cnt < max_cnt) {
      lr1item_ptr = (*iter);
      lr1item_ptr->print_lr1item ();
      if (cnt != (max_cnt - 1)) {
         printf ("\n      ");
      }
      iter++;
      cnt++;
   }
   printf ("\n\n");
}
int 
canonical_lr::find_in_lr1item_set (lr1item_set * lr1item_set_ptr,
                           lr1item * search_lr1item_ptr) 
{
   if (lr1item_set_ptr->find (search_lr1item_ptr) != lr1item_set_ptr->end ()) {
      return (1);
   }
   return (0);
}
int 
canonical_lr::build_canonical_collection () 
{
   symbol_collection::iterator iter;
   symbol * sym;
   int no_of_elem, cnt;

   lr1item_set tmp_set;
   lr1item_set * lr1item_set_ptr;
   lr1item_set * I;
   int added = 0, i;

   lr1item_set_ptr = create_initial_set ();
   add_in_can_coll (lr1item_set_ptr);
   
   do {
      added = 0;
      for (i = 0; i < no_of_sets_in_can_coll; i++) {
         I = canonical_collection[i];
         cnt = 0;
         iter = symbol_table.begin ();
         no_of_elem = symbol_table.size ();
         while (cnt < no_of_elem) {
            tmp_set.clear ();
            sym = (*iter).second;
            goto_set (&tmp_set, I, sym);
            if (tmp_set.size ()) {
               if (!is_it_present_in_can_coll (&tmp_set)) {
                  added++;
                  add_in_can_coll (&tmp_set);
               }
            }
            iter++;
            cnt++;
         }
      }
   } while (added);
   return (1);
}
int 
canonical_lr::is_it_present_in_can_coll (lr1item_set * p) 
{
   int i;

   for (i = 0; i < no_of_sets_in_can_coll; i++) {
      if (p->size () == canonical_collection[i]->size ()) {
         if (are_equal (p, canonical_collection[i])) {
            return (1);
         }
      }
   }
   return (0);
}
void 
canonical_lr::add_in_can_coll (lr1item_set * p) 
{
   lr1item_set * j;
   j = new lr1item_set;
   *j = *p;
   canonical_collection[no_of_sets_in_can_coll] = j;
   no_of_sets_in_can_coll++;
}
void 
canonical_lr::print_canonical_sets () 
{
   int i;

	printf("\n*********  Canonical Collection *********\n");
	printf("No of sets in Canonical Collection=%d \n",no_of_sets_in_can_coll);
   for (i = 0; i < no_of_sets_in_can_coll; i++) {
      printf ("I(%d)= ", i);
      print_lr1item_set (canonical_collection[i]);
   }
   return;
}
int 
canonical_lr::are_equal (const lr1item_set * p1, const lr1item_set * p2) 
{
   lr1item_set C;
   set_difference (p1->begin (), p1->end (), p2->begin (),
                    p2->end (), inserter (C, C.begin ()),
                    lr1item_lessthan ());
   if (C.size () != 0) {
      return (0);
   }
	C.clear();

   set_difference (p2->begin (), p2->end (), p1->begin (),
                    p1->end (), inserter (C, C.begin ()),
                    lr1item_lessthan ());

   if (C.size () != 0) {
      return (0);
   }

   return (1);
}

lr1item_set * canonical_lr::create_initial_set () 
{
   lr1item * lr1item_ptr;
   lr1item_set tmp_lr1item_set;
   lr1item_set * closure_set_ptr;
   string tmp_prod_str, tmp_rhs;

	 end_of_input_sym_ptr = new symbol("$");
   lr1item_ptr = new lr1item (augmented_prod_ptr, START_OF_LHS,end_of_input_sym_ptr);
   tmp_lr1item_set.insert (lr1item_ptr);
   closure_set_ptr = new lr1item_set;
   closure_set (closure_set_ptr, &tmp_lr1item_set);
   return (closure_set_ptr);
}

void
canonical_lr:: construct_canonical_parsing_table()
{
	int i;
	lr1item_set *lr1item_set_ptr;
	int cnt,max_cnt;
	lr1item_set ::iterator iter;
	lr1item *lr1item_ptr;

	gen_prod_tab_entries(); 
	for(i=0;i< no_of_sets_in_can_coll; i++){
		lr1item_set_ptr = canonical_collection[i];
		max_cnt = lr1item_set_ptr ->size();
		cnt=0;
		iter = lr1item_set_ptr->begin();
		while(cnt < max_cnt){
			lr1item_ptr = (*iter);
			fill_up_action_table(lr1item_ptr,lr1item_set_ptr, i);
			iter ++;
			cnt++;
		}
		fill_up_reductions(lr1item_set_ptr,i);

	}
	print_symbol_row(action_table,goto_table,no_of_sets_in_can_coll);
}

int 
canonical_lr::fill_up_action_table(lr1item *lr1item_ptr, lr1item_set *lr1item_set_ptr, int current_state)
{
	symbol *sym_ptr;
	lr1item_set tmp_set;
	prod *prod_ptr=lr1item_ptr->prod_ptr;
	int dot=lr1item_ptr->dot;
	int index;

	if((prod_ptr -> lhs->symbol_name == augmented_start) &&
		(dot == 1 ) ){
//		printf("action_table[%d][$]=accept\n",current_state);
		action_table[current_state][0]=ACCEPT;
	}
	if(dot <prod_ptr->no_of_rhs){
		if((sym_ptr=prod_ptr->rhs[dot])->type == TERMINAL ){
			goto_set(&tmp_set,lr1item_set_ptr,sym_ptr);
			index=get_index_in_can_coll(&tmp_set);
			if(index == -1){
				printf("No set found matching the given set for goto(I%d,%s)\n",current_state,sym_ptr->symbol_name.c_str());
				printf("Searching for the set \n");
				print_lr1item_set (&tmp_set);
				exit(0);
			}
			if(action_table[current_state][sym_ptr->val] == 0 ){
				action_table[current_state][sym_ptr->val]=index;
//				printf("action_table[%d][%s]=%d\n",current_state,sym_ptr->symbol_name.c_str(),index );
			}else{
				if(action_table[current_state][sym_ptr->val] !=index){
					printf("Conflicting entry at action_table[%d][%s]  oldval=%d newval=%d\n",current_state,sym_ptr->symbol_name.c_str(),action_table[current_state][sym_ptr->val],index );
					// exit(1);
				}
			}
		}
	}else{
			if(prod_ptr->lhs->symbol_name != augmented_start) {
				sym_ptr = lr1item_ptr->term;
				if(action_table[current_state][sym_ptr->val] == 0 ){
//					printf("action_table[%d][%s]=%d\n",current_state,sym_ptr->symbol_name.c_str(),-(prod_ptr->prod_table_index) );
					action_table[current_state][sym_ptr->val] = -(prod_ptr->prod_table_index);
				} else {
					if(action_table[current_state][sym_ptr->val] != -(prod_ptr->prod_table_index)){
						printf("Conflicting entry at action_table[%d][%s]  oldval=%d newval=%d\n",current_state,sym_ptr->symbol_name.c_str(),action_table[current_state][sym_ptr->val],-(prod_ptr->prod_table_index) );
// 						exit(1);
					}
				}
			}
	}
	return(1);
}

int
canonical_lr:: get_index_in_can_coll(lr1item_set *lr1item_set_ptr)
{
	int i;

	for(i=1;i<no_of_sets_in_can_coll;i++){
		if(are_equal(canonical_collection[i],lr1item_set_ptr) ){
			if(lalr_type){
				return(nstate[i]);
			}else{
				return(i);
			}
		}
	}
	return(-1);
}

int
canonical_lr:: fill_up_reductions(lr1item_set *lr1item_set_ptr,int current_state)
{
   symbol_collection::iterator iter;
   symbol *sym_ptr;
   int max_cnt, cnt;
	int index;
	lr1item_set tmp_lr1item_set;

   iter = symbol_table.begin ();

   max_cnt = symbol_table.size ();

   cnt = 0;
   while (cnt < max_cnt) {
      sym_ptr = (*iter).second;
      if(sym_ptr->type == NON_TERMINAL){
			tmp_lr1item_set.clear();
			goto_set(&tmp_lr1item_set,lr1item_set_ptr,sym_ptr);
			index=get_index_in_can_coll(&tmp_lr1item_set);
			if(index != -1 ){
//				printf("goto_table[%d][%s]=%d\n",current_state,sym_ptr->symbol_name.c_str(),index );
				goto_table[current_state][sym_ptr->val]=index;
			}
		}
      iter++;
      cnt++;
   }
	return(1);
}

int 
canonical_lr::gen_prod_tab_entries () 
{
   int cnt = 0;
	int rule_no;
   symbol_collection::iterator iter;
   symbol *sym;
   int max_cnt;

   prod * prod_ptr;
   for (cnt = 0; cnt < no_of_productions; cnt++) {
      prod_ptr = productions[cnt];
		rule_no = cnt +1;
		prod_table[rule_no].lhs_elem=prod_ptr->lhs->val;
   	prod_table[rule_no].no_of_elem_in_rhs=prod_ptr->rhs.size();
   	prod_table[rule_no].rule_no=rule_no;
		prod_ptr->prod_table_index=rule_no;
   }

   iter = symbol_table.begin ();

   max_cnt = symbol_table.size ();

   cnt = 0;
   while (cnt < max_cnt) {
      sym = (*iter).second;
			if(sym->type == TERMINAL ){
				tname[sym->val-TERM_BASE]=strdup(sym->symbol_name.c_str());
			}
      iter++;
      cnt++;
   }
   return (0);
}




void
canonical_lr::print_symbol_row (tab_ptr act, tab_ptr got,int no_of_rows)
{
   symbol_collection::iterator iter;
   symbol *sym;
	tab_ptr tab;
   int max_cnt, cnt;
	int i;
	int chk=TERMINAL;
	char print_buf[600];
	int offsets[100];
	char tst_buf[600];
	char line_buf[600];
	int tmp,index=0,n,symb_no=0;
	char type_of_act;


	memset(tst_buf,' ',sizeof(tst_buf));
	memset(line_buf,'-',sizeof(line_buf));

	n=sprintf(print_buf,"%s","State |");
//	offsets[symb_no]=n;
	index = n;
//	symb_no++;
again:
   iter = symbol_table.begin ();

   max_cnt = symbol_table.size ();

   cnt = 0;
   while (cnt < max_cnt) {
      sym = (*iter).second;
		if(sym->type == chk){
      	n=sprintf(&print_buf[index],"  %s  ",sym->symbol_name.c_str());
			offsets[symb_no]=(index+index+n-1)/2;
//			printf("%d(%s) %d \n",symb_no,sym->symbol_name.c_str(),offsets[symb_no]);
			index +=n;
			symb_no ++;
		}
      iter++;
      cnt++;
   }

	if(chk==TERMINAL){
      	n=sprintf(&print_buf[index],"  %s  ","$" );
			offsets[symb_no]=(index+index+n-1)/2;
	//		printf("%d(%s) %d \n",symb_no,"$",offsets[symb_no]);
			index +=n;
			symb_no ++;
		chk = NON_TERMINAL;
		goto again;
	}
	tst_buf[index]=0;
	line_buf[index]=0;

	printf("%s \n",line_buf);
	printf("%s \n",print_buf);
	printf("%s \n",line_buf);


   for (i = 0; i < no_of_rows; i++) {
	memset(tst_buf,' ',sizeof(tst_buf));
	n=sprintf(tst_buf," %03d  |",i);
	tst_buf[n]=' ';
	chk = TERMINAL;
	tab=act;
	symb_no=0;
again1:
   iter = symbol_table.begin ();

   max_cnt = symbol_table.size ();

   cnt = 0;
   while (cnt < max_cnt) {
      sym = (*iter).second;
		if(sym->type == chk){
			if(tab[i][sym->val] ){
				tmp= tab[i][sym->val];
				type_of_act= tmp < 0 ? 'r' : 's' ; 
				if(chk == NON_TERMINAL){
					n=sprintf(&tst_buf[offsets[symb_no]],"%d",tab[i][sym->val]);
				}else {
					n=sprintf(&tst_buf[offsets[symb_no]],"%c%d",type_of_act,abs(tab[i][sym->val]));
				}
				tst_buf[offsets[symb_no]+n]=' ';
			}
			symb_no ++;
		}
      iter++;
      cnt++;
   }

	if(chk==TERMINAL){
		tmp= tab[i][0];
		if(tmp){
			if(tmp == ACCEPT ){

				n=sprintf(&tst_buf[offsets[symb_no]],"acc");
			}else{
				type_of_act= tmp < 0 ? 'r' : 's' ; 
				n=sprintf(&tst_buf[offsets[symb_no]],"%c%d",type_of_act,abs(tab[i][0]));
			}
			tst_buf[offsets[symb_no]+n]=' ';
		}
		symb_no ++;
		chk = NON_TERMINAL;
		tab = got;
		goto again1;
	}

	tst_buf[index]=0;
	printf("%s \n",tst_buf);
	}
	printf("%s \n",line_buf);

}

