int fun( int x)
{
	int *p;
	int a,b,d;

	a=24;

	if( x> 10 ){
		p = &b;
	}else{
		p = &a;
	}

	/* NOT dead store */
	b = 10;
	   
	d = *p;
	return(d);
}

