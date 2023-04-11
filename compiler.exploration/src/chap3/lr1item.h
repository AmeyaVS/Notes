#ifndef	LR1ITEM_H
#include "item.h"
class lr1item: public item
{
 public:
	 symbol *term;
	 symbol_set FIRST_for_rhs;

   lr1item ();
   lr1item (prod * p, int posn, symbol *s);
   void print_lr1item ();
};
struct lr1item_lessthan
{
   bool operator () (const lr1item * s1, const lr1item * s2) const
   {
      if (s1->prod_ptr < s2->prod_ptr)
      {
         return (true);
      } else {
         if (s1->prod_ptr == s2->prod_ptr) {
            if ((s1->dot < s2->dot)) {
               return (true);
            }else {
            	if ((s1->dot == s2->dot)) {
								if(s1 -> term ->val  < s2->term -> val ){
									return(true);
								}
							}
				    }
         }
      }
      return (false);
   }
};
typedef set < lr1item *, lr1item_lessthan > lr1item_set;


#define	START_OF_LHS	0

#define LR1ITEM_H
#endif /*  */
