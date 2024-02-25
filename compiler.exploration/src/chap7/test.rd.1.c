int func(int  y)
{
	int x,z;

	x=160;

	if( y > 25 ){

		/* the definition x=160 
		reaches here */
		z=y+x;

	}else{

		/* the definition x=160 
		reaches here */
		z=y-x;
	}

	/* there are two definitions
	of 'z' that reach here.
	z = y+x from the 'if' block
	and z = y - x from the 'else'
	block
	*/
	return(z);
}

