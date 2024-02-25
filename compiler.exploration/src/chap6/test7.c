/* Prototype */
int printf();

int v1,v2,v3,v4;

int main()
{
	v1=10; v2=20; v3=0; v4=0;

	printf("Before1 : v1=%d v2=%d v3=%d v4=%d \n",v1,v2,v3,v4);
	if(v1 < v2 ){
		v3=10;
	}
	v4=40;
	printf("After1  : v1=%d v2=%d v3=%d v4=%d \n",v1,v2,v3,v4);

	v1=20; v2=10; v3=0; v4=0;
	printf("Before2 : v1=%d v2=%d v3=%d v4=%d \n",v1,v2,v3,v4);
	if(v1 < v2 ){
		v3=10;
	}
	v4=40;
	printf("After2  : v1=%d v2=%d v3=%d v4=%d \n",v1,v2,v3,v4);

	return(0);

}
