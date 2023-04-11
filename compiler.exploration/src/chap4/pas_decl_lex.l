D			[0-9]
L			[a-zA-Z_]

%{
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "recur_eval.h"
#include "pas_decl_gram.hh"

void count();
int column = 0;
int line_no=1;
char linebuf[500];

extern int done;
%}
%option nounput

%%
				
integer        { count();return(INTEGER);}
char       { count();;return(CHAR);}
real      { count();;return(REAL);}

{L}({L}|{D})*     { count();yylval=new IDENTIFIER_node(yytext);return(IDENTIFIER); }

","			{ count();;return(','); }
";"			{ count();return(';'); }
":"			{ count();return(':'); }
[ \t\v\n\f]		{ count(); }
.			{ /* Ignore bad char */;return(-1); }
%%
int yywrap()
{
	return(1);
}

int line_char_cnt;
void count()
{
	int i;

	for (i = 0; yytext[i] != '\0'; i++){
		if (yytext[i] == '\n'){
			column = 0;
			line_char_cnt=0;
			line_no++;
		}else if (yytext[i] == '\t')
			column += 8 - (column % 8);
		else{
			column++;
		}
		linebuf[line_char_cnt]=yytext[i];
		line_char_cnt++;
	}
	linebuf[line_char_cnt]=0;

}


int
yyerror (char *s)
{
   printf ("%s", linebuf);
   fflush (stdout);
   printf ("\n%*s\n", column, "^" );
	printf("Error in Line number=%d \n",line_no);
   return (1);
}

