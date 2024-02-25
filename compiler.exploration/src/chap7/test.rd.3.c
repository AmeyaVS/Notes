

int func(int c,int d,int e)
{
	int i,j,x,z;

	x = e;

	if ( x > 100 ) { /* x can be replaced by e */

		i=200;
		j=300;

		if(c > 100 ){
			i=150;
			j=160;
		}

		j=j+100;
	
		if(d == 200 ){
			j=j+400;
			i=i+100;
		}

		j=j+250;
		i=i+200;

		x=x+i+j; /* x on the RHS can be replaced by e */

	}

	return(x); /* x can be replaced by e */
}


