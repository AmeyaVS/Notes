all: ex1 ex1a ex1b ex1c ex1d ex1e

CFLAGS=$(MYFLAGS)
DEPS1= ex1.c

cmt=echo

ex1: $(DEPS1) 
	$(cmt) " A Sample input C file"
	cat -n ex1.c
	$(cmt) "Creating an executable from sample input file using the GNU C compiler system"
	gcc -Wall ex1.c -o ex1
	$(cmt) "Invoking the executable"
	./ex1

ex1a: $(DEPS1)
	$(cmt) "Compiling with verbose option and preserving the intermediate files"
	gcc --save-temps --verbose  ex1.c -o ex1

ex1b: $(DEPS1)
	$(cmt) "The pre-proceesed output file - ex1.i "
	cat -n ex1.i

ex1c: $(DEPS1)
	$(cmt) "The assembly language output file - ex1.s "
	cat -n ex1.s

ex1d: $(DEPS1)
	$(cmt) "Figuring out the object file format"
	file ex1.o

ex1e: $(DEPS1)
	$(cmt) "The properties of the executable ex1.exe"
	file ex1.exe

clean: 
	rm -f *.o ex1.exe ex1.i ex1.s
