all:sem_err ex1 ex2 ex3 ex4 ex5 ex6a ex6b ex7
YACC=bison
YFLAGS	= -d -y -v
#-t for debug
LFLAGS= 
CC=gcc
CPPFLAGS +=  $(MYFLAGS) -g -Wall
LDFLAGS	= -g
LEX=flex

cmt=echo


OBJ1 = c_decl_gram.o  c_decl_lex.o
OBJ2 = ex2.cc top_down.cc top_down_lex.o
OBJ3 = ex3.cc ptree_gram.o ptree_lex.o ptree_method.cc
OBJ4 = ex4.cc pas_decl_gram.o pas_decl_lex.o recur_eval.cc
OBJ5 = ex5.cc semantic_analysis.cc c-small-gram.o c-small-lex.o
OBJ7 = main.cc semantic_analysis.cc c-small-gram.o c-small-lex.o

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

sem_err :
	$(cmt) "A C Program using an undeclared variable"
	cat -n sem_err1.c
	$(cmt) "The Compiler detects it and reports the error"
	gcc -Wall sem_err1.c -o sem_err1
	$(cmt) "A C Program Assigning a float to char pointer"
	cat -n sem_err2.c
	$(cmt) "The Compiler detects it and reports the error"
	gcc -Wall sem_err2.c -o sem_err2
	$(cmt) "A C Program using '->' operator on a float Variable"
	cat -n sem_err3.c
	$(cmt) "The Compiler detects it and reports the error"
	gcc -Wall sem_err3.c -o sem_err3
	$(cmt) "A C Program using break statement in a non-loop context"
	cat -n sem_err4.c
	$(cmt) "The Compiler detects it and reports the error"
	gcc -Wall sem_err4.c -o sem_err4
	$(cmt) "A C Program using a float variable as function"
	cat -n sem_err5.c
	$(cmt) "The Compiler detects it and reports the error"
	gcc -Wall sem_err5.c -o sem_err5
	$(cmt) "A C Program declaring a variable twice in the same scope"
	cat -n sem_err6.c
	$(cmt) "The Compiler detects it and reports the error"
	gcc -Wall sem_err6.c -o sem_err6

ex1 :	$(OBJ1)
	$(cmt) "Building ex1 Binary"
	g++ $(CPPFLAGS) $(OBJ1) -o ex1
	$(cmt) "This is a sample input source file"
	cat -n test1.c
	$(cmt) "Parsing and displaying Symbol table information for the declarations"
	./ex1 test1.c
	$(cmt) "Input source file with Syntax Error"
	cat -n test1a.c
	$(cmt) "Syntax Error is detected"
	./ex1 test1a.c

ex2 :	$(OBJ2)
	$(cmt) "Building ex2 Binary"
	g++ $(CPPFLAGS) $(OBJ2) -o ex2
	$(cmt) "Executing it for a sample Expression"
	./ex2 '9+15-20'
	$(cmt) "Another sample Expression"
	./ex2 '3*21 - (4*5)'
	$(cmt) "Another sample Expression"
	./ex2 '(9*53)/(7-4)'
	$(cmt) "Syntax Error in Expression"
	./ex2 '9*53)/(7-4)'

ex3 :	$(OBJ3)
	$(cmt) "Building ex3 Binary"
	g++ $(CPPFLAGS) $(OBJ3) -o ex3
	$(cmt) "Executing it for a sample Expression"
	./ex3 '9+15-20'
	$(cmt) "Another sample Expression"
	./ex3 '3*21 - (4*5)'
	$(cmt) "Another sample Expression"
	./ex3 '(9*53)/(7-4)'
	$(cmt) "Syntax Error in Expression"
	./ex3 '9*53)/(7-4)'

ex4 :	$(OBJ4)
	$(cmt) "Building ex4 Binary"
	g++ $(CPPFLAGS) $(OBJ4) -o ex4
	$(cmt) "This is a input source file"
	cat test4.pas
	$(cmt) "Symbol Table for it"
	./ex4 test4.pas

ex5 :	$(OBJ5)
	$(cmt) "Building ex5 Binary"
	g++ $(CPPFLAGS) $(OBJ5) -o ex5
	$(cmt) "This is a sample input source file"
	cat -n test5.c
	$(cmt) "Parsing , creating and displaying Symbol table for the declarations"
	./ex5 test5.c
	$(cmt) "Another input source file"
	cat -n test5a.c
	$(cmt) "Parsing , creating and displaying Symbol table for the declarations"
	./ex5 test5a.c



ex6a:
	$(cmt) "C Program using variables following structural equivalence"
	cat -n test6a.c
	$(cmt) "The C Compiler compiles it successfully"
	gcc -Wall test6a.c -o test6a
	$(cmt) "C Program using variables that do not follow structure equivalence "
	cat -n test6b.c
	$(cmt) "The C Compiler rejects it with appropriate error messages"
	gcc -Wall test6b.c -o test6b

ex6b:
	$(cmt) "C Program showing Name equivalence with regard to structures"
	cat -n  test6c.c
	$(cmt) "The C Compiler compiles it successfully"
	gcc -Wall test6c.c -o test6c
	$(cmt) "C Program with assignment between struct variables that are not name equivalent"
	cat -n test6d.c
	$(cmt) "The C Compiler rejects it with appropriate error messages"
	gcc -Wall test6d.c -o test6d

ex7 :$(OBJ7)
	$(cmt) "Building the Semantic Analyzer Binary"
	g++ $(CPPFLAGS) $(OBJ7) -o sem_analyzer
	$(cmt) "A C Program using an undeclared variable"
	cat -n sem_err1.c
	$(cmt) "The Semantic Analyzer detects it and reports the error"
	./sem_analyzer sem_err1.c
	$(cmt) "A C Program Assigning a float to char pointer"
	cat -n sem_err2.c
	$(cmt) "The Semantic Analyzer detects it and reports the error"
	./sem_analyzer sem_err2.c
	$(cmt) "A C Program using '->' operator on a float Variable"
	cat -n sem_err3.c
	$(cmt) "The Semantic Analyzer detects it and reports the error"
	./sem_analyzer sem_err3.c 
	$(cmt) "A C Program using break statement in a non-loop context"
	cat -n sem_err4.c
	$(cmt) "The Semantic Analyzer does NOT detect it and report error"
	./sem_analyzer sem_err4.c 
	$(cmt) "A C Program using a float variable as function"
	cat -n sem_err5.c
	$(cmt) "The Semantic Analyzer detects it and reports the error"
	./sem_analyzer sem_err5.c 
	$(cmt) "A C Program declaring a variable twice in the same scope"
	cat -n sem_err6.c
	$(cmt) "The Semantic Analyzer detects it and reports the error"
	./sem_analyzer sem_err6.c 

clean:
	rm -f *.o *.exe
	rm -f c_decl_gram.hh c_decl_gram.output c_decl_gram.cc c_decl_lex.cc
	rm -f top_down_lex.cc
	rm -f ptree_gram.cc ptree_lex.cc ptree_gram.output ptree_gram.hh
	rm -f pas_decl_gram.cc pas_decl_gram.hh pas_decl_gram.output pas_decl_lex.cc
	rm -f c-small-gram.cc c-small-gram.hh c-small-gram.output c-small-lex.cc


