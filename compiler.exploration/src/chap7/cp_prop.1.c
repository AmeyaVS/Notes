
/*
	Copy Propagation : Explicitly created 

	In this example, the value of 'd' is
	propagated further as follows

	if(a > 10 ){
		e = a + b; ( using 'a' instead of 'd')
	}else{
		e = a + c; ( using 'a' instead of 'd')
	}

	f = a*e  ; (using 'a' instead of 'd')

	This allows us to avoid the assignment statement

	d = a;

*/

int func(int a,int b,int c)
{
	int d,e,f;

	d = a;


	if(a > 10 ){
		e = d + b;
	}else{
		e = d + c;
	}

	f = d*e  ;

	return(f);
}
