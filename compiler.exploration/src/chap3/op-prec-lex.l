D			[0-9]
L			[a-zA-Z_]

%{
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "op-prec.h"

void count();
int column = 0;

extern int done;
extern char input_str[];

#define	YY_INPUT(buf,result,maxsize) \
	{ \
		int tmp,i=0; \
		if(done){	\
			result=YY_NULL;	\
		}else{	\
			tmp=strlen(input_str); \
			for(i=0;i<tmp;i++){ \
				buf[i]=input_str[i]; \
			}\
			result=tmp; \
		} \
		done=1;	\
	}



%}

%%
{L}({L}|{D})*		{ count(); return(ID); }
{D}+			{ count(); return(ID); }
"("			{ count(); return(LP); }
")"			{ count(); return(RP); }
"+"			{ count(); return(PLUS); }
"-"			{ count(); return(MINUS); }
"/"			{ count(); return(DIV); }
"*"			{ count(); return(STAR); }
"^"			{ count(); return(POW); }
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
/*
	ECHO;
*/
}

int dbg_printf(const char *fmt,...)
{
#ifdef	DEBUG
	va_list ap;
	va_start(ap,fmt);
	vprintf(fmt,ap);
#endif	
	return(1);
}

