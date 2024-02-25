#include <stdio.h>

void func2();
void func1();

int g=0;

int main()
{
	func1();
	func2();
	return(0);
}

void func1()
{
	int i,j,k;

	i=1;
	j=2;
	k=3;

	printf("==============\n");
	printf("Variable 'i' is bound to %lx state=%d \n",(unsigned long)&i,i);
	printf("Variable 'j' is bound to %lx state=%d \n",(unsigned long)&j,j);
	printf("Variable 'k' is bound to %lx state=%d \n",(unsigned long)&k,k);

	printf("Variable 'g' is bound to %lx state=%d \n",(unsigned long)&g,g);
	g++;

}

void func2()
{
	int a,b,c;

	a=10;
	b=20;
	c=30;

	printf("==============\n");
	printf("Variable 'a' is bound to %lx state=%d \n",(unsigned long)&a,a);
	printf("Variable 'b' is bound to %lx state=%d \n",(unsigned long)&b,b);
	printf("Variable 'c' is bound to %lx state=%d \n",(unsigned long)&c,c);

	printf("Variable 'g' is bound to %lx state=%d \n",(unsigned long)&g,g);

	func1();

}
