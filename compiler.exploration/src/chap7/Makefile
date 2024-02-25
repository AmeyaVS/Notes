all:ex1 ex2 ex3 ex4 ex5 ex6 ex7

YACC=bison
YFLAGS	= -d -y -v  -t
#-t for debug
LFLAGS= 
CC=gcc
CPPFLAGS +=$(MYFLAGS) -DICGEN -g -Wall
CFLAGS +=$(MYFLAGS) -DICGEN -g -Wall
ASFLAGS=-gstabs
LDFLAGS	= -g
LEX=flex

UFLAGS=-g --save-temps -v

cmt=echo


#MYCC_OBJ=mycc.cc sym_tab.cc type_expr.cc c-small-gram.o c-small-lex.o quad.cc backpatch.cc x86_code_gen.cc x86_as_quad.cc

MYCC_OBJ= ic_gen.o optimize.o target_code_gen.o mycc.o semantic_analysis.o c-small-gram.o c-small-lex.o

MYCC_SRC= ic_gen.cc optimize.cc target_code_gen.cc mycc.cc semantic_analysis.cc c-small-gram.o c-small-lex.o

OBJ4= ic_gen.o optimize.o target_code_gen.o mycc.o semantic_analysis.o c-small-gram.o c-small-lex.o

.cc.o:
	g++ $(CPPFLAGS) -c -o $@  $<

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

# Scope for Optimization
scope_for_opt :$(MYCC_OBJ) sample.c
	$(cmt) "Building 'mycc' - A Toy Compiler for C Language"
	g++ $(CPPFLAGS) $(MYCC_OBJ) -o mycc.exe
	$(cmt) "Sample Input C file - sample.c "
	cat -n sample.c
	$(cmt) "Generating the Intermediate code (TAC) output with -i option for mycc"
	./mycc -i sample.c >sample.tac
	$(cmt) "Listing the TAC Code"
	cat sample.tac
	$(cmt) "Compiling it with mycc to generate x86 assembly language output"
	./mycc sample.c >sample.s
	$(cmt) "Listing the assembly language output.Note TAC code in comments"
	cat -n sample.s

#Example 1 : Breaking IC to basic blocks
ex1 :$(MYCC_SRC) test1.c test1a.c
	$(cmt) "Building 'mycc' - A Toy Compiler for C Language"
	g++ -DCHAP7_EX1 $(CPPFLAGS) $(MYCC_SRC) -o mycc.exe
	$(cmt) "An input source"
	cat -n test1.c
	$(cmt) "Partitioning the IC into Basic Blocks"
	./mycc -i -O local -v test1.c
	$(cmt) "Another input source"
	cat -n test1a.c
	$(cmt) "Basic Blocks for the input source"
	./mycc -i -O local -v test1a.c
	$(cmt) "dummy"

#Example 2 : Constructing DAG and generating Optimized IC
ex2 :$(MYCC_SRC) test2a.c test2b.c test2c.c test2d.c test2e.c
	$(cmt) "Building 'mycc' - A Toy Compiler for C Language"
	g++ -DCHAP7_EX2 $(CPPFLAGS) $(MYCC_SRC) -o mycc.exe
	$(cmt) "Common Sub-Expression Elimination Transformation"
	cat -n test2a.c
	./mycc -i -O local -v test2a.c
	$(cmt) "Copy Propagation Transform "
	cat -n test2b.c
	./mycc -i -O local -v test2b.c
	$(cmt) "Constant Folding Transformation"
	cat -n test2c.c
	./mycc -i -O local -v test2c.c
	$(cmt) "Dead Assignment Elimination Transformation"
	cat -n test2d.c
	./mycc -i -O local -v test2d.c
	$(cmt) "All of the transformations at Work"
	cat -n test2e.c
	./mycc -i -O local -v test2e.c
	$(cmt) "dummy"

#Example 2a : Showing the defeciencies in Array Handling
ex2a :$(MYCC_SRC) test3a.c
	$(cmt) "Sample Input C file containing array references"
	cat -n test3a.c
	$(cmt) "Intermediate code before and after optimization"
	./mycc.exe -i -O local -v test3a.c 

#Example 2b : Showing the defeciencies in Pointer Handling
ex2b :$(MYCC_SRC) test3d.c test3f.c
	$(cmt) "Sample Input C file with a write via pointer.This affects CSE"
	cat -n test3d.c
	$(cmt) "Intermediate code before and after optimization"
	./mycc.exe -i  -O local -v test3d.c 
	$(cmt) "Sample Input C file Using pointer to write into variable. "
	$(cmt) "Ambigious pointer.  This Affects CSE"
	cat -n test3f.c
	$(cmt) "Intermediate code before and after optimization"
	./mycc.exe -i  -O local -v test3f.c 

#Example 2c : Showing the defeciencies in Handling procedure calls
ex2c :$(MYCC_SRC) test3h.c
	$(cmt) "Sample Input C file with a procedure call passing a pointer. "
	$(cmt) "The called procedure can write using the pointer.This Affects CSE."
	cat -n test3h.c
	$(cmt) "Intermediate code before and after optimization"
	./mycc.exe -i  -O local -v test3h.c 

#Example 2d : Showing the defeciencies in Overwriting input Nodes
ex2d :$(MYCC_SRC) test3j.c test3k.c
	$(cmt) "Sample Input C file with Overwritten input nodes"
	cat -n test3j.c
	$(cmt) "Intermediate code before and after optimization"
	./mycc.exe -i  -O local -v test3j.c 

#Example 3 : Fixing Array,Pointer,Procedure Handling Issues
ex3 :$(MYCC_SRC) test3a.c test3d.c
	$(cmt) "Building 'mycc' - A Toy Compiler for C Language"
	g++ $(CPPFLAGS) $(MYCC_SRC) -o mycc.exe
	$(cmt) "Sample Input C file containing array references"
	cat -n test3a.c
	$(cmt) "Intermediate code before and after optimization"
	./mycc.exe -i -O local -v test3a.c 
	\
	$(cmt) "Sample Input C file with a write via pointer.Affects CSE"
	cat -n test3d.c
	$(cmt) "Intermediate code before and after optimization"
	./mycc.exe -i  -O local -v test3d.c 
	\
	$(cmt) "Sample Input C file Using pointer to write into variable. "
	$(cmt) "Ambigious pointer.  Affects CSE"
	cat -n test3f.c
	$(cmt) "Intermediate code before and after optimization"
	./mycc.exe -i  -O local -v test3f.c 
	\
	$(cmt) "Sample Input C file with a procedure call passing a pointer. "
	$(cmt) "The called procedure can write using the pointer.This Affects CSE."
	cat -n test3h.c
	$(cmt) "Intermediate code before and after optimization"
	./mycc.exe -i  -O local -v test3h.c 

#Example 4 : Global Optimizations - Available Expression
ex4 :$(MYCC_SRC)
	$(cmt) "Building 'mycc' - A Toy Compiler for C Language"
	g++ $(CPPFLAGS) $(MYCC_SRC) -o mycc.exe
	$(cmt) "Sample Input C file containing common subexpressions"
	cat -n test.cse.1.c
	$(cmt) "Intermediate code before and after optimization"
	$(cmt) "-O gcse for Global Common Subexpression, -v for verbosity"
	./mycc.exe -i -O gcse -v test.cse.1.c
	$(cmt) "Sample Input C file containing common subexpressions"
	cat -n test.cse.2.c
	$(cmt) "Intermediate code before and after optimization"
	./mycc.exe -i -O gcse -v test.cse.2.c
	$(cmt) "Sample Input C file containing common subexpressions"
	cat -n test.cse.3.c
	$(cmt) "Intermediate code before and after optimization"
	./mycc.exe -i -O gcse -v test.cse.3.c
	$(cmt) "dummy"

#Example 5 : Global Optimizations - Live Variable Analysis
ex5 :$(MYCC_SRC)
	$(cmt) "Building 'mycc' - A Toy Compiler for C Language"
	g++ $(CPPFLAGS) $(MYCC_SRC) -o mycc.exe
	$(cmt) "Sample Input C file containing dead code that can be eliminated"
	cat -n test.dce.1.c
	$(cmt) "Intermediate code before and after optimization"
	$(cmt) "-O gdce for Global Dead Code Elimination, -v for verbosity"
	./mycc.exe -i -O gdce -v test.dce.1.c
	$(cmt) "Sample Input C file containing dead code that can be eliminated"
	cat -n test.dce.2.c
	$(cmt) "Intermediate code before and after optimization"
	./mycc.exe -i -O gdce -v test.dce.2.c

#Example 6 : Global Optimizations - Reaching Definition Analysis
ex6 :$(MYCC_SRC)
	$(cmt) "Building 'mycc' - A Toy Compiler for C Language"
	g++ $(CPPFLAGS) $(MYCC_SRC) -o mycc.exe
	$(cmt) "Sample Input C file with Loop invariants that can be moved out"
	cat -n test.cm.1.c
	$(cmt) "Intermediate code before and after optimization"
	$(cmt) "-O gcm for Global Code Motion, -v for verbosity"
	./mycc.exe -i -O gcm -v test.cm.1.c
	$(cmt) "Sample Input C file with Loop invariants that can be moved out"
	cat -n test.cm.2.c
	$(cmt) "Intermediate code before and after optimization"
	./mycc.exe -i -O gcm -v test.cm.2.c

#Example 7 : Target Code Optimization
ex7 :$(MYCC_SRC)
	$(cmt) "Building 'mycc' - A Toy Compiler for C Language"
	g++ $(CPPFLAGS) $(MYCC_SRC) -o mycc.exe
	$(cmt) "Sample Input C file "
	cat -n test.tc.1.c
	$(cmt) "Generating Target code with optimization"
	$(cmt) "-t for Target Code Optimization"
	./mycc.exe -t -O all test.tc.1.c
	\
	$(cmt) "Generate Target code with optimization"
	./mycc.exe -O all -t test.tc.2.c >test.tc.2.s
	$(cmt) "Assemble it"
	gcc test.tc.2.s -o test.tc.2.exe
	$(cmt) "Execute it"
	./test.tc.2.exe
	\
	$(cmt) "Generate Target code with optimization"
	./mycc.exe -O all -t test.tc.3.c >test.tc.3.s
	$(cmt) "Assemble it"
	gcc test.tc.3.s -o test.tc.3.exe
	$(cmt) "Execute it"
	./test.tc.3.exe
	\
	$(cmt) "Generate Target code with optimization"
	./mycc.exe -O all -t test.tc.4.c >test.tc.4.s
	$(cmt) "Assemble it"
	gcc test.tc.4.s -o test.tc.4.exe
	$(cmt) "Execute it"
	./test.tc.4.exe
	\
	$(cmt) "Generate Target code with optimization"
	./mycc.exe -O all -t test.tc.5.c >test.tc.5.s
	$(cmt) "Assemble it"
	gcc test.tc.5.s -o test.tc.5.exe
	$(cmt) "Execute it"
	./test.tc.5.exe

listing : 


clean	:
		rm -f c-small-lex*.cc c-small-gram*.cc c-small-gram*.hh
		rm -f *.o *.output *.stackdump core 
		rm -f mycc.exe test1.exe  test1.s sample.tac sample.s 
		rm -f test.tc.[2-5].s
		rm -f mycc.exe
