#include "grammar.h"

grammar::grammar (char *fname)
{
   strcpy (file_name, fname);
}

/* Identfies Blank lines and returns 1 on finding a blank line */
int
grammar::is_it_blank_line (char *str)
{
   int len = strlen (str);
   int i;
   int c;

   for (i = 0; i < len; i++) {
      c = str[i];
      if (isprint (c)) {
         if (!isspace (c)) {
            return (0);
         }
      }
   }
   return (1);
}


/* Fills in symbol hash set and the productions list */
void
grammar::fill_in_grammar_table ()
{
   FILE *fp;
   char str[MAX_LINE];
   prod *prod_ptr;

   fp = fopen (file_name, "r");

   if (fp == NULL) {
      printf ("The file <%s> does not exist - exiting \n", file_name);
      exit (1);
   }


   while (fgets (str, sizeof (str), fp) != NULL) {
      if (is_it_blank_line (str)) {
         continue;
      }

      /* Filling in Symbol Table */
      fill_in_symbol_table (str);

      /* Fill in Productions */
      prod_ptr = fill_in_prod_info (str);
      productions.push_back (prod_ptr);

   }
   order_terminals ();
   no_of_productions = productions.size ();
}

void
grammar::fill_in_symbol_table (char *str)
{

   char tmp[MAX_LINE];
   char *tok;
   char *rest_of_str;
   int lhs;
   char *key;
   symbol *sym_ptr;

   symbol_collection::iterator iter;

   epsilon_symbol_ptr = new symbol ("epsilon");

   replace_ws (str);
   strcpy (tmp, str);
   rest_of_str = tmp;
   lhs = 1;
   while ((tok = strtok (rest_of_str, " ")) != NULL) {
      if (strcmp (tok, ":") == 0) {
         rest_of_str = NULL;
         lhs = 0;
         continue;
      }

      if (strcmp (tok, "epsilon") == 0) {
         rest_of_str = NULL;
         continue;
      }

      rest_of_str = NULL;

      if ((iter = symbol_table.find (tok)) == symbol_table.end ()) {
         key = strdup (tok);
         sym_ptr = symbol_table[key] = new symbol (tok);
      } else {
         sym_ptr = (*iter).second;
      }
      if (lhs == 1) {
         sym_ptr->type = NON_TERMINAL;
      }
   }
}

prod *
grammar::fill_in_prod_info (char *str)
{

   char tmp[MAX_LINE];
   char *tok;
   char *rest_of_str;
   int lhs;
   prod *prod_ptr;


   replace_ws (str);
   strcpy (tmp, str);
   prod_ptr = new prod (str);
   rest_of_str = tmp;
   lhs = 1;
   while ((tok = strtok (rest_of_str, " ")) != NULL) {
      if (strcmp (tok, ":") == 0) {
         rest_of_str = NULL;
         lhs = 0;
         continue;
      }

      if (strcmp (tok, "epsilon") == 0) {
         rest_of_str = NULL;
         continue;
      }


      if (lhs == 1) {
         prod_ptr->lhs = symbol_table[tok];
      } else {
         prod_ptr->rhs.push_back (symbol_table[tok]);
      }
      rest_of_str = NULL;

   }
   prod_ptr->no_of_rhs = prod_ptr->rhs.size ();

   return (prod_ptr);
}

void
grammar::replace_ws (char *str)
{
   int i;
   int n = strlen (str);

   for (i = 0; i < n; i++) {
      if (isspace (str[i])) {
         str[i] = ' ';
      }
   }
}

void
grammar::print_symbol_table ()
{
   symbol_collection::iterator iter;
   symbol *sym;
   int max_cnt, cnt;

   iter = symbol_table.begin ();

   max_cnt = symbol_table.size ();

   cnt = 0;
   while (cnt < max_cnt) {
      sym = (*iter).second;
      sym->print_sym ();
      iter++;
      cnt++;
   }
}

void
grammar::print_productions ()
{
   int i;
   prod *prod_ptr;

   for (i = 0; i < no_of_productions; i++) {
      prod_ptr = productions[i];
      prod_ptr->print_prod ();
   }
}
void
grammar::calculate_FIRST ()
{
   symbol_collection::iterator iter;
   symbol *sym;
   prod *prod_ptr;
   int max_cnt, cnt, added;

   iter = symbol_table.begin ();

   max_cnt = symbol_table.size ();

   cnt = 0;
   while (cnt < max_cnt) {
      sym = (*iter).second;

      if (sym->type == NON_TERMINAL) {
         if (is_there_an_epsilon_prod (sym)) {
            sym->FIRST.insert (epsilon_symbol_ptr);
         }
      } else {
         sym->FIRST.insert (sym);
      }
      iter++;
      cnt++;
   }


   do {
      cnt = 0;
      added = 0;
      for (cnt = 0; cnt < no_of_productions; cnt++) {
         prod_ptr = productions[cnt];
         added += prod_ptr->populate_FIRST ();
      }
   } while (added);
}

int
grammar::is_there_an_epsilon_prod (symbol * sym_ptr)
{
   int cnt = 0;
   prod *prod_ptr;

   for (cnt = 0; cnt < no_of_productions; cnt++) {
      prod_ptr = productions[cnt];
      if (prod_ptr->lhs->symbol_name == sym_ptr->symbol_name) {
         if (prod_ptr->no_of_rhs == 0) {
            return (1);
         }
      }
   }
   return (0);
}

/* FOLLOW related */
void
grammar::calculate_FOLLOW ()
{
   symbol *sym;
   prod *prod_ptr;
   int cnt, added;

   /* for the start symbol '$' is the FOLLOW */
   prod_ptr = productions[0];
   sym = new symbol ("$");
   (prod_ptr->lhs->FOLLOW).insert (sym);


   do {
      cnt = 0;
      added = 0;
      for (cnt = 0; cnt < no_of_productions; cnt++) {
         prod_ptr = productions[cnt];
         added += prod_ptr->populate_FOLLOW ();
			prod_ptr ->prod_table_index = cnt+1;
      }
   } while (added);
}

void
grammar:: print_FIRST_FOLLOW_sets()
{
   symbol_collection::iterator iter;
   symbol *sym;
   int max_cnt, cnt;

   iter = symbol_table.begin ();

   max_cnt = symbol_table.size ();

   cnt = 0;
   while (cnt < max_cnt) {
      sym = (*iter).second;
      if (sym->type == NON_TERMINAL) {
      	sym->print_FIRST_FOLLOW_sets ();
		}
      iter++;
      cnt++;
   }
}

void
grammar::order_terminals ()
{
   symbol_collection::iterator iter;
   symbol *sym;
   int max_cnt, cnt;
   int nonterm_count = NONTERM_BASE, term_count = TERM_BASE;

   iter = symbol_table.begin ();
   max_cnt = symbol_table.size ();

   cnt = 0;
   while (cnt < max_cnt) {
      sym = (*iter).second;

      if (sym->type == TERMINAL) {
         sym->val = term_count;
         term_count++;
      }
      if (sym->type == NON_TERMINAL) {
         sym->val = nonterm_count;
         nonterm_count++;
      }
      iter++;
      cnt++;
   }
}

void
grammar::init_all_tables()
{
   fill_in_grammar_table ();

   /* Calculate the FIRST set */
   calculate_FIRST ();

   /* Calculate the FOLLOW set */
   calculate_FOLLOW ();

}

symbol * 
grammar::get_sym_ptr(int val)
{
   symbol_collection::iterator iter;
   symbol *sym;
   int max_cnt, cnt;

   iter = symbol_table.begin ();
   max_cnt = symbol_table.size ();

   cnt = 0;
   while (cnt < max_cnt) {
      sym = (*iter).second;

      if (sym->val == val) {
			return(sym);
      }
      iter++;
      cnt++;
   }
printf("gee\n");
	return(NULL);
}

int 
grammar::gen_parsing_tab_entries (parse_tab_entry M[MAX_PARSE_TAB_SIZE][MAX_PARSE_TAB_SIZE]) 
{
   int cnt = 0;

   prod * prod_ptr;
   for (cnt = 0; cnt < no_of_productions; cnt++) {
      prod_ptr = productions[cnt];
      prod_ptr->make_pentry ();
      prod_ptr->fill_in_a_entry (M);
   }
   return (0);
}



void
grammar::create_augmented_grammar()
{
   string tmp_prod_str, tmp_rhs;

   augmented_start = productions[0]->lhs->symbol_name + "DASH";
   tmp_prod_str =
      augmented_start + " : " + productions[0]->lhs->symbol_name;
   augmented_prod_ptr = new prod ((char *) tmp_prod_str.c_str ());
   augmented_prod_ptr->lhs = new symbol ((char *) augmented_start.c_str ());
   augmented_prod_ptr->lhs->type = NON_TERMINAL;
   augmented_prod_ptr->rhs.push_back (productions[0]->lhs);
   augmented_prod_ptr->no_of_rhs = 1;
}

/* prod.cc */
#include <stdio.h>
#include <iostream>
#include "prod.h"


prod::prod (char *prule)
{
   prod_rule += strdup (prule);
}

void
prod::print_prod ()
{
   printf ("prod is = '%s' ", prod_rule.c_str ());
   printf ("no_of_rhs = %d \n", no_of_rhs);
}

int
prod::populate_FIRST ()
{
   int cnt = 0;
   symbol *Y;
   int added = 0;


   for (cnt = 0; cnt < no_of_rhs; cnt++) {
      Y = rhs[cnt];
      if (!does_epsilon_exist (&Y->FIRST)) {
         added = merge (&(lhs->FIRST), &(Y->FIRST),INCLUDE_EPSILON);
         break;
      }
   }
   if (cnt == no_of_rhs) {
      if (!does_epsilon_exist (&lhs->FIRST)) {
         insert_epsilon (&lhs->FIRST);
         added++;
      }
   }
   return (added);
}


/* FOLLOW related */
int
prod::populate_FOLLOW ()
{
   int cnt = 0;
   symbol *A;
   symbol *B;
   symbol_set my_first_set, *b_follow_ptr, *a_follow_ptr;
   int added = 0;


   A = lhs;
   a_follow_ptr = &(A->FOLLOW);

   for (cnt = 0; cnt < no_of_rhs; cnt++) {
      B = rhs[cnt];
      b_follow_ptr = &(B->FOLLOW);

      if (B->type == NON_TERMINAL) {
         compute_FIRST_for_rhs (&my_first_set, cnt);

         /* merge with B-FOLLOW */
         added +=
            merge (b_follow_ptr, &my_first_set, EXCLUDE_EPSILON);

         if (does_epsilon_exist (&my_first_set)) {
            added += merge (b_follow_ptr, a_follow_ptr,INCLUDE_EPSILON);
         }
      }

   }

   if (no_of_rhs > 0) {
      int x;

      B = rhs[no_of_rhs - 1];
      if (B->type == NON_TERMINAL) {
         b_follow_ptr = &(B->FOLLOW);
         x = merge (b_follow_ptr, a_follow_ptr,INCLUDE_EPSILON);
         added += x;
      }

   }
   return (added);
}

void
prod::compute_FIRST_for_rhs (symbol_set * set_ptr, int index)
{
   int cnt = 0;
   symbol *sym_ptr;

   for (cnt = 0; cnt < no_of_rhs; cnt++) {
      if (cnt > index) {
         sym_ptr = rhs[cnt];
         merge (set_ptr, &sym_ptr->FIRST, EXCLUDE_EPSILON);
         if (!does_epsilon_exist (&sym_ptr->FIRST)) {
            return;
         }
      }
   }
   if (!does_epsilon_exist (set_ptr)) {
      insert_epsilon (set_ptr);
   }
}
int
prod::merge (symbol_set * dst, symbol_set * src, int type)
{
   int cnt = 0, added = 0, max_cnt;
   symbol *sym_ptr;

   symbol_set::iterator iter;

   iter = src->begin ();
   max_cnt = src->size ();

   while (cnt < max_cnt) {
      sym_ptr = (*iter);
      if (type == EXCLUDE_EPSILON) {
         if (strcmp (sym_ptr->symbol_name.c_str (), "epsilon") == 0) {
            iter++;
            cnt++;
            continue;
         }
      }
      if (dst->find (sym_ptr) == dst->end ()) {
         dst->insert (sym_ptr);
         added++;
      }
      iter++;
      cnt++;
   }
   return (added);
}

int
prod::does_epsilon_exist (symbol_set * s)
{
   symbol eps ("epsilon");
   symbol *epsilon_sym_ptr = &eps;;
   int result;

   result = is_it_in_the_set (epsilon_sym_ptr, s);
   return (result);
}

int
prod::is_it_in_the_set (symbol * ptr, symbol_set * s1)
{
   if (s1->find (ptr) == s1->end ()) {
      return (0);
   }
   return (1);
}

void
prod::insert_epsilon (symbol_set * s)
{
   symbol *epsilon_sym_ptr;

   epsilon_sym_ptr = new symbol ("epsilon");;
   s->insert (epsilon_sym_ptr);
   return;
}

void
prod::print_symbol_set (symbol_set * s1)
{
   int cnt = 0, max_cnt;
   symbol *sym_ptr;

   symbol_set::iterator iter;

   iter = s1->begin ();
   max_cnt = s1->size ();

   printf ("{");
   while (cnt < max_cnt) {
      sym_ptr = (*iter);
      printf (" %s ", sym_ptr->symbol_name.c_str ());
      iter++;
      cnt++;
   }
   return;
}

void 
prod::fill_in_a_entry (parse_tab_entry M[MAX_PARSE_TAB_SIZE][MAX_PARSE_TAB_SIZE])
{
   symbol_set my_first_set;
   symbol_set::iterator iter;
   int cnt = 0, max_cnt;

   symbol * sym_ptr;
   compute_FIRST_for_rhs (&my_first_set, -1);
   iter = my_first_set.begin ();
   max_cnt = my_first_set.size ();
   while (cnt < max_cnt) {
      sym_ptr = (*iter);
      if (does_epsilon_exist (&my_first_set)) {
         add_everything_in_FOLLOW (M);
         break;
      } else {
         M[lhs->val][sym_ptr->val] = pentry;
         printf ("M[%s][%s]= '%s'\n", lhs->symbol_name.c_str (),
                  sym_ptr->symbol_name.c_str (), pentry.prod_string);
      }
      iter++;
      cnt++;
   }
   return;
}
void 
prod::add_everything_in_FOLLOW (parse_tab_entry M[MAX_PARSE_TAB_SIZE][MAX_PARSE_TAB_SIZE])
{
   symbol_set::iterator iter;
   int cnt = 0, max_cnt;

   symbol * sym_ptr;
   iter = lhs->FOLLOW.begin ();
   max_cnt = lhs->FOLLOW.size ();
   while (cnt < max_cnt) {
      sym_ptr = (*iter);
      M[lhs->val][sym_ptr->val] = pentry;
      printf ("M[%s][%s]= '%s'\n", lhs->symbol_name.c_str (),
               sym_ptr->symbol_name.c_str (), pentry.prod_string);
      iter++;
      cnt++;
   }
}
void 
prod::make_pentry () 
{
   symbol * sym_ptr;
   int cnt = 0;

   pentry.prod_lhs = lhs->val;
   pentry.no_of_elem_in_rhs = rhs.size ();
   strcpy (pentry.prod_string, prod_rule.c_str ());
   for (cnt = 0; cnt < no_of_rhs; cnt++) {
      sym_ptr = rhs[cnt];
      pentry.prod_rhs[cnt] = sym_ptr->val;
   }
}

/* symbol.cc */
#include <stdio.h>
#include "stl.h"
#include "symbol.h"

#define	NON_TERMINAL 1
#define	TERMINAL 0

symbol::symbol (char *sym_name)
{
   symbol_name += sym_name;
   type = TERMINAL;
}
void
symbol::
print_FIRST_FOLLOW_sets()
{
   symbol_set::iterator iter;
   symbol *sym_ptr;

   printf ("%s ", symbol_name.c_str ());
   iter = FIRST.begin ();

   printf (" FIRST={");
   while (iter != FIRST.end ()) {
      sym_ptr = (*iter);
      printf ("%s", sym_ptr->symbol_name.c_str ());
      iter++;
		if(iter != FIRST.end() ){
			printf(",");
		}
   }
   printf ("}");

   /* Added for FOLLOW */
   iter = FOLLOW.begin ();
   printf (" FOLLOW={");
   while (iter != FOLLOW.end ()) {
      sym_ptr = (*iter);
      printf ("%s", sym_ptr->symbol_name.c_str ());
      iter++;
		if(iter != FOLLOW.end() ){
			printf(",");
		}
   }
   printf ("}\n");
}

void
symbol::print_sym ()
{
   symbol_set::iterator iter;
   symbol *sym_ptr;

   printf ("name=%s ", symbol_name.c_str ());
   printf ("val=%03d ", val);
   if (type == TERMINAL) {
      printf ("type=Terminal\n");
      return;
   } else {
      printf ("type=NonTerminal ");
   }
   iter = FIRST.begin ();

   printf (" FIRST={");
   while (iter != FIRST.end ()) {
      sym_ptr = (*iter);
      printf ("%s", sym_ptr->symbol_name.c_str ());
      iter++;
		if(iter != FIRST.end() ){
			printf(",");
		}
   }
   printf ("}");

   /* Added for FOLLOW */
   iter = FOLLOW.begin ();
   printf (" FOLLOW={");
   while (iter != FOLLOW.end ()) {
      sym_ptr = (*iter);
      printf ("%s", sym_ptr->symbol_name.c_str ());
      iter++;
		if(iter != FOLLOW.end() ){
			printf(",");
		}
   }
   printf ("}\n");
}
