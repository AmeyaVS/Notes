#ifndef	GRAMMAR_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "stl.h"
#include "symbol.h"
#include "prod.h"
//#include "item.h"


#define MAX_LINE 400
#define MAX_FILE_NAME 400
#define MAX_CANONICAL_COLLECTION 2000
#define	MAX_TERM	100

#define	EPSILON	0

#define	TERM_BASE	256
#define	NONTERM_BASE	1




struct sym_map_lessthan
{

   bool operator () (const char *s1, const char *s2) const
   {
      return strcmp (s1, s2) < 0;
   }
};

typedef map < const char *, symbol *,
   sym_map_lessthan > symbol_collection;
typedef vector < prod * >prod_list;

class grammar
{
 public:
   grammar (char *fname);
	 virtual ~grammar() { ; };
   void print_productions ();
   void print_symbol_table ();
   void calculate_FIRST ();
   void calculate_FOLLOW ();
	void print_FIRST_FOLLOW_sets();
   void fill_in_grammar_table ();
   int gen_parsing_tab_entries (parse_tab_entry M[MAX_PARSE_TAB_SIZE][MAX_PARSE_TAB_SIZE]);
	 void init_all_tables();

   void create_augmented_grammar();
   virtual int build_canonical_collection () { return(1); };
   virtual void print_canonical_sets () { };
	 virtual void construct_parsing_table() { };
	symbol * get_sym_ptr(int val);

 protected:
     symbol * epsilon_symbol_ptr;
   int is_it_blank_line (char *str);
   char file_name[MAX_FILE_NAME];
   void replace_ws (char *str);
   symbol_collection symbol_table;
   int no_of_symbols;
   prod_list productions;
   int no_of_productions;
   void fill_in_symbol_table (char *str);
   prod *fill_in_prod_info (char *str);
   int is_there_an_epsilon_prod (symbol * sym_ptr);
   void order_terminals ();
   string augmented_start;
	 prod *augmented_prod_ptr;
};
#define	GRAMMAR_H 1
#endif
