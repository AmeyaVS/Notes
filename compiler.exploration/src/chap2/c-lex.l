D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
FS			(f|F|l|L)
IS			(u|U|l|L)*

%{


enum
{
  IDENTIFIER = 1, CONSTANT, STRING_LITERAL, SIZEOF, PTR_OP, 
  INC_OP, DEC_OP, LEFT_OP, RIGHT_OP, LE_OP, GE_OP, EQ_OP, NE_OP, 
  AND_OP, OR_OP,MUL_ASSIGN, DIV_ASSIGN, MOD_ASSIGN, ADD_ASSIGN, 
  SUB_ASSIGN, LEFT_ASSIGN,RIGHT_ASSIGN, AND_ASSIGN, XOR_ASSIGN, 
  OR_ASSIGN, TYPE_NAME, TYPEDEF,EXTERN, STATIC, AUTO, REGISTER, 
  CHAR, SHORT, INT, LONG, SIGNED, UNSIGNED,FLOAT, DOUBLE, CONST, 
  VOLATILE, VOID, STRUCT, UNION, ENUM, ELLIPSIS,CASE, DEFAULT, 
  IF, ELSE, SWITCH, WHILE, DO, FOR, GOTO, CONTINUE, BREAK,RETURN, 
  DEFINE, SEMI, LC, RC, COMMA, COLON, EQUAL, LP, RP, LB, RB,
  STRUCTOP, AND, NOT, NOR, MINUS, PLUS, STAR, DIVOP, MOD, LT, 
  GT, XOR, OR, QUEST, POUND, 
};
void comment();
#define	preturn(val)  \
   printf ("Lexeme=[%s] \t Length=%d \t Token is %s\n", \
           yytext, yyleng, #val ); return(val);

%}

%%
"/*"			{ comment(); }

%{
	/* Keywords */
%}

"auto"			{  preturn(AUTO); }
"break"			{  preturn(BREAK); }
"case"			{  preturn(CASE); }
"char"			{  preturn(CHAR); }
"const"			{  preturn(CONST); }
"continue"		{  preturn(CONTINUE); }
"default"		{  preturn(DEFAULT); }
"define"		{  preturn(DEFINE); }
"do"			{  preturn(DO); }
"double"		{  preturn(DOUBLE); }
"else"			{  preturn(ELSE); }
"enum"			{  preturn(ENUM); }
"extern"		{  preturn(EXTERN); }
"float"			{  preturn(FLOAT); }
"for"			{  preturn(FOR); }
"goto"			{  preturn(GOTO); }
"if"			{  preturn(IF); }
"int"			{  preturn(INT); }
"long"			{  preturn(LONG); }
"register"		{  preturn(REGISTER); }
"return"		{  preturn(RETURN); }
"short"			{  preturn(SHORT); }
"signed"		{  preturn(SIGNED); }
"sizeof"		{  preturn(SIZEOF); }
"static"		{  preturn(STATIC); }
"struct"		{  preturn(STRUCT); }
"switch"		{  preturn(SWITCH); }
"typedef"		{  preturn(TYPEDEF); }
"union"			{  preturn(UNION); }
"unsigned"		{  preturn(UNSIGNED); }
"void"			{  preturn(VOID); }
"volatile"		{  preturn(VOLATILE); }
"while"			{  preturn(WHILE); }

%{
	/* Identifier */
%}

{L}({L}|{D})*		{  preturn(IDENTIFIER); }

%{
	/* Constants */
%}

0[xX]{H}+{IS}?		{ preturn(CONSTANT); }
0{D}+{IS}?		{ preturn(CONSTANT); }
{D}+{IS}?		{ preturn(CONSTANT); }
L?'(\\.|[^\\'])+'	{ preturn(CONSTANT); }
{D}+{E}{FS}?		{ preturn(CONSTANT); }
{D}*"."{D}+({E})?{FS}?	{ preturn(CONSTANT); }
{D}+"."{D}*({E})?{FS}?	{ preturn(CONSTANT); }

%{
	/* String Literal */
%}

L?\"(\\.|[^\\"])*\"	{  preturn(STRING_LITERAL); }

%{
	/* Operators */
%}

"..."			{  preturn(ELLIPSIS); }
">>="			{  preturn(RIGHT_ASSIGN); }
"<<="			{  preturn(LEFT_ASSIGN); }
"+="			{  preturn(ADD_ASSIGN); }
"-="			{  preturn(SUB_ASSIGN); }
"*="			{  preturn(MUL_ASSIGN); }
"/="			{  preturn(DIV_ASSIGN); }
"%="			{  preturn(MOD_ASSIGN); }
"&="			{  preturn(AND_ASSIGN); }
"^="			{  preturn(XOR_ASSIGN); }
"|="			{  preturn(OR_ASSIGN); }
">>"			{  preturn(RIGHT_OP); }
"<<"			{  preturn(LEFT_OP); }
"++"			{  preturn(INC_OP); }
"--"			{  preturn(DEC_OP); }
"->"			{  preturn(PTR_OP); }
"&&"			{  preturn(AND_OP); }
"||"			{  preturn(OR_OP); }
"<="			{  preturn(LE_OP); }
">="			{  preturn(GE_OP); }
"=="			{  preturn(EQ_OP); }
"!="			{  preturn(NE_OP); }
";"			{  preturn(SEMI); }
("{"|"<%")		{  preturn(LC); }
("}"|"%>")		{  preturn(RC); }
","			{  preturn(COMMA); }
":"			{  preturn(COLON); }
"="			{  preturn(EQUAL); }
"("			{  preturn(LP); }
")"			{  preturn(RP); }
("["|"<:")		{  preturn(LB); }
("]"|":>")		{  preturn(RB); }
"."			{  preturn(STRUCTOP); }
"&"			{  preturn(AND); }
"!"			{  preturn(NOT); }
"~"			{  preturn(NOR); }
"-"			{  preturn(MINUS); }
"+"			{  preturn(PLUS); }
"*"			{  preturn(STAR); }
"/"			{  preturn(DIVOP); }
"%"			{  preturn(MOD); }
"<"			{  preturn(LT); }
">"			{  preturn(GT); }
"^"			{  preturn(XOR); }
"|"			{  preturn(OR); }
"?"			{  preturn(QUEST); }
"#"			{  preturn(POUND); }

[ \t\v\n\f]		{  }
.			{ /* ignore bad characters */ }

%%


void comment()
{
	int c=0, prev=0;

	/* eat up all characters till end of comment */
loop:
	prev= c;
	c = input();
	if( (prev == '*') && ( c== '/' )){
		return;
	}
	if( c == -1 ){
		exit(1);
	}
	goto loop;
}

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
		/* return value is token type like BREAK, IDENTIFIER*/	
	}
}
