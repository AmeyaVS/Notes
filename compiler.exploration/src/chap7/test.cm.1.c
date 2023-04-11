int arr[1000];

int func(int a,int b)
{
	int i;
	int n1,n2;

	i=0;

	n1 = a * b ;
	n2 = a - b ;

	while( arr[i] > (n1*n2) )
	{
		i=i+1;
	}


	return(i);
}

