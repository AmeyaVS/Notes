%token IDENTIFIER CONSTANT OPERATOR EQ_TO_OP SEMI_COLON 
%start c_statement
%{
	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>

	extern int yylex();
	int yyerror(char *s);

%}

%%

c_statement  : IDENTIFIER EQ_TO_OP c_expression SEMI_COLON
             ;

c_expression : CONSTANT  
             ;

c_expression :	IDENTIFIER 
             ;

c_expression :	c_expression OPERATOR c_expression
             ;

%%

extern int column;
int yydebug = 0;
char input_str[500];

int
yyerror (char *s)
{
   printf ("%s", input_str);
   fflush (stdout);
   printf ("\n%*s\n%*s\n", column, "^", column, s);
   return (1);
}

int
main (int argc, char **argv)
{
   int ret;

   if (argc != 2) {
      printf ("Usage: %s 'C Statement' \n", argv[0]);
      return (1);
   }

   strcpy (input_str, argv[1]);

   ret = yyparse ();

   if (ret == 0) {
      printf ("%s", input_str);
      printf ("\nSYNTAX CORRECT \n");
   } else {
      printf ("SYNTAX INCORRECT \n");
   }

   return (0);
}
