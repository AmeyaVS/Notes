#include <stdio.h>

void test();
int my_func(int a,int b);

int x,y,z;

int main()
{
	test();
	return(0);
}

void test()
{
	x=5;
	y=10;
	z=my_func((x+43),(y+20));
	x = z + y;
	printf("Value of z=%d \n",z);

	return;
}

int my_func(int a,int b)
{
	int ret;
	int c=10,d=20;

	c = a *a ;
	d = b* b;
	ret = c +d + 2 *a *b;

	return(ret);
}
