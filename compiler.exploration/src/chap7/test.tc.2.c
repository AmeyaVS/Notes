/*
This input file tests the TC
generation for the following
IC operators

ADD,MUL,SUB,UMINUS

*/

int printf();

int x,y,z,q;

int main()
{
	int a,b;

	a=3+x;
	b=5+y;

	printf("BEFORE:x=%d y=%d z=%d q=%d \n",x,y,z,q);

	x=a+b;
	y=a-b;
	z=2*x+y;
	q=z-y;
	y=2*z-q;
	q=-y;

	printf("AFTER :x=%d y=%d z=%d q=%d \n",x,y,z,q);

	return(0);
}
