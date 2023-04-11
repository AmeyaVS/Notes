D			[0-9]
L			[a-zA-Z_]

%{
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "semantic_analysis.h"
#include "ic_gen.h"
#include "syntax_analysis.h"

#include "c-small-gram.hh"

/* Global */
int line_no=1;

static void count();
static void update_counts(int c );
static int real_const_node(char *s);
static int const_node(char *s);
static int string_lit_node(char *s);
static int id_node(char *s);
static void comment();
static void mark_eol();
static int column = 0;
static char linebuf[500];
static int line_char_cnt;

static int chap_ex=-1;

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
"return"        { count(); return(RET); }
"break"        { count(); return(BREAK); }
{L}({L}|{D})*		{ count(); return(id_node(yytext)); }
{D}+			{ count(); return(const_node(yytext)); }
{D}+\.{D}+	{ count(); return(real_const_node(yytext)); }
\"(\\.|[^\\"])*\"	{ count(); return(string_lit_node(yytext)); }
"="			{ count();return('='); }
"+"			{ count();return('+'); }
"-"			{ count();return('-'); }
"/"			{ count();return('/'); }
"*"			{ count();return('*'); }
"."			{ count();return(DOT); }
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
"!"         { count(); return(NEG); }
"<"         { count(); return(LT_OP); }
">"         { count(); return(GT_OP); }


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
   while ((c = yyinput()) != '*' && c != 0)
   {
		update_counts(c);
   }

   if ((c1 = yyinput()) != '/' && c != 0)
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

int id_node(char *s)
{
	yylval = new IDENTIFIER_node(s);
	return(IDENTIFIER);
}

int string_lit_node(char *s)
{
	yylval = new STRING_LIT_node(s);
	return(STRING_LITERAL);
}
int const_node(char *s)
{
	yylval = new CONSTANT_node(s);
	return(CONSTANT);
}

int real_const_node(char *s)
{
	yylval = new REAL_CONSTANT_node(s);
	return(REAL_CONSTANT);
}

void set_example_flag(int flag )
{

	chap_ex= flag;
}

int get_example_flag()
{

   return(chap_ex);
}


