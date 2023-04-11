#include <stdio.h>
#include <malloc.h>
#include <ctype.h>
#include <iostream>
#include "rlex.h"


/*------------ rlex1.cc begins ---------------*/
rlex::rlex ()
{
  no_of_nfa = 0;
  memset (all_inputs, 0, sizeof (all_inputs));
  memset (all_alpha, 0, sizeof (all_alpha));
}

/* Methods for supporting the Production Rules */

NFA *
rlex::lex_line (char *a_line)
{
  NFA_ptr e1_start=NULL, e1_end=NULL;
  char *tmp;
  int i, j, c;

  inp_str = a_line;

  i = j = 0;
  tmp = a_line;
  while (1) {
    if ((c = tmp[i]) == ' ') {
      break;
    }
		if( c == 0 ){
			break;
		}
    if (isalpha (c)) {
      all_alpha[c] = 1;
    }
    i++;
  }

  j = 0;
  for (i = 0; i < 128; i++) {
    if (all_alpha[i] == 1) {
      all_inputs[j] = i;
      j++;
    }
  }
  all_inputs[j] = 0;
  no_of_inputs = j;

  cur_tok = *inp_str;
  reg_expr (e1_start, e1_end);

  if((e1_start==NULL) || (e1_end==NULL) ){
  	printf("Unable to handle the line [%s] in input lex file \n",inp_str);
	exit(0);
  }

  e1_end->state_type = ACCEPTING_STATE;

  /* 
   * Everything past the regular expression is the accept string 
   * e.g
   * abc?  { printf ("abc found\n"); }
   * The accept string is "  { printf ("abc found\n"); }" 
   */
  e1_end->accept_action = strdup (inp_str);

  return (e1_start);

}

void
rlex::reg_expr (NFA_ptr & startp, NFA_ptr & endp)
{
  NFA_ptr e2_start, e2_end, tmp1;

  single_reg_expr (startp, endp);

  /* Regular expressions are seperated by '|' */
  while (match ('|')) {
    /* regular expression 2 */
    single_reg_expr (e2_start, e2_end);
    startp =
      add_new_NFA_node (0, startp, e2_start, no_of_nfa);
    tmp1 = add_new_NFA_node (0, NULL, NULL, no_of_nfa);
    endp->next = tmp1;
    e2_end->next = tmp1;
    endp = tmp1;
  }
}

void
rlex::single_reg_expr (NFA_ptr & startp, NFA_ptr & endp)
{

  NFA_ptr e2_start, e2_end;

  if (regex_continues (cur_tok)) {
    factor (startp, endp);
  }

  /* the regex continues as long as we have a 'alphabet' or '(' */
  while (regex_continues (cur_tok)) {   /* The next factor */
    factor (e2_start, e2_end);
    endp->next = e2_start;
    endp = e2_end;
  }

}

void
rlex::factor (NFA_ptr & startp, NFA_ptr & endp)
{

  NFA_ptr tmp1, tmp2;

  term (startp, endp);

  if (match ('*')) {            /* term* */
    tmp1 = add_new_NFA_node (0, startp, NULL, no_of_nfa);
    tmp2 = add_new_NFA_node (0, NULL, NULL, no_of_nfa);
    tmp1->next2 = tmp2;
    endp->next = tmp2;
    endp->next2 = startp;
    startp = tmp1;
    endp = tmp2;
  } else if (match ('+')) {     /* term+ */
    tmp1 = add_new_NFA_node (0, startp, NULL, no_of_nfa);
    tmp2 = add_new_NFA_node (0, NULL, NULL, no_of_nfa);
    endp->next = tmp2;
    endp->next2 = startp;
    startp = tmp1;
    endp = tmp2;
  } else if (match ('?')) {     /* term? */
    tmp1 = add_new_NFA_node (0, startp, NULL, no_of_nfa);
    tmp2 = add_new_NFA_node (0, NULL, NULL, no_of_nfa);
    tmp1->next2 = tmp2;
    endp->next = tmp2;
    startp = tmp1;
    endp = tmp2;
  }

}

void
rlex::term (NFA_ptr & startp, NFA_ptr & endp)
{
  if (match ('(')) {            /* dealing with '(regex)' */
    reg_expr (startp, endp);
    if (match (')')) {
    }
  } else if (isalpha (cur_tok)) {   /* alphabet */
    startp =
      add_new_NFA_node (cur_tok, NULL, NULL, no_of_nfa);
    endp = add_new_NFA_node (0, NULL, NULL, no_of_nfa);
    startp->next = endp;

    inp_str++;
    cur_tok = *inp_str;
  }
}

/* match and advance really! */
int
rlex::match (char m)
{
  if (cur_tok == m) {
    inp_str++;
    cur_tok = *inp_str;
    return (1);
  }
  return (0);
}

int
rlex::regex_continues (char tok)
{

  if (isalpha (tok)) {
    return (1);
  }
  if (tok == '(') {
    return (1);
  }
  return (0);
}

NFA *
rlex::add_new_NFA_node (int edg, NFA * nex, NFA * nex2,
                        int stat)
{
  NFA *p;

  nfa_tab[no_of_nfa] = p = new NFA (edg, nex, nex2, stat);
  no_of_nfa++;

  if (no_of_nfa > MAX_NO_OF_NFA) {
    printf ("No of NFA Exceeded Limit [%d]\n",
            MAX_NO_OF_NFA);
    exit (1);
  }

  return (p);
}

void
rlex::convert_re2nfa (char *inp_regex)
{

  NFA *p;
  int i = 0;

  p = add_new_NFA_node (0, NULL, NULL, no_of_nfa);
  p->state_type = START_STATE;

  nfa_start_set.insert (p->state);

  p->next = lex_line (inp_regex);
  for (i = 0; i < no_of_nfa; i++) {
    if (nfa_tab[i]->state_type == ACCEPTING_STATE) {
      nfa_accept_set.insert (i);
    }
  }

}

void
rlex::print_nfa_tree ()
{
  int i = 0, j = 0;
  NFA *ptr;

  /* Title of the Table */
  printf ("\n         NFA Transition Table         \n\n");
  printf ("===========|");
  for (i = 0; i < no_of_inputs; i++) {
    printf ("=====");
  }
  printf ("===========\n");
  printf ("State      | Input Symbol\n");

  /* All the possible inputs */
  printf ("           |");
  for (i = 0; i < no_of_inputs; i++) {
    printf ("  %c  ", all_inputs[i]);
  }
  printf ("epsilon\n");

  printf ("===========|");
  for (i = 0; i < no_of_inputs; i++) {
    printf ("=====");
  }
  printf ("===========");
  printf ("\n");

  for (i = 0; i < no_of_nfa; i++) {
    ptr = nfa_tab[i];

    /* Printing the Type of State */
    if (ptr->state_type == ACCEPTING_STATE) {
      printf ("ACCEP");
    } else if (ptr->state_type == START_STATE) {
      printf ("START");
    } else {
      printf ("     ");
    }

    /* Printing the State number */

    printf ("%3d   |", ptr->state);

    /* Printing the Row */
    for (j = 0; j < no_of_inputs; j++) {
      if ((ptr->edge) && (ptr->edge == all_inputs[j])) {
        printf (" %3d ", ptr->next->state);
      } else {
        printf ("  -  ");
      }
    }

    /* Printing out epsilon transitions */
		if(ptr->state_type == ACCEPTING_STATE ){
      printf (" - ");
			if(strlen(ptr->accept_action) != 0 ){
				printf(" Accept Action is [%s]",ptr->accept_action );
			}
      goto over;
		}
    if ((ptr->edge != 0) && (ptr->next2 == NULL)) {
      printf (" - ");
      goto over;
    } else {
      printf ("{");
    }
    if ((ptr->edge == 0) && (ptr->next != NULL)) {
      printf ("%3d ", ptr->next->state);
    }
    if (ptr->next2 != NULL) {
      printf ("%3d ", ptr->next2->state);
    }
    printf ("}");

  over:
    printf ("\n");

  }
  printf ("===========|");
  for (i = 0; i < no_of_inputs; i++) {
    printf ("=====");
  }
  printf ("===========\n");
}
/*------------ rlex1.cc ends ---------------*/


/*------------ rlex2.cc begins ---------------*/
int
rlex::interpret_nfa (char *str,int verbose)
{
  /* states waiting for input in paralel */
  STATE_SET current_set;
  STATE_SET nfa_start_current_set;

  STATE_SET e_closure_set;
  STATE_SET move_set;

  char *cur_ptr = str;
  char c;
  int acc_state;

  current_set.clear ();
  compute_e_closure_set (&nfa_start_set, &current_set);
  nfa_start_current_set = current_set;

  if(verbose ==1 ){
  	cout << "e_closure(";
  	print_set (&nfa_start_set);
  	cout << ") = ";
  	print_set (&current_set);
  	cout << endl;
  }

  /* Catch all Regular Expressions like 'a*' */
  if (intersect (&nfa_accept_set, &current_set) != -1) {
    return (1);
  }

  while ((c = *cur_ptr) != 0) {

    /* Move Set Computation */
    move_set.clear ();
    move (&current_set, c, &move_set);

  if(verbose ==1 ){
    cout << "move(";
    print_set (&current_set);
    if (c == '\n') {
      cout << "," << "[new line]" << ")= ";
    } else {
      cout << "," << c << ")= ";
    }
    print_set (&move_set);
    cout << endl;
  }

    /* compute epsilon closure set */
    e_closure_set.clear ();
    compute_e_closure_set (&move_set, &e_closure_set);

  if(verbose ==1 ){
    cout << "e_closure(";
    print_set (&move_set);
    cout << ")= ";
    print_set (&e_closure_set);
    cout << endl;
  }

    if (e_closure_set.size ()) {    /* succesful Transition */

      /* Is there an accepting state ? */
      if ((acc_state =
           intersect (&nfa_accept_set,
                      &e_closure_set)) != -1) {
  			if(verbose ==1 ){
        		cout << "Accept State =" << acc_state << endl;
  			}
        	return (1);
      }

      current_set.clear ();
      current_set = e_closure_set;
    } else {
      /* Failure Transition - Retry Transition with start state */

	if (current_set != nfa_start_current_set) {
        	current_set = nfa_start_current_set;
	        continue;
        }

    }

    cur_ptr++;
  }
  return (0);

}

void
rlex::compute_e_closure_set (STATE_SET * move_set,
                             STATE_SET * closure_set)
{
  STATE_SET::iterator my_it, my_end;
  int tmp_state;

  my_it = move_set->begin ();
  my_end = move_set->end ();

  /* For every element in move set */
  while (my_it != my_end) {
    tmp_state = *my_it;
    closure_set->insert (tmp_state);
    e_closure (nfa_tab[tmp_state], closure_set);
    my_it++;
  }
  return;

}

void
rlex::e_closure (NFA * cur, STATE_SET * cset)
{

  if ((cur->edge == EPSILON) && (cur->next != NULL)) {
    cset->insert (cur->next->state);
    e_closure (cur->next, cset);
  }

  if (cur->next2 != NULL) {
    cset->insert (cur->next2->state);
    e_closure (cur->next2, cset);
  }

}

void
rlex::move (STATE_SET * clos_set, char c,
            STATE_SET * move_set)
{

  STATE_SET::iterator my_it, my_end;
  int tmp_state;

  my_it = clos_set->begin ();
  my_end = clos_set->end ();

  /* for every element in clos set */
  while (my_it != my_end) {
    tmp_state = *my_it;

    if (nfa_tab[tmp_state]->edge == c) {
      move_set->insert (nfa_tab[tmp_state]->next->state);
    }
    my_it++;
  }

  return;

}

int
rlex::intersect (STATE_SET * s1, STATE_SET * s2)
{
  int result = -1;
  set_intersection (s1->begin (), s1->end (), s2->begin (),
                    s2->end (), &result);
  return (result);
}


void
rlex::print_set (STATE_SET * s)
{

  cout << "{ ";
  copy (s->begin (), s->end (),
        ostream_iterator < const int >(cout, " "));
  cout << "}";
}
/*------------ rlex2.cc ends ---------------*/

/*------------ rlex3.cc begins ---------------*/
void
rlex::convert_nfa2dfa ()
{
  int i = 0, j = 0;
  unsigned char c;
  STATE_SET clos_set, mv_set;
  int dfa_state_no;

  mv_set.clear ();
  clos_set.clear ();

  compute_e_closure_set (&nfa_start_set, &clos_set);
  add_new_dfa_state (clos_set);
  clos_set.clear ();

  memset (dtran, 0, sizeof (dtran));

  i = 0;

  while (i < no_of_dfa) {
    for (j = 0; j < no_of_inputs; j++) {

      c = all_inputs[j];

      /* move set */
      move (&(dfa_tab[i]->nfa_set), c, &mv_set);

      /* Compute the epsilon closure */
      compute_e_closure_set (&mv_set, &clos_set);

      if (clos_set.size () != 0) {

        /* is there a DFA State which maps to the same NFA Set  */
        dfa_state_no = get_dfa_state (&clos_set);

        if (dfa_state_no == -1) {

          /* None - So add a new one */
          dtran[i][c] = no_of_dfa;
          add_new_dfa_state (clos_set);

        } else {                /* Already exists */
          dtran[i][c] = dfa_state_no;
        }
      }
      clos_set.clear ();
      mv_set.clear ();
    }
    i++;
  }

}

DFA *
rlex::add_new_dfa_state (STATE_SET clos_set)
{
  DFA *tmp;
  int ind;
  STATE_SET e_closure_set;
  STATE_SET nfa_start_e_closure_set;

  compute_e_closure_set (&nfa_start_set, &nfa_start_e_closure_set);

  dfa_tab[no_of_dfa] = tmp = new DFA();

  tmp->nfa_set = clos_set;
  compute_e_closure_set (&tmp->nfa_set, &e_closure_set);

  if ( e_closure_set == nfa_start_e_closure_set) {
    tmp->state_type = START_STATE;
  }

  if ((ind =
       intersect (&e_closure_set, &nfa_accept_set)) != -1) {
    tmp->state_type = ACCEPTING_STATE;
    tmp->accept_action = nfa_tab[ind]->accept_action;
  }

  tmp->state = no_of_dfa;
  no_of_dfa++;

  if (no_of_dfa > MAX_NO_OF_DFA) {
    printf ("No of DFA Exceeded Limit [%d]\n",
            MAX_NO_OF_DFA);
    exit (1);
  }

  return (tmp);
}

void
rlex::print_dfa_tree (int verbose)
{
  int i;

  if(verbose){
	  DFA *ptr;
	  for (i = 0; i < no_of_dfa; i++) {
		 ptr = dfa_tab[i];
		 printf
			("\n DFA State=%d    Corresponding NFA Set :     ",
			 ptr->state);
		 print_set (&(ptr->nfa_set));
	  }
	  printf ("\n");
  }

  /* Title of the Table */
  printf ("\n         DFA Transition Table         \n\n");
  printf ("===========|");
  for (i = 0; i < no_of_inputs; i++) {
    printf ("=====");
  }
  printf ("\n");
  printf ("State      | Input Symbol\n");

  /* All the possible inputs */
  printf ("           |");
  for (i = 0; i < no_of_inputs; i++) {
    printf ("  %c  ", all_inputs[i]);
  }
  printf ("\n");

  print_dfa_tab (dtran, dfa_tab, no_of_dfa);
  printf ("\n");
}

void
rlex::

print_dfa_tab (unsigned int tran[MAX_NO_OF_DFA][MAX_CHAR],
               DFA ** dtab, int no_of_elem)
{
  int i = 0;
  int tmp2;
  int tmp;
  int cur_state;
  DFA *ptr;
  int j;

  printf ("===========|");
  for (i = 0; i < no_of_inputs; i++) {
    printf ("=====");
  }
  printf ("\n");

  for (i = 0; i < no_of_elem; i++) {
    ptr = dtab[i];

    /* Printing the Type of State */
    if (ptr->state_type == ACCEPTING_STATE) {
      printf ("ACCEP");
    } else if (ptr->state_type == START_STATE) {
      printf ("START");
    } else {
      printf ("     ");
    }

    /* Printing the State number */

    printf ("%3d   |", ptr->state);
    cur_state = ptr->state;

    /* Printing the Row */
    for (j = 0; j < no_of_inputs; j++) {
      tmp2 = all_inputs[j];
      if ((tmp = tran[cur_state][tmp2])) {
        printf (" %3d ", tmp);
      } else {
        printf ("  -  ");
      }
    }

    if (ptr->state_type == ACCEPTING_STATE) {
			if(strlen(ptr->accept_action) ){
				printf("Accept Action is [%s] ",ptr->accept_action);
			}
		}

    printf ("\n");

  }
  printf ("===========|");
  for (i = 0; i < no_of_inputs; i++) {
    printf ("=====");
  }
  printf ("\n");

}

int
rlex::get_dfa_state (STATE_SET * cset)
{
  int i = 0;

  for (i = 0; i < no_of_dfa; i++) {
    if (dfa_tab[i]->nfa_set == (*cset)) {
      return (i);
    }
  }
  return (-1);
}
/*------------ rlex3.cc ends ---------------*/

/*------------ rlex4.cc begins ---------------*/
int
rlex::interpret_dfa (char *str)
{

  int res = 0;

  res = interpret_engine (str, dtran, dfa_tab);
  return (res);
}

int
rlex::interpret_engine (char *str,
                        unsigned int
                        tran_tab[MAX_NO_OF_DFA][MAX_CHAR],
                        DFA ** dtab)
{
  char c;
  char cur_state = 0;
  char *cur_ptr = str;

  tran = tran_tab;
  while ((c = *cur_ptr) != 0) {
    cur_state = move (cur_state, c);
    if (dtab[cur_state]->state_type == ACCEPTING_STATE) {
      return (1);
    }
    cur_ptr++;
  }
  return (0);
}

int
rlex::move (int state, char c)
{
  char next_state;

  if (tran[state][c]) {
    next_state = tran[state][c];
  } else {
    if (tran[0][c]) {
      next_state = tran[0][c];
    } else {
      next_state = 0;
    }
  }
  return (next_state);
}
/*------------ rlex4.cc ends ---------------*/

/*------------ rlex5.cc begins ---------------*/
void
rlex::minimise_dfa (int verbose)
{
  STATE_SET groups[MAX_NO_OF_DFA];
  int no_of_groups;
  int i, j;
  STATE_SET::iterator iter;
  STATE_SET new_group;
  STATE_SET *cur_group_ptr;
  unsigned char c;
  int first, next;
  int go_to_first, go_to_next;
  int elem;
  int old_row, new_row;
  unsigned int tran_tab[MAX_NO_OF_DFA];

  memset (min_dtran, 0, sizeof (min_dtran));
  no_of_groups = 1;

  /* Group all the accepting states with same action */
  for (i = 0; i < no_of_dfa; i++) {
    if (dfa_tab[i]->state_type == ACCEPTING_STATE) {
      for (j = 1; j < no_of_groups; j++) {
        iter = groups[j].begin ();
        first = (*iter);
        if (strcmp
            (dfa_tab[first]->accept_action,
             dfa_tab[i]->accept_action) == 0) {
          groups[j].insert (i);
          break;
        }
      }
      if (j == no_of_groups) {
        groups[no_of_groups].insert (i);
        no_of_groups++;
      }
    } else {
      groups[0].insert (i);
    }
  }

  i = 0;

  while (i < no_of_groups) {

    new_group.clear ();

    cur_group_ptr = &groups[i];
    iter = cur_group_ptr->begin ();

    first = (*iter);
    if (iter != cur_group_ptr->end ()) {
      iter++;
    }

    while (iter != cur_group_ptr->end ()) {

      next = (*iter);
      if (iter != cur_group_ptr->end ()) {
        iter++;
      }
      for (j = 0; j < no_of_inputs; j++) {
        c = all_inputs[j];
        go_to_first = dtran[first][c];
        go_to_next = dtran[next][c];

        if (go_to_first != go_to_next) {
          if (!((go_to_first == first)
                && (go_to_next == next))) {
            cur_group_ptr->erase (next);
            new_group.insert (next);
            break;
          }
        }
      }
    }

    if (new_group.size () != 0) {
      groups[no_of_groups] = new_group;
      no_of_groups++;
    }
    i++;

  }

  if(verbose){
  		printf ("No of Old States=%d No of New States=%d \n",
          no_of_dfa, no_of_groups);
	}

  for (i = 0; i < no_of_groups; i++) {
    cur_group_ptr = &groups[i];
    iter = cur_group_ptr->begin ();
    elem = (*iter);
    memcpy (&min_dtran[i][0], &dtran[elem][0],
            (MAX_CHAR * sizeof (int)));
    min_dfa_tab[i] = dfa_tab[elem];
    min_dfa_tab[i]->state = i;
    while (iter != cur_group_ptr->end ()) {
      old_row = (*iter);
      tran_tab[old_row] = i;
      iter++;
    }
  }

  if(verbose){
  		for (i = 0; i < no_of_dfa; i++) {
    		printf ("Old State=%d New State=%d \n", i, tran_tab[i]);
  		}
  }

  for (i = 0; i < no_of_groups; i++) {
    for (j = 0; j < no_of_inputs; j++) {
      c = all_inputs[j];
      old_row = min_dtran[i][c];
      if (old_row != 0) {
        new_row = tran_tab[old_row];
        min_dtran[i][c] = new_row;
      }
    }
  }

  no_of_min_dfa = no_of_groups;

  for (i = 0; i < no_of_min_dfa; i++) {
    if (min_dfa_tab[i]->state_type == ACCEPTING_STATE) {
      yyaccept[i] = 1;
    } else {
      yyaccept[i] = 0;
    }
  }
}

void
rlex::print_min_dfa_tree ()
{
  int i;

  /* Title of the Table */
  printf ("\n      MIN DFA Transition Table         \n\n");
  printf ("===========|");
  for (i = 0; i < no_of_inputs; i++) {
    printf ("=====");
  }
  printf ("\n");
  printf ("State      | Input Symbol\n");

  /* All the possible inputs */
  printf ("           |");
  for (i = 0; i < no_of_inputs; i++) {
    printf ("  %c  ", all_inputs[i]);
  }
  printf ("\n");

  print_dfa_tab (min_dtran, min_dfa_tab, no_of_min_dfa);
  printf ("\n");
}

int
rlex::interpret_min_dfa (char *str)
{
  int res = 0;

  res = interpret_engine (str, min_dtran, min_dfa_tab);

  return (res);
}
/*------------ rlex5.cc ends ---------------*/



void
rlex::machine (FILE * rules_file)
{
  char str[500];

  /* By pass the Header part */
  while (fgets (str, sizeof (str), rules_file) != NULL) {
    if (strstr (str, "%%") != NULL) {
      break;
    }
  }
  handle_lex_rules (rules_file);

  /* rewind to the top of the File */
  rewind (rules_file);

  lex_file = rules_file;

}

void
rlex::gen_code ()
{
  int i, j;
  char str[500];
  string header;
  string body;
  string tail;

  code_gen_template (header, body, tail);

  handle_header (lex_file);
  cout << header << endl;

  printf ("unsigned int yynext[%d][%d]= {\n", no_of_min_dfa,
          MAX_CHAR);
  for (i = 0; i < no_of_min_dfa; i++) {
    printf ("{\n");
    for (j = 0; j < MAX_CHAR; j++) {
      printf ("%d ", min_dtran[i][j]);
      if (j != 255) {
        putchar (',');
      }
    }
    printf ("}\n");
    if (i != (no_of_min_dfa - 1)) {
      putchar (',');
    }

  }
  printf ("};\n");

  printf ("char yyaccept[%d]= {\n", no_of_min_dfa);
  for (i = 0; i < no_of_min_dfa; i++) {
    printf ("%d ", yyaccept[i]);
    if (i != (no_of_min_dfa - 1)) {
      putchar (',');
    }
  }
  printf ("};\n");

  cout << body << endl;

  cout <<
    "/* This is the Code that Changes for each lex file */"
    << endl;
  for (i = 0; i < no_of_min_dfa; i++) {
    if (min_dfa_tab[i]->state_type == ACCEPTING_STATE) {
      printf ("case %d : %s ;break;\n", i,
              min_dfa_tab[i]->accept_action);
    }
  }
  cout << tail << endl;

  /* Skip the Lex rules Part */
  while (fgets (str, sizeof (str), lex_file) != NULL) {
    if (strstr (str, "%%") != NULL) {
      break;
    }
  }

  handle_tail (lex_file);
}

void
rlex::handle_header (FILE * fp)
{
  int output = 0;
  char str[400];

  while (fgets (str, sizeof (str), fp) != NULL) {
    if (strstr (str, "%{") != NULL) {
      output = 1;
      continue;
    } else if (strstr (str, "%}") != NULL) {
      output = 0;
    } else if (strstr (str, "%%") != NULL) {
      break;
    }
    if (output == 1) {
      printf ("%s", str);
    }
  }
}

void
rlex::handle_lex_rules (FILE * rules_file)
{

  NFA *p, *tmp = NULL;
  char str[500];
  int i;

   p =
    add_new_NFA_node (0, NULL, NULL, no_of_nfa);

  p->state_type = START_STATE;
	nfa_start_set.insert(p->state);

  while (fgets (str, sizeof (str), rules_file) != NULL) {
    if (strstr (str, "%%") != NULL) {
      break;
    }
    if (is_it_blank_line (str)) {
      continue;
    }
    p->next = lex_line (str);
    p->next2 = add_new_NFA_node (0, NULL, NULL, no_of_nfa);
    tmp = p;
    p = p->next2;
  }

  if (tmp != NULL) {
    tmp->next2 = NULL;
  }

  for (i = 0; i < no_of_nfa; i++) {
    if (nfa_tab[i]->state_type == ACCEPTING_STATE) {
      nfa_accept_set.insert (i);
    }
  }

}

void
rlex::handle_tail (FILE * fp)
{
  char str[400];

  while (fgets (str, sizeof (str), fp) != NULL) {
    printf ("%s", str);
  }
}

int
rlex::is_it_blank_line (char *str)
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

void
rlex::code_gen_template (string & head, string & body,
                         string & tail)
{

  head = 
"#include <stdio.h> \n"
"static char stack[5000]; \n"
"static int last_accept_state; \n"
"static int yylast_accept_char; \n"
"char yytext[3000]; \n"
"FILE *yyin; \n"
"int my_strcpy (char *s1, char *s2, int len); \n"
"int yylex (); \n"
"int yyleng; \n"
;
  
body = "\n"
"yylex ()\n"
"{\n"
"  char c;\n"
"  int i;\n"
"  int temp;\n"
"  unsigned int cur_state = 0;\n"
"  unsigned int yynext_state;\n"
"  int sptr = 0;\n"
"\n"
"  if (yyin == NULL) {\n"
"    yyin = stdin;\n"
"  }\n"
"\n"
"  while (1) {\n"
"\n"
"    if (last_accept_state) {\n"
"      if (yylast_accept_char == EOF) {\n"
"        return (0);\n"
"      } else {\n"
"        c = yylast_accept_char;\n"
"      }\n"
"      cur_state = 0;\n"
"      last_accept_state = 0;\n"
"      sptr = 0;\n"
"      goto again;\n"
"    }\n"
"\n"
"    while (1) {\n"
"\n"
"      c = fgetc (yyin);\n"
"      if (c == EOF) {\n"
"        if (!last_accept_state) {\n"
"          return (0);\n"
"        }\n"
"        break;\n"
"      }\n"
"\n"
"    again:\n"
"      yynext_state = yynext[cur_state][c];\n"
"\n"
"      if (yynext_state == 0) {\n"
"        if (last_accept_state) {\n"
"          break;\n"
"        }\n"
"\n"
"        if (cur_state) {\n"
"          /* for (i = 0; i < sptr; i++) {\n"
"            putchar (stack[i]);\n"
"          }*/\n"
"          cur_state = 0;\n"
"          sptr = 0;\n"
"          goto again;\n"
"        }\n"
"        /* putchar (c); */\n"
"      } else {\n"
"        cur_state = yynext_state;\n"
"        stack[sptr] = c;\n"
"        sptr++;\n"
"        if (yyaccept[cur_state]) {\n"
"          last_accept_state = cur_state;\n"
"        }\n"
"      }\n"
"    }\n"
"\n"
"    /* if it comes here ..it means that there is a last_accept State */\n"
"\n"
"    my_strcpy (yytext, stack, sptr);\n"
"    yyleng = sptr;\n"
"    yytext[sptr] = 0;\n"
"    yylast_accept_char = c;\n"
"    switch (last_accept_state) {\n"
;
      
tail = "      \n"
"    default:\n"
"      printf (\"Error - Unknown Accepting State. Exiting \");\n"
"      exit (1);\n"
"    }\n"
"  }\n"
"}\n"
"int\n"
"my_strcpy (char *s1, char *s2, int len)\n"
"{\n"
"  int i;\n"
"  for (i = 0; i < len; i++) {\n"
"    *(s1 + i) = *(s2 + i);\n"
"  }\n"
"}\n"
;
    
    
}


/*------------ nfa.cc begins ---------------*/

NFA::NFA (int edg, NFA * nex, NFA * nex2, int stat)
{
  edge = edg;
  next = nex;
  next2 = nex2;
  state = stat;
  state_type = 0;
}
/*------------ nfa.cc ends ---------------*/
