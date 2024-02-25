/* Prototype */
int printf();

int v1,v2;
int v3,v4;

int main()
{

	v1=9; v4=0; v2=1; v3=1;

	while ( v2 <= v1 )
	{
		v3=v3*v2;
		v2 = v2 + 1;
	}
	v4=v3;

	printf("factorial(%d) = %d \n",v1,v4);

	return(0);

}
