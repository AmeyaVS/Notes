#ifndef	ITEM_H
#include "prod.h"
#include "symbol.h"
class item
{
 public:
   prod * prod_ptr;
   int dot;
     item ();
     item (prod * p, int posn);
   int can_add (symbol * X);
   symbol *symbol_after_dot_is_non_terminal ();
   void print_item ();
};
struct item_lessthan
{
   bool operator () (const item * s1, const item * s2) const
   {
		  
      if (s1->prod_ptr < s2->prod_ptr)
      {
         return (true);
      } else
      {
         if (s1->prod_ptr == s2->prod_ptr) {
            if ((s1->dot < s2->dot)) {
               return (true);
            }
         }
      }
      return (false);
   }
};
typedef set < item *, item_lessthan > item_set;


#define	START_OF_LHS	0

#define ITEM_H
#endif /*  */
