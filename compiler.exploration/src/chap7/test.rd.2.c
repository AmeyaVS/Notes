
int func(int  m,int n)
{
	int x,r,y;

	/* This is copy propagated */
	x = m ; 

	/* This is NOT copy propagated */
	y = n;

	if(m > 20 ){

		/* The definition y=n
		   is killed */
		y=x+n;
	}

	/* Two definitions of 'y' reach here
	one is y=n and the other is y=x+n
	*/

	/* Only one definition
	   x = m , reaches here */
	r = x + y;

	return(r);
}

