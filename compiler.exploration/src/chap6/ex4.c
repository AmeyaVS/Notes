#include <stdio.h>
#include <stdlib.h>
int g1;
int g2;

int *ptr;
int func1();
int func2();

int func1()
{
	int a,b;

	printf("========== STACK AREA ============\n");
	printf("Address of Local Variable a=%lx\n",(unsigned long)(unsigned long)&a);
	printf("Address of Local Variable b=%lx\n",(unsigned long)(unsigned long)&b);

	func2();

	return(0);
}

int main()
{
	printf("========== CODE AREA ============\n");
	printf("Start address of the main() in code area=%lx \n",(unsigned long)&main);
	printf("Start address of the func1() in code area=%lx \n",(unsigned long)&func1);
	printf("Start address of the func2() in code area=%lx \n",(unsigned long)&func2);

	printf("========== STATIC AREA ============\n");
	printf("Address of the Global Variable g1 in Static area=%lx \n",(unsigned long)&g1);
	printf("Address of the Global Variable g2 in Static area=%lx \n",(unsigned long)&g2);

	func1();

	printf("========== HEAP AREA ============\n");
	ptr = (int *)malloc(3);
	printf("Address allocated on the heap=%lx \n",(unsigned long)ptr);

	ptr = (int *)malloc(3);
	printf("Address allocated on the heap=%lx \n",(unsigned long)ptr);

	ptr = (int *)malloc(3);
	printf("Address allocated on the heap=%lx \n",(unsigned long)ptr);

	return(0);
}

int func2()
{
	int e,f;

	printf("Address of Local Variable e=%lx \n",(unsigned long)&e);
	printf("Address of Local Variable f=%lx \n",(unsigned long)&f);

	return(0);
}
