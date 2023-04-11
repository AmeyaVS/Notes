int val;

int func(int num)
{
	int i,val;

	val=1;
	i=num;

	while(i>0)
	{
		val = val * i;
		i = i-1;

		if(val > 10000 ){
			break;
		}
	}

}
