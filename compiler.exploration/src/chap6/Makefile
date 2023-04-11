all:ex1 ex1a ex1b ex1c ex1d ex1e ex2 ex3 ex4 nested_proc scopes ex5 ex6 ex7 ex8 ex9 ex10


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

UFLAGS=-g

cmt=echo

OBJ1	= 

OBJ10= ic_gen.cc target_code_gen.cc mycc.cc semantic_analysis.cc c-small-gram.o c-small-lex.o

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


# A Simple Assembly Function called from main
ex1 :$(OBJ1) ex1.s
	$(cmt) "Invoking the assembler to create an object file"
	$(cmt) "..and linking it with startup file (crt0.o) to make the executable"
	gcc $(UFLAGS) ex1.s -o ex1
	$(cmt) "Executing it"
	./ex1.exe
	$(cmt) "dummy"

# Calling a C Library Function
ex1a :$(OBJ1) ex1a.s
	$(cmt) "Invoking the assembler to create an object file"
	$(cmt) "..and linking it with startup file (crt0.o) to make the executable"
	gcc $(UFLAGS) ex1a.s -o ex1a
	$(cmt) "Executing it"
	./ex1a.exe

# Fetching the return value of a function
ex1b :$(OBJ1) ex1b.s
	$(cmt) "Invoking the assembler to create an object file"
	$(cmt) "..and linking it with startup file (crt0.o) to make the executable"
	gcc $(UFLAGS) ex1b.s -o ex1b
	$(cmt) "Executing it"
	./ex1b.exe

# Accesing Arguments and returning value from a function
ex1c :$(OBJ1) ex1c.s
	$(cmt) "Invoking the assembler to create an object file"
	$(cmt) "..and linking it with startup file (crt0.o) to make the executable"
	gcc $(UFLAGS) ex1c.s -o ex1c
	$(cmt) "Executing it"
	./ex1c.exe

# Using Local Variables
ex1d :$(OBJ1) ex1d.s
	$(cmt) "Invoking the assembler to create an object file"
	$(cmt) "..and linking it with startup file (crt0.o) to make the executable"
	gcc $(UFLAGS) ex1d.s -o ex1d
	$(cmt) "Executing it"
	./ex1d.exe

# Conditional
ex1e :$(OBJ1) ex1e.s
	$(cmt) "Invoking the assembler to create an object file"
	$(cmt) "..and linking it with startup file (crt0.o) to make the executable"
	gcc $(UFLAGS) ex1e.s -o ex1e
	$(cmt) "Executing it"
	./ex1e.exe

#Example 2 : Activation tree
ex2:ex2.c
	$(cmt) "String Reversal Program in C "
	cat -n ex2.c
	$(cmt) "Compiling it"
	gcc -Wall ex2.c -o ex2
	$(cmt) "Executing it"
	./ex2

#Example 3 : Binding and states of Variables
ex3:ex3.c
	$(cmt) "Program to print the Bindings and states of Variables"
	cat -n ex3.c
	$(cmt) "Compiling it"
	gcc -Wall ex3.c -o ex3
	$(cmt) "Executing it"
	./ex3

#Example 4 : Memory Organization
ex4:ex4.c
	$(cmt) "Program to display addresses of variables"
	$(cmt) "Compiling it"
	gcc -Wall ex4.c -o ex4
	$(cmt) "Executing it"
	./ex4
	$(cmt) "Using the objdump utility to get the addresses"
	objdump -h ex4.exe	

#nested_proc.pas : Program illustrating nested procedures
nested_proc:nested_proc.pas
	$(cmt) "Program illustrating nested procedures"
	gpc -Wall --save-temps nested_proc.pas -o nested_proc
	$(cmt) "The x86 assembly langauge output for nested_proc.pas"
	cat -n nested_proc.s

#Example 5 : Activation Record in C 
ex5:ex5.c
	$(cmt) "Program to print the Activation record in C "
	$(cmt) "Compiling it"
	gcc -Wall ex5.c -o ex5
	$(cmt) "Executing it"
	./ex5

#Example 6 : Calling and return Sequence in C
ex6:ex6.c
	$(cmt) "Program to dislay Calling and return Sequence in C "
	cat -n ex6.c
	$(cmt) "Compiling it for generating the x86 assembly Language code"
	gcc -Wall -c --save-temps ex6.c 
	$(cmt) "Displaying the x86 assembly file"
	cat -n ex6.s

#Example 7 : Call by Value
ex7: ex7.pas
	$(cmt) "Compiling ex7.pas to demonstrate Call-by-Value "
	gpc -Wall --save-temps ex7.pas -o ex7
	$(cmt) "x86 assembly language output"
	cat -n ex7.s
	$(cmt) "Executing it"
	./ex7

#Example 8 : Call by Reference
ex8: ex8.pas
	$(cmt) "Compiling ex8.pas to demonstrate Call-by-Reference "
	gpc -Wall --save-temps ex8.pas -o ex8
	$(cmt) "x86 assembly language output"
	cat -n ex8.s
	$(cmt) "Executing it"
	./ex8

#Example 9 : Call by Value-Result
ex9: ex9.adb
	$(cmt) "Compiling ADA files to demonstrate Call-by-Value-Result "
	gcc -c --save-temps ex9.adb
	gcc -c --save-temps mytst.adb
	$(cmt) "Binding it"
	gnatbind ex9
	$(cmt) "Linking it"
	gnatlink ex9
	$(cmt) "Executing it"
	./ex9
	$(cmt) "x86 assembly language output for ex9.adb"
	cat -n ex9.s
	$(cmt) "x86 assembly language output for myproc.adb"
	cat -n mytst.s

scopes : static.pl dynamic.pl
	$(cmt) "dynamic.pl"
	cat -n dynamic.pl
	$(cmt) "Executing it"
	./dynamic.pl
	$(cmt) "static.pl"
	cat -n static.pl
	$(cmt) "Executing it"
	./static.pl

#Example 10 : Target Code generator
ex10 : $(OBJ10) hello.c test3.c test4.c test5.c test6.c test7.c test8.c
	$(cmt) "Building 'mycc' - A Toy Compiler for C Language"
	g++ $(CPPFLAGS) $(OBJ10) -o mycc.exe
	$(cmt) "Input C source - Hello World Program"
	cat -n hello.c
	$(cmt) "Compiling it with toy compiler(mycc) to generate x86 assembly language output"
	./mycc hello.c > hello.s
	$(cmt) "Observe the interspersed TAC code (in comments )with the assembly language output"
	cat -n hello.s
	$(cmt) "Using gcc in verbose mode to make the executable binary"
	$(cmt) "Observe the Invoking of the assembler(as) to create an object file"
	$(cmt) "..and the linker (collect2) for linking to make the executable"
	gcc  $(UFLAGS) -v  hello.s -o hello.exe
	$(cmt) "Executing the Binary !!"
	./hello.exe
	$(cmt) "mycc has an option -i for merely generating TAC output"
	./mycc -i hello.c
	$(cmt) "Input C file using Local and Global variables"
	cat -n test3.c
	$(cmt) "Compiling it with mycc to generate x86 assembly language output"
	./mycc test3.c >test3.s
	$(cmt) "Using gcc to make the executable binary"
	gcc $(UFLAGS) test3.s -o test3.exe
	$(cmt) "Executing the Binary "
	./test3
	$(cmt) "Input C file having Simple single dimensional array access"
	cat -n test4.c
	$(cmt) "Compiling it with mycc to generate x86 assembly language output"
	./mycc test4.c > test4.s
	$(cmt) "Using gcc to make the executable binary"
	gcc $(UFLAGS) test4.s -o test4.exe
	$(cmt) "Executing the Binary "
	./test4
	$(cmt) "Input C file using Pointer and Address Operator"
	cat -n test5.c
	$(cmt) "Compiling it with mycc to generate x86 assembly language output"
	./mycc test5.c > test5.s
	$(cmt) "Using gcc to make the executable binary"
	gcc $(UFLAGS) test5.s -o test5.exe
	$(cmt) "Executing the Binary "
	./test5
	$(cmt) "Input C file using Structures "
	cat -n test6.c
	$(cmt) "Compiling it with mycc to generate x86 assembly language output"
	./mycc test6.c > test6.s
	$(cmt) "Using gcc to make the executable binary"
	gcc $(UFLAGS) test6.s -o test6.exe
	$(cmt) "Executing the Binary "
	./test6
	$(cmt) "Input C file using if-else statements"
	cat -n test7.c
	$(cmt) "Compiling it with mycc to generate x86 assembly language output"
	./mycc test7.c > test7.s
	$(cmt) "Using gcc to make the executable binary"
	gcc $(UFLAGS) test7.s -o test7.exe
	$(cmt) "Executing the Binary "
	./test7
	$(cmt) "Input C file using while statement"
	cat -n test8.c
	$(cmt) "Compiling it with mycc to generate x86 assembly language output"
	./mycc test8.c > test8.s
	$(cmt) "Using gcc to make the executable binary"
	gcc $(UFLAGS) test8.s -o test8.exe
	$(cmt) "Executing the Binary "
	./test8

listing : ex1.s ex1a.s ex1b.s ex1c.s ex1d.s ex1e.s
	mklist ex1a.s  listing6.3.rtf
	mklist ex1b.s  listing6.4.rtf
	mklist ex1c.s  listing6.5.rtf
	mklist ex1d.s  listing6.6.rtf
	mklist ex1e.s  listing6.7.rtf
	mklist nested.pas  listing6.8.rtf
	mklist blocks.c  listing6.9.rtf
	mklist nested_proc.pas  listing6.10.rtf
	mklist ex7.pas  listing6.11.rtf
	mklist ex8.pas  listing6.12.rtf
	mklist mytst.adb  listing6.13.rtf
	mklist ex9.adb  listing6.16.rtf
	mklist ex10.c  listing6.14.rtf
	mklist params.c  listing6.15.rtf


clean	:
		rm -f c-small-lex*.cc c-small-gram*.cc c-small-gram*.hh
		rm -f *.o hello.exe test*.exe ex*.exe *.output *.stackdump core y.tab.* test9.tac test[3-9].s hello.s ex6.s ex6.i ex7.s ex7.i ex8.s ex8.i ex9.s ex9.i ex9.ali mytst.s mytst.ali nested_proc.exe nested_proc.i nested_proc.s mycc.exe blocks.s blocks.i
		rm -f ex6.s ex7.s ex8.s ex9.s nested_proc.s mytst.s
		rm -f ex6.i ex7.i ex8.i ex9.i nested_proc.i mytst.i
