#ifndef	SYMBOL_H
#include "stl.h"
#include <string>

#define	NON_TERMINAL 1
#define	TERMINAL 0

class symbol
{
 public:
   void print_sym ();
     symbol (char *name);
   struct symbol_lessthan
   {
      bool operator () (const symbol * s1, const symbol * s2) const
      {
         return strcmp (s1->symbol_name.c_str (),
                        s2->symbol_name.c_str ()) < 0;
      }
   };
     set < symbol *, symbol_lessthan > FIRST;
     set < symbol *, symbol_lessthan > FOLLOW;
   string symbol_name;
   int type;
   int val;
	void print_FIRST_FOLLOW_sets();
};


typedef vector < symbol * >symbol_list;
typedef set < symbol *,
   symbol::symbol_lessthan >
   symbol_set;

#define SYMBOL_H
#endif
