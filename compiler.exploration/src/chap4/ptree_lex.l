D			[0-9]
L			[a-zA-Z_]

%{
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "ptree_method.h"
#include "ptree_gram.hh"

void count();
int column = 0;
int line_no=1;

char input_str[500];
int done = 0;
/*
{L}({L}|{D})*		{ count();yylval=new IDENTIFIER_node(yytext);return(IDENTIFIER); }
*/
#define	YY_INPUT(buf,result,maxsize) \
	{ \
		if(done){	\
			result=0;	\
		}else{	\
			strcpy(buf,input_str); \
			result=strlen(input_str); \
		} \
		done=1;	\
	}


%}
%option nounput

%%
{D}+			{ count();yylval=new CONSTANT_node(yytext);return(CONSTANT); }
"="			{ count(); yylval=new misc_terminal_node(yytext);return('='); }
"+"			{ count(); yylval=new operator_node(yytext);return('+'); }
"-"			{ count(); yylval=new operator_node(yytext);return('-'); }
"/"			{ count(); yylval=new operator_node(yytext);return('/'); }
"*"			{ count(); yylval=new operator_node(yytext);return('*'); }
"("			{ count(); yylval=new misc_terminal_node(yytext);return('('); }
")"			{ count(); yylval=new misc_terminal_node(yytext);return(')'); }
";"			{ count(); yylval=new misc_terminal_node(yytext);return(';'); }
"{"			{ count(); yylval=new misc_terminal_node(yytext);return('{'); }
"}"			{ count(); yylval=new misc_terminal_node(yytext);return('}'); }
[ \t\v\n\f]		{ count(); }
.			{ /* Ignore bad char */;return(-1); }
%%

int yywrap()
{
	return(1);
}

void count()
{
	int i;

	for (i = 0; yytext[i] != '\0'; i++)
		if (yytext[i] == '\n'){
			column = 0;
		}else if (yytext[i] == '\t')
			column += 8 - (column % 8);
		else
			column++;
}


int
yyerror (char *s)
{
   printf ("%s", input_str);
   fflush (stdout);
   printf ("\n%*s\n%*s\n", column, "^", column, s);
   return (1);
}
