int fact(int num)
{
	int i,prod;

	i=1;
	prod=1;

	while(i<=num){
		prod=prod*i;
		i=i+1;
	}

	return(prod);
}
