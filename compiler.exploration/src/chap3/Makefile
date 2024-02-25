all:ex1 ex2 ex3 ex4 ex5 ex6 ex7 ex8 ex9 ex9a ex9b ex10 ex10a ex11 ex11a ex12 ex13
LEX=flex
YACC=bison
YFLAGS	= -dy -v
LFLAGS=
CC=gcc
#CFLAGS	+= -g 
CPPFLAGS += -g -Wall
LDFLAGS	= -g

cmt=echo

OBJ1	= c-stmt-gram.o c-stmt-lex1.o
OBJ2	= recur_descent.c c-stmt-lex.o
OBJ3	= ex3.cc table_parse.cc c-stmt-lex.o
OBJ4	= grammar.cc ex4.cc
OBJ5	= grammar.cc table_parse.cc c-stmt-lex.o ex5.cc
OBJ6	= ex6.c c-stmt-bot-up-gram.o c-stmt-lex6.o
OBJ7	= ex7.cc op-prec.cc op-prec-lex.o
OBJ8	= ex8.cc c-stmt-lr-parse.cc c-stmt-lex.o
OBJ9	= grammar.cc slr.cc item.cc ex9.cc

OBJ10	= grammar.cc canonical_lr.cc lr1item.cc item.cc ex10.cc

OBJ11	= grammar.cc canonical_lr.cc lalr.cc lr1item.cc item.cc ex11.cc
OBJ12	= ex12.cc c-stmt-lr-parse.cc c-stmt-lex.o
OBJ13	= c-small-gram.o c-small-lex.o main.c

c-stmt-lex1.o : c-stmt-lex.l
		$(cmt) "Generating the Lexical Analyzer from Lexical Specifications"
		flex -oc-stmt-lex.c c-stmt-lex.l
		$(cmt) "Compiling the Lexical Analyzer"
		gcc $(CFLAGS) -c -DGENERATED_PARSER -DCHAP3_EX1 -o c-stmt-lex1.o c-stmt-lex.c

c-stmt-lex6.o : c-stmt-lex.l
		$(cmt) "Generating the Lexical Analyzer from Lexical Specifications"
		flex -oc-stmt-lex.c c-stmt-lex.l
		$(cmt) "Compiling the Lexical Analyzer"
		gcc $(CFLAGS) -c -DGENERATED_PARSER -DCHAP3_EX6 -o c-stmt-lex6.o c-stmt-lex.c

c-stmt-lex.o : c-stmt-lex.l
		$(cmt) "Generating the Lexical Analyzer from Lexical Specifications"
		flex -oc-stmt-lex.c c-stmt-lex.l
		$(cmt) "Compiling the Lexical Analyzer"
		gcc $(CFLAGS) -c -o c-stmt-lex.o c-stmt-lex.c

op-prec-lex.o : op-prec-lex.l
		$(cmt) "Generating the Lexical Analyzer from Lexical Specifications"
		flex -oop-prec-lex.c op-prec-lex.l
		$(cmt) "Compiling the Lexical Analyzer"
		gcc $(CFLAGS) -c -o op-prec-lex.o op-prec-lex.c

c-small-gram.o : c-small-gram.y
		$(cmt) "Generating the Parser from Grammar Specifications"
		bison -dy -oc-small-gram.c -v c-small-gram.y 
		$(cmt) "Compiling the Parser"
		gcc -g -Wall -DGENERATED_PARSER -c -o c-small-gram.o c-small-gram.c
c-small-lex.o : c-small-lex.l
		$(cmt) "Generating the Lexical Analyzer from Lexical Specifications"
		flex -oc-small-lex.c c-small-lex.l
		$(cmt) "Compiling the Lexical Analyzer"
		gcc $(CFLAGS) -c -o c-small-lex.o c-small-lex.c


c-stmt-gram.o : c-stmt-gram.y
		$(cmt) "Generating the Parser from Grammar Specifications"
		bison -dy -oc-stmt-gram.c -v c-stmt-gram.y 
		$(cmt) "Compiling the Parser"
		gcc -g -Wall -DGENERATED_PARSER -c -o c-stmt-gram.o c-stmt-gram.c

c-stmt-bot-up-gram.o : c-stmt-bot-up-gram.y
		$(cmt) "Generating C File from grammar. -t enables debugging i.e shows shift/reduce actions performed"
		bison -dy -t -oc-stmt-bot-up-gram.c -v c-stmt-bot-up-gram.y 
		$(cmt) "Compiling the Parser"
		gcc -g -Wall -DGENERATED_PARSER -c -o c-stmt-bot-up-gram.o c-stmt-bot-up-gram.c

ex1 :	$(OBJ1)
	$(cmt) "Building ex1 Binary"
	$(CC) $(CFLAGS) $(OBJ1) -o ex1
	$(cmt) "Variant 1"
	./ex1 'count=5;'                        
	$(cmt) "Variant 2"
	./ex1 'count=index;'
	$(cmt) "Variant 3"
	./ex1 'count=3*2;'
	$(cmt) "Variant 4"
	./ex1 'count=count+index;'
	$(cmt) "Variant 5"
	./ex1 'count=count+1;'
	$(cmt) "Variant 6"
	./ex1 'count=count*2+5;'
	$(cmt) "Variant 7"
	./ex1 'count=count*2+index;'
	$(cmt) "Missing Identifier / Constant"
	./ex1 'count=5+;'
	$(cmt) "Missing semicolon"
	./ex1 'count=index'

ex2 :	$(OBJ2)
	$(cmt) "Building ex2 Binary"
	$(CC) $(CPPFLAGS) $(OBJ2) -o ex2
	$(cmt) "Variant 1"
	./ex2 'count=5;'                        
	$(cmt) "Variant 2"
	./ex2 'count=index;'
	$(cmt) "Variant 3"
	./ex2 'count=3*2;'
	$(cmt) "Variant 4"
	./ex2 'count=count+index;'
	$(cmt) "Variant 5"
	./ex2 'count=count+1;'
	$(cmt) "Variant 6"
	./ex2 'count=count*2+5;'
	$(cmt) "Variant 7"
	./ex2 'count=count*2+index;'
	$(cmt) "Missing Identifier / Constant"
	./ex2 'count=5+;'
	$(cmt) "Missing semicolon"
	./ex2 'count=index'


ex3 :	$(OBJ3)
	$(cmt) "Building ex3 Binary"
	g++ $(CPPFLAGS) $(OBJ3) -o ex3
	$(cmt) "Variant 1"
	./ex3 'count=5;'                        
	$(cmt) "Variant 2"
	./ex3 'count=index;'
	$(cmt) "Variant 3"
	./ex3 'count=3*2;'
	$(cmt) "Variant 4"
	./ex3 'count=count+index;'
	$(cmt) "Variant 5"
	./ex3 'count=count+1;'
	$(cmt) "Variant 6"
	./ex3 'count=count*2+5;'
	$(cmt) "Variant 7"
	./ex3 'count=count*2+index;'
	$(cmt) "Missing Identifier / Constant"
	./ex3 'count=5+;'
	$(cmt) "Missing semicolon"
	./ex3 'count=index'


ex4 :	$(OBJ4)
	$(cmt) "Building ex4 Binary"
	g++ $(CPPFLAGS) $(OBJ4) -o ex4
	$(cmt) "A sample grammar file - sample1.gram"
	cat sample1.gram	
	./ex4 'sample1.gram'
	$(cmt) "A sample grammar file - sample2.gram"
	cat sample2.gram	
	./ex4 'sample2.gram'
	$(cmt) "A sample grammar file - sample3.gram"
	cat sample3.gram	
	./ex4 'sample3.gram'


ex5 :	$(OBJ5)
	$(cmt) "Building ex5 Binary"
	g++ $(CPPFLAGS) $(OBJ5) -o ex5
	$(cmt) "Variant 1"
	./ex5 'sample1.gram'  'count=5;'
	$(cmt) "Variant 2"
	./ex5 'sample1.gram'  'count=index;'

ex6 :	$(OBJ6)
	$(cmt) "Building ex6 Binary"
	gcc $(CPPFLAGS) $(OBJ6) -o ex6
	$(cmt) 'An Example for shift-reduce Parsing'
	./ex6 'a=count+2;'

ex7 :	$(OBJ7)
	$(cmt) "Building ex7 Binary"
	g++ $(CPPFLAGS) $(OBJ7) -o ex7
	$(cmt) 'Simple Expression with 2 numbers'
	./ex7 '25 + 35 '
	$(cmt) 'Simple Expression with a number and a variable'
	./ex7 'a + 35'
	$(cmt) 'Expression with multiple Operators'
	./ex7 'a + 35 * 40'
	$(cmt) 'Expression with multiple Operators'
	./ex7 'a ^ 35 - 40 '
	$(cmt) 'Expression with missing Operand'
	./ex7 'a ^ - '
	$(cmt) 'Expression with missing Operator'
	./ex7 'a h'

ex8 :	$(OBJ8)
	$(cmt) "Building ex8 Binary"
	g++ $(CPPFLAGS) -DCHAP3_EX8 $(OBJ8) -o ex8
	$(cmt) "Variant 1"
	./ex8 'count=5;'                        
	$(cmt) "Variant 5"
	./ex8 'count=count+1;'
	$(cmt) "Missing Identifier / Constant"
	./ex8 'count=5+;'


ex9 :	$(OBJ9)
	$(cmt) "Building ex9 Binary"
	g++ $(CPPFLAGS) $(OBJ9) -o ex9
	$(cmt) "A sample grammar file - sample4.gram"
	cat sample4.gram	
	./ex9 'sample4.gram'

#
#	$(cmt) "A sample grammar file - sample5.gram"
#	cat sample5.gram	
#	./ex9 'sample5.gram'
#	$(cmt) "sample6.gram - A Non SLR(1) Grammar"
#	cat sample6.gram	
#	./ex9 'sample6.gram'

ex9a : 
		$(cmt) "A sample grammar file - sample7.gram"
		cat sample7.gram	
		./ex9 'sample7.gram'

ex9b :
		$(cmt) "A sample grammar file - sample7a.gram"
		cat sample7a.gram	
		./ex9 'sample7a.gram'

ex10 :	$(OBJ10)
	$(cmt) "Building ex10 Binary"
	g++ $(CPPFLAGS) $(OBJ10) -o ex10
	$(cmt) "A sample grammar file - sample7a.gram"
	cat sample7a.gram	
	./ex10 'sample7a.gram'

#	$(cmt) "sample6.gram - A Non SLR(1) Grammar"
#	cat sample6.gram	
#	./ex10 'sample6.gram'
#	$(cmt) "A sample grammar file - sample8.gram"
#	cat sample8.gram	
#	./ex10 'sample8.gram'

ex10a :
		$(cmt) "A sample grammar file - sample9.gram"
		cat sample9.gram	
		./ex10 'sample9.gram'


ex11 :	$(OBJ11)
	$(cmt) "Building ex11 Binary"
	g++ $(CPPFLAGS) $(OBJ11) -o ex11
	$(cmt) "A sample grammar file - sample9.gram"
	cat sample9.gram	
	./ex11 'sample9.gram'

#	$(cmt) "A sample grammar file - sample8.gram"
#	cat sample8.gram	
#	./ex11 'sample8.gram'

ex11a :
		$(cmt) "A sample grammar file - sample10.gram"
		cat sample10.gram	
		./ex11 'sample10.gram'

# This example is used in Error reporting and recovery demonstartion
# this can remain hardcoded, so commented it from Makefile
#ex12 :
#		$(cmt) "An input C source Program containing errors"
#		cat -n test1.c
#		gcc test1.c -o test1

ex12 :	$(OBJ12)
	$(cmt) "Building ex12 Binary"
	g++ $(CPPFLAGS) -DCHAP3_EX12 $(OBJ12) -o ex12
	$(cmt) "Variant 7"
	./ex12 'count=count*2+index;'
	$(cmt) "Missing Identifier / Constant"
	./ex12 'count=5+;'
	$(cmt) "Missing semicolon"
	./ex12 'count=index'
	$(cmt) "Missing EQTO Operator"
	./ex12 'count index;'

ex13 :	$(OBJ13)
	$(cmt) "Building the Syntax Analyzer Binary "
	gcc $(CPPFLAGS) $(OBJ13) -o syn_analyzer
	$(cmt) "Sample C Program with correct syntax"
	cat -n test1.c
	$(cmt) "Trying out the Syntax Analyzer on the Sample Program"
	./syn_analyzer test1.c
	$(cmt) "Sample C Program with syntax error"
	cat -n test1a.c
	$(cmt) "Trying out the Syntax Analyzer on the Sample Program"
	./syn_analyzer test1a.c
	$(cmt) "Sample C Program with syntax error"
	cat -n test1b.c
	$(cmt) "Trying out the Syntax Analyzer on the Sample Program"
	./syn_analyzer test1b.c
	$(cmt) "Sample C Program with no syntax error,but semantic error"
	cat -n test1c.c
	$(cmt) "Trying out the Syntax Analyzer on the Sample Program"
	./syn_analyzer test1c.c


listing : c-stmt-gram.y
	mklist c-stmt-gram.y listing3.1.rtf

clean	:
	rm -f y.tab.h 	c-small-gram.c c-small-gram.h c-small-gram.output c-small-lex.c c-stmt-lex.c op-prec-lex.c c-stmt-gram.output c-stmt-gram.c c-stmt-gram.h c-stmt-bot-up-gram.h c-stmt-bot-up-gram.output c-stmt-bot-up-gram.c y.output *.o *~ core c-stmt ex1 ex2 ex3 ex4 ex5 ex6 ex7 ex1 ex2 ex3 ex4 ex5 ex6 ex7 ex8 ex8 ex9 ex9 ex10 ex10 ex11 ex11 ex12 ex12 syn_analyzer

