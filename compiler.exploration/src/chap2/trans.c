#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#define	MAX_STATES	256
#define	MAX_CHAR	256

int transition_table[MAX_STATES][MAX_CHAR];
int accept[MAX_STATES];
int current_state;

int init_done = 1;

FILE *yyin;

void
yyinit ()
{
  memset (transition_table, 0, sizeof (transition_table));
}

int
yylex ()
{
  int c;

  if (!init_done) {
    yyinit ();
    init_done = 1;
  }
  current_state = 0;
  while (1) {

    if ((c = fgetc (yyin)) == EOF) {
      return (0);
    }

  try_transition:

    if (transition_table[current_state][c]) {
      current_state = transition_table[current_state][c];
    } else {

      /* No Transition available - Retry with start 
         *  state */

      if (current_state != 0) {
        current_state = 0;
        goto try_transition;
      }
    }

    if (accept[current_state]) {
      break;
    }

  }

  /* Reached an accept state */
  switch (current_state) {
  case 3:
    printf ("found 'for' ");
    return (3);
  case 8:
    printf ("found 'while' ");
    return (8);
  default:
    printf ("Unknown accept state\n");
    break;
  }

  return (1);
}

int
main (int argc, char **argv)
{
  FILE *fp;
  int ret_val;

  if (argc != 2) {
    printf ("Usage %s <input File>\n", argv[0]);
    exit (1);
  }

  if ((fp = fopen (argv[1], "r")) == NULL) {
    printf ("File [%s] does not exist \n", argv[1]);
    exit (1);
  }

  /* Making the Transition Table for keyword 'for' */
  transition_table[0]['f'] = 1;
  transition_table[1]['o'] = 2;
  transition_table[2]['r'] = 3;
  accept[3] = 1;

  /* Making the Transition Table for keyword 'while' */
  transition_table[0]['w'] = 4;
  transition_table[4]['h'] = 5;
  transition_table[5]['i'] = 6;
  transition_table[6]['l'] = 7;
  transition_table[7]['e'] = 8;
  accept[8] = 1;

  yyin = fp;                    /* Input file to Lexical Analyzer */

  while (1) {
    if ((ret_val = yylex ()) == 0) {
      break;
    }
    /* return value is 3 - 'for' , 8 - 'while'  */
    printf ("Return Val=%d \n", ret_val);
  }
  return(0);
}
