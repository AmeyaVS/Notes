#ifndef	PARSE_TAB_H

#define	MAX_RHS 30
#define	MAX_PROD_STRING	151
#define MAX_PARSE_TAB_SIZE	512
typedef struct
{
   int prod_lhs;
   int prod_rhs[MAX_RHS];
   int no_of_elem_in_rhs;
   char prod_string[MAX_PROD_STRING];
}
parse_tab_entry;
#ifdef __cplusplus
extern "C"
{
#endif
/*
extern parse_tab_entry M[MAX_PARSE_TAB_SIZE][MAX_PARSE_TAB_SIZE];
*/
extern int start_symbol;
parse_tab_entry fill_in_tab_entry (int count, ...);
void fill_in_parsing_tab ();
extern int tbl_driven_parse ();
#ifdef __cplusplus
}
#endif

#define	PARSE_TAB_H
#endif
