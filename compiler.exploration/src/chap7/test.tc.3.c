/*
This input file tests the TC
generation for the following
IC operators

LT,GT,EQ
LBL,GOTO
PARAM,CALL,RETRIEVE


*/
int x,y;
int printf();

int func(int a,int b)
{
   int c,z;

	if( a < b ){
		x=a+b;
		y=x*(a-b);
	} else if( a > b ){
		x=a-b;
		y=x*(a+b);
	} else if ( a == b ){
		x=a*b;
		y=2*x+a;
	}

	z=(x+y);

	return(z);

}

int main()
{
	int ret;

	ret=func(3,5);
	printf("AFTER 1:x=%d y=%d ret=%d \n",x,y,ret);

	ret=func(5,3);
	printf("AFTER 2:x=%d y=%d ret=%d \n",x,y,ret);

	ret=func(5,5);
	printf("AFTER 3:x=%d y=%d ret=%d \n",x,y,ret);

	return(0);

}
