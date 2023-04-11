D			[0-9]

%{
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "top_down.h"

void count();
int column = 0;

extern char input_str[];
int done = 0;

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

%%
{D}+			{ count(); return(CONSTANT); }
"+"			{ count(); return('+'); }
"-"			{ count(); return('-'); }
"/"			{ count(); return('/'); }
"*"			{ count(); return('*'); }
"("			{ count(); return('('); }
")"			{ count(); return(')'); }
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
