#include <stdio.h>
#include "prod.h"
#include "symbol.h"
#include "lr1item.h"

lr1item::lr1item() : item()
{
}
lr1item::lr1item (prod * p, int n, symbol *s)  : item(p,n)
{
	symbol_set tmp_set;

   prod_ptr = p;
   dot = n;
	 term = s;

	 p->compute_FIRST_for_rhs(&tmp_set,n);
	 if(p->does_epsilon_exist (&tmp_set)){
		 FIRST_for_rhs.insert(s);
	 }else{
		 FIRST_for_rhs = tmp_set;
	 }
}

void 
lr1item::print_lr1item () 
{
   symbol_list::iterator iter;
   symbol * sym_ptr;
   int index, max_cnt, cnt;

   index = dot;
   sym_ptr = prod_ptr->lhs;
	 printf("[ ");
   printf ("%s :", sym_ptr->symbol_name.c_str ());
   max_cnt = prod_ptr->rhs.size ();
   iter = prod_ptr->rhs.begin ();
   cnt = 0;
   while (cnt < max_cnt) {
      printf (" ");
      if (index == cnt) {
         printf (".");
      }
      sym_ptr = (*iter);
      printf ("%s", sym_ptr->symbol_name.c_str ());
      iter++;
      cnt++;
   }
   if (index == cnt) {
      printf (". ");
   }
	 printf(",%s ]",term->symbol_name.c_str() );
   return;
   
//      printf("%s,%d  ",prod_ptr->prod_rule.c_str(),dot);
}


