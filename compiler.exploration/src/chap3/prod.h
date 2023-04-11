#ifndef	PROD_H
#include "stl.h"
#include "symbol.h"
#include "parse_tab.h"

#define	EXCLUDE_EPSILON 1
#define	INCLUDE_EPSILON 2

class prod
{
 public:
   prod (char *rule);
   symbol *lhs;
   symbol_list rhs;
   void print_prod ();
   int no_of_rhs;
   int populate_FIRST ();
   int populate_FOLLOW ();
   void make_pentry ();
   void add_everything_in_FOLLOW (parse_tab_entry M[MAX_PARSE_TAB_SIZE][MAX_PARSE_TAB_SIZE]);
   void fill_in_a_entry (parse_tab_entry M[MAX_PARSE_TAB_SIZE][MAX_PARSE_TAB_SIZE]);
	int prod_table_index;
   parse_tab_entry pentry;
   int does_epsilon_exist (symbol_set * s);
   void compute_FIRST_for_rhs (symbol_set * set_ptr, int index);
 private:
     string prod_rule;
   int add_to_set (symbol_set * s1, symbol_set * s2);
   int merge (symbol_set * dst, symbol_set * src, int type );
	/*
	=
              INCLUDE_EPSILON);
				  */
   void insert_epsilon (symbol_set * s);
   int is_it_in_the_set (symbol * ptr, symbol_set * s1);
   void print_symbol_set (symbol_set * s1);
};


#define PROD_H
#endif
