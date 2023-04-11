#include <stdio.h>

void my_func(char *);

char str1[]="String 1";
char str2[]="String 2";

int main()
{
	char *x;
	float y;

	x = str1;
	y = str2;

	/* Function my_func() is being called with appropriate type */
	my_func(x);

	/* 
	Function my_func() is being called with a float 
	as parameter instead of a pointer to char 
	*/

	my_func(y);

	/* x and y are not type equivalant and cannot be assigned  
	  to each other*/
	x = y ;
	y = x ;

	return(0);
}

void my_func(char *p)
{
	printf("%s\n",p);
}
