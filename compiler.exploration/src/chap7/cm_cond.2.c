int arr[100];


int func(int a,int b)
{
	int i,flag;

	i=0;

	while( 1 == 1 ){

		flag=0;

		if(arr[i] < a ){
			flag=1;
		}


		if ( arr[i] >= b ){
			break;
		}

		i=i+1;

	}
	return(flag);
}

