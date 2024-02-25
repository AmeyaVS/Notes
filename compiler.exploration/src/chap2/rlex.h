#ifndef    RLEX_H
#include <cstring>
#include <string>
#include <set>
#include <iostream>


using std::set;
using std::string;
using std::cout;
using std::endl;

/*------------ nfa.h begins ---------------*/

typedef set < int >STATE_SET;

#define	EPSILON	0

#define	OTHER_STATE	3
#define	START_STATE	1
#define	ACCEPTING_STATE 2

class NFA
{
public:
  void print_NFA ();            /* Printing the NFA */
    NFA (int edge, NFA * next, NFA * next2, int stat);

  NFA *next;                    /* Next state */
  NFA *next2;                   /* Another next state */
  int state_type;               /* type of state */
  int edge;                     /* Label for edge */
  int state;                    /* state number */
  char *accept_action;          /* C Code for the action */
};

/*------------ nfa.h ends ---------------*/

/*------------ dfa.h begins ---------------*/

class DFA
{
public:
  DFA()
  {
  		state_type=OTHER_STATE;
	};
  int state_type;
  int state;
  STATE_SET nfa_set;
  char *accept_action;
};
/*------------ dfa.h ends ---------------*/


#define	MAX_NO_OF_NFA	1000
#define MAX_NO_OF_DFA	(2 * MAX_NO_OF_NFA )
#define MAX_CHAR	128

typedef NFA *NFA_ptr;

class rlex
{

public:                        /* Methods */
  rlex ();
  NFA *lex_line (char *a_line);
  void convert_re2nfa (char *inp_regex);
  void print_nfa_tree ();

private:                       /* Methods */
  void reg_expr (NFA_ptr & startp, NFA_ptr & endp);
  void single_reg_expr (NFA_ptr & startp, NFA_ptr & endp);
  void factor (NFA_ptr & startp, NFA_ptr & endp);
  void term (NFA_ptr & startp, NFA_ptr & endp);
  int match (char m);
  int regex_continues (char tok);
  NFA *add_new_NFA_node (int edg, NFA * nex, NFA * nex2,
                         int stat);

private:                       /* Data */
  char cur_tok;
  char *inp_str;
  int no_of_nfa;
  NFA *nfa_tab[MAX_NO_OF_NFA];
  STATE_SET nfa_accept_set;
  STATE_SET nfa_start_set;
  char all_alpha[MAX_CHAR];
  char all_inputs[MAX_CHAR];
  int no_of_inputs;

  /* Added for rlex2.cc */

public:                        /* methods */
  int interpret_nfa (char *str,int verbose=0);

private:                       /* methods */
  void print_set (STATE_SET * s);
  void move (STATE_SET * clos_set, char c,
             STATE_SET * move_set);
  void e_closure (NFA * cur, STATE_SET * cset);
  void compute_e_closure_set (STATE_SET * move_set,
                              STATE_SET * closure_set);
  int intersect (STATE_SET * s1, STATE_SET * s2);

  /* Added for rlex3.cc */

  /* Methods */

public:
  void convert_nfa2dfa ();
  void print_dfa_tree (int verbose=0);

private:
    DFA * add_new_dfa_state (STATE_SET cset);
  int get_dfa_state (STATE_SET * cset);
  void print_dfa_tab (unsigned int
                      tran[MAX_NO_OF_DFA][MAX_CHAR],
                      DFA ** dtab, int no_of_elem);

  /* data */

public:

private:
    DFA * dfa_tab[MAX_NO_OF_DFA];
  unsigned int dtran[MAX_NO_OF_DFA][MAX_CHAR];
  int no_of_dfa;

  /* Added for rlex4.cc */

  /* Methods */

public:

  int interpret_dfa (char *str);

private:
  int interpret_engine (char *str,
                        unsigned int
                        tran[MAX_NO_OF_DFA][MAX_CHAR],
                        DFA ** dtab);
  int move (int state, char c);

  /* data */

public:

private:
  unsigned int (*tran)[MAX_CHAR];

  /* Added for rlex5.cc */

  /* Methods */

public:

  void minimise_dfa (int verbose=0);
  void print_min_dfa_tree ();
  int interpret_min_dfa (char *str);

private:

  /* data */

public:

private:
  unsigned int min_dtran[MAX_NO_OF_DFA][MAX_CHAR];
  char yyaccept[MAX_NO_OF_DFA];
  DFA *min_dfa_tab[MAX_NO_OF_DFA];
  int no_of_min_dfa;

  /* Added for rlex6.cc */

  /* Methods */

public:
  void machine (FILE * rules_file);
  void gen_code ();

private:
  void handle_header (FILE * fp);
  void handle_lex_rules (FILE * fp);
  void handle_tail (FILE * fp);
  int is_it_blank_line (char *str);
  void code_gen_template (string & head, string & body,
                          string & tail);

  /* data */

public:

private:
    FILE * lex_file;
};



#define    RLEX_H
#endif
