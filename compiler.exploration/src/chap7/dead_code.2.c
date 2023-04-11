int printf();

int debug;

int func(int a,int b,int c)
{
	int v1;

	debug=0;

	if( debug == 1 )
		printf("a=%d b=%d c=%d \n",a,b,c);

	v1=a+b+c;

	return(v1);
}
