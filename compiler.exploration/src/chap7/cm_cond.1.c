int arr[100];


int cap_it(int a)
{
	int i,flag;

	i=0;
	flag=0;

	while(i < 100 ){

		if (arr[i] > a ){
			arr[i]=a;

			/* Loop Invariant */
			flag=1; 
		}

		i=i+1;
	}

	return(flag);

}

