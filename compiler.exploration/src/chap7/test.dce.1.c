int func(int a,int b)
{
	int i,j,k;

	i=45; /* dead code after local opt*/
	j=a+b;

	/* 'i' is replaced by 45 */
	if((a+i) > 100 ){ 
		k=a+j;
	}else{
		k=b+j;
	}
	return(k);
}
		
