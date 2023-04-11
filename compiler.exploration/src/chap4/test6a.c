#include <stdio.h>

void my_func(char *);

char str1[]="String 1";
char str2[]="String 2";

int main()
{
	char *x;
	char *y;

	x = str1;
	y = str2;

	/* Function my_func() can be called with parameter x or y */
	my_func(x);
	my_func(y);

	/* x and y are type equivalant and can be assigned either way */
	x = y ;
	y = x ;

	return(0);
}

void my_func(char *p)
{
	printf("%s\n",p);
}
