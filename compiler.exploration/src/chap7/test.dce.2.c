
int func(int a,int b)
{
	int i,j;

	i= a * b; /* dead store */

	if(a > 100 ){
		i=a+b; /* i redefined */
		j=j+i;
	}else{
		i=a-b; /* i redefined */
		j=b*a;
		j = j + a;
	}
	return(j+i);


}
		
