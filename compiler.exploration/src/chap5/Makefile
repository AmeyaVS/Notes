all:ex1 ex2 ex3 ex4 ex5 ex6 ex7 ex8
YACC=bison
YFLAGS	= -d -y -v  -t
#-t for debug
LFLAGS= 
CC=gcc
CPPFLAGS +=  $(MYFLAGS) -DICGEN -g -Wall
LDFLAGS	= -g
LEX=flex

cmt=echo


OBJ1= ic_gen.cc semantic_analysis.cc main.cc c-small-gram.o c-small-lex.o

.l.o:
	$(cmt) "Generating the Lexical Analyzer from Lexical Specifications"
	$(LEX) $(LFLAGS) -o$*.cc $<
	$(cmt) "Compiling the Lexical Analyzer"
	g++ $(CPPFLAGS) -c -o $@  $*.cc

.y.o:
	$(cmt) "Generating the Parser from Grammar Specifications"
	$(YACC) $(YFLAGS) -o$*.cc $<
	$(cmt) "Compiling the Parser"
	g++ $(CPPFLAGS) -c -o $@  $*.cc


#Example 1 : Simple assignment statements
ex1 :$(OBJ1)
	$(cmt) "Building icgen Binary"
	g++ $(CPPFLAGS) $(OBJ1) -o icgen
	$(cmt) "This is a input source file"
	cat -n test1.c
	$(cmt) "Generating Intermediate code for simple assignment statements"
	./icgen test1.c
	$(cmt) "Another input source file"
	cat -n test1a.c
	$(cmt) "Generating IC for assignment statements "
	./icgen test1a.c

#Example 2 : Array Access
ex2 :$(OBJ1)
	$(cmt) "Building icgen Binary"
	g++ $(CPPFLAGS) $(OBJ1) -o icgen
	$(cmt) "This is a input source file"
	cat -n test2.c
	$(cmt) "Generating Intermediate code for array access statements"
	./icgen test2.c
	$(cmt) "Another input source file"
	cat -n test2a.c
	$(cmt) "Generating IC for Array access"
	./icgen test2a.c
	$(cmt) "Another input source file"
	cat -n test2b.c
	$(cmt) "Generating IC for Array access"
	./icgen test2b.c

#Example 3 : Pointer and Address Operator
ex3 :$(OBJ1)
	$(cmt) "Building icgen Binary"
	g++ $(CPPFLAGS) $(OBJ1) -o icgen
	$(cmt) "This is a input source file"
	cat -n test3.c
	$(cmt) "Generating IC for statements with pointer and Address operators"
	./icgen test3.c
	$(cmt) "Input source file"
	cat -n test3a.c
	$(cmt) "Generating IC"
	./icgen test3a.c
	$(cmt) "Input source file"
	cat -n test3b.c
	$(cmt) "Generating IC"
	./icgen test3b.c
	$(cmt) "Input source file"
	cat -n test3c.c
	$(cmt) "Generating IC"
	./icgen test3c.c

#Example 4 : Record Access
ex4 :$(OBJ1)
	$(cmt) "Building icgen Binary"
	g++ $(CPPFLAGS) $(OBJ1) -o icgen
	$(cmt) "This is a input source file"
	cat -n test4.c
	$(cmt) "Generating IC"
	./icgen test4.c
	$(cmt) "Input source file"
	cat -n test4a.c
	$(cmt) "Generating IC"
	./icgen test4a.c
	$(cmt) "Input source file"
	cat -n test4b.c
	$(cmt) "Generating IC"
	./icgen test4b.c

#Example 5 : if-else statement
ex5 :$(OBJ1)
	$(cmt) "Building icgen Binary"
	g++ $(CPPFLAGS) $(OBJ1) -o icgen
	$(cmt) "Input file"
	cat -n test5.c
	$(cmt) "Generating IC"
	./icgen test5.c
	$(cmt) "Input file"
	cat -n test5a.c
	$(cmt) "Generating IC"
	./icgen test5a.c
	$(cmt) "Input source file"
	cat -n test5b.c
	$(cmt) "Generating IC"
	./icgen test5b.c


#Example 6 : while statement
ex6 :$(OBJ1)
	$(cmt) "Building icgen Binary"
	g++ $(CPPFLAGS) $(OBJ1) -o icgen
	$(cmt) "Input file"
	cat -n test6.c
	$(cmt) "Generating IC"
	./icgen test6.c
	$(cmt) "Input file"
	cat -n test6a.c
	$(cmt) "Generating IC"
	./icgen test6a.c

#Example 7 : switch-case statement
ex7 :$(OBJ1)
	$(cmt) "Building icgen Binary"
	g++ $(CPPFLAGS) $(OBJ1) -o icgen
	$(cmt) "Input file"
	cat -n test7.c
	$(cmt) "Generating IC"
	./icgen test7.c
	$(cmt) "Input file"
	cat -n test7a.c
	$(cmt) "Generating IC"
	./icgen test7a.c

#Example 8 : Function call statements
ex8 :$(OBJ1)
	$(cmt) "Building icgen Binary"
	g++ $(CPPFLAGS) $(OBJ1) -o icgen
	$(cmt) "Input file"
	cat -n test8.c
	$(cmt) "Generating IC"
	./icgen test8.c
	$(cmt) "Input file"
	cat -n test8a.c
	$(cmt) "Generating IC"
	./icgen test8a.c

listing:
	
clean	:
		rm -f c-stmt-lex*.cc c-stmt-gram*.hh c-stmt-gram*.cc 
		rm -f c-decl-lex*.cc c-decl-gram*.hh c-decl-gram*.cc 
		rm -f c-decl-gram*.hh c-decl-gram*.cc c-decl-lex*.cc 
		rm -f pas-decl-gram*.hh pas-decl-gram*.cc pas-decl-lex*.cc 
		rm -f c-small-lex*.cc c-small-gram*.cc c-small-gram*.hh
		rm -f *.o *.exe *.output *.stackdump core
