LETTER	[A-Za-z]
WS [ \t\n]

%{
	#define	WORD		1
%}

%%
{LETTER}+ { return (WORD) ; }

{WS}	 { ; /* eat up White Space */ }
.        { ; /* eat up all others like punctuation marks etc */}

%%

#include <stdio.h>
int main(int argc, char **argv)
{
	FILE *fp;
	int ret_val;

	if(argc != 2){
		printf("Usage %s <input File>\n",argv[0]);
		exit(1);
	}

	if((fp = fopen(argv[1],"r")) == NULL ){
		printf("File [%s] does not exist \n",argv[1]);
		exit(1);
	}

	yyin = fp;	/* Input file to Lexical Analyzer */

	while (1) {
		if( (ret_val=yylex()) == 0 ){
			break;
		} 
		

		/* return value signifies token type */	
		if(ret_val == WORD ){
			printf("Lexeme=[%s] \t length=%d ",yytext,yyleng); 
			printf(" \t Token is  WORD \n");
		}
	}
}
