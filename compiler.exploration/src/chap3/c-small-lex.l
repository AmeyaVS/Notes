D			[0-9]
L			[a-zA-Z_]

%{
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "c-small-gram.h"

void count();
void update_counts(int c );
int real_const_node(char *s);
int const_node(char *s);
int string_lit_node(char *s);
int id_node(char *s);
void comment();
void mark_eol();
int column = 0;
int line_no=1;
char linebuf[500];
int line_char_cnt;

%}

%%
"/*"       { comment(); }
int        { count();return(INT);}
char       { count();return(CHAR);}
float      { count();return(FLOAT);}
void      { count();return(VOID);}
extern    { count();return(EXTERN);}
"struct"        { count(); return(STRUCT); }
"if"        { count(); return(IF); }
"else"        { count(); return(ELSE); }
"while"        { count(); return(WHILE); }
"switch"        { count(); return(SWITCH); }
"case"        { count(); return(CASE); }
"default"        { count(); return(DEFAULT); }
"return"        { count(); return(RETURN); }
"break"        { count(); return(BREAK); }
{L}({L}|{D})*		{ count(); return(IDENTIFIER); }
{D}+			{ count(); return(CONSTANT); }
{D}+\.{D}+	{ count(); return(CONSTANT); }
\"(\\.|[^\\"])*\"	{ count(); return(STRING_LITERAL); }
"="			{ count();return('='); }
"+"			{ count();return('+'); }
"-"			{ count();return('-'); }
"/"			{ count();return('/'); }
"*"			{ count();return('*'); }
"."			{ count();return('.'); }
"("			{ count();return('('); }
")"			{ count();return(')'); }
":"			{ count();return(':'); }
";"			{ count();return(';'); }
","			{ count();return(','); }
"["			{ count();return('['); }
"]"         { count();return(']'); }
"{"			{ count();return('{'); }
"}"			{ count();return('}'); }

"&&"        { count(); return(AND_OP); }
"||"        { count(); return(OR_OP); }
"<="        { count(); return(LE_OP); }
">="        { count(); return(GE_OP); }
"=="        { count(); return(EQ_OP); }
"!="        { count(); return(NE_OP); }
"->"        { count(); return(PTR_OP); }
"!"         { count(); return('!'); }
"<"         { count(); return('<'); }
">"         { count(); return('>'); }


"&"			{ count();return('&'); }


[ \t\v\n\f]		{ count(); }
.			{ /* Ignore bad char */;return(-1); }
%%
int yywrap()
{
	return(1);
}

void comment()
{
   char c, c1;

loop:
   while ((c = input()) != '*' && c != 0)
   {
		update_counts(c);
   }

   if ((c1 = input()) != '/' && c != 0)
   {
      unput(c1);
      goto loop;
   }

	mark_eol();
}


void count()
{
	int i;

	for (i = 0; yytext[i] != '\0'; i++){
		update_counts(yytext[i]);
	}
	mark_eol();

}

void update_counts(int c )
{
		if (c == '\n'){
			column = 0;
			line_char_cnt=0;
			line_no++;
		}else if (c == '\t')
			column += 8 - (column % 8);
		else{
			column++;
		}
		linebuf[line_char_cnt]=c;
		line_char_cnt++;
}

void mark_eol()
{
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
