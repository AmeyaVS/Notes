
int func(int a,int b,int c)
{
	int v1,v2,v3;

	v1 = a +b;
	v2 = v1/c;

	return(v2);

	/* Dead Code */
	v3=v1+v2;
	v2=v3+1;
	return(v2);
}
