#include <stdio.h>

typedef struct complex
{
	double real_part;
	double  imaginary_part;
} complex;


void my_func(complex v1);

int main()
{
	complex x;
	complex y;

	y.real_part =10;
	y.imaginary_part = 20;

	/* x and y are name equivalant !!! */
	x = y ;

	/* 
	my_func called with a variable of appropriate type - complex
	*/
	my_func(x);

	return(0);
}

void my_func(complex v1)
{
	v1.real_part = 10;
	v1.imaginary_part = 20;
}
