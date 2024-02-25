#include <stdio.h>

/* Two structure definitions with Identical field types and names */
typedef struct complex
{
	double real_part;
	double  imaginary_part;
} complex;

typedef struct complex_again
{
	double real_part;
	double  imaginary_part;
} complex_again;


void my_func(complex v1);

int main()
{
	complex x;
	complex_again y;

	y.real_part =10;
	y.imaginary_part = 20;

	/* x and y are NOT name equivalant !!! */
	x = y ;

	/* 
	my_func called with a variable of wrong type - complex_again
	*/
	my_func(y);

	return(0);
}

void my_func(complex v1)
{
	v1.real_part = 10;
	v1.imaginary_part = 20;
}
