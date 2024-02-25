#include <stdio.h>
#include "prod.h"
#include "symbol.h"
#include "item.h"
 item::item () 
{
}

item::item (prod * p, int n) 
{
   prod_ptr = p;
   dot = n;
}
int 
item::can_add (symbol * X) 
{
   symbol_list::iterator iter;
   symbol * sym_ptr;
   int index, max_cnt, cnt;

   
      /*
         if(dot == START_OF_LHS ){
         index=0;
         }else{
         index = (dot+1);
         }
       */ 
      index = dot;
   max_cnt = prod_ptr->rhs.size ();
   if (index >= max_cnt) {
      return (0);
   }
   iter = prod_ptr->rhs.begin ();
   cnt = 0;
   while (cnt < max_cnt) {
      if (index == cnt) {
         sym_ptr = (*iter);
         break;
      }
      iter++;
      cnt++;
   }
   if (sym_ptr->symbol_name == X->symbol_name) {
      return (1);
   }
   return (0);
}

symbol * item::symbol_after_dot_is_non_terminal () 
{
   symbol_list::iterator iter;
   symbol * sym_ptr;
   int
      index,
      max_cnt,
      cnt;

   
      /*
         if(dot == START_OF_LHS ){
         index=0;
         }else{
         index = (dot+1);
         }
       */ 
      index = dot;
   max_cnt = prod_ptr->rhs.size ();
   if (index >= max_cnt) {
      return ((symbol *) NULL);
   }
   iter = prod_ptr->rhs.begin ();
   cnt = 0;
   while (cnt < max_cnt) {
      if (index == cnt) {
         sym_ptr = (*iter);
         if (sym_ptr->type == NON_TERMINAL) {
            return (sym_ptr);
         } else {
            return ((symbol *) NULL);
         }
      }
      iter++;
      cnt++;
   }
   return ((symbol *) NULL);
}
void 
item::print_item () 
{
   symbol_list::iterator iter;
   symbol * sym_ptr;
   int index, max_cnt, cnt;

   index = dot;
   sym_ptr = prod_ptr->lhs;
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
   return;
   
//      printf("%s,%d  ",prod_ptr->prod_rule.c_str(),dot);
}


