
int a,b,c,d;

void fun(int x)
{
	int *p;

	if( x> 10 ){
		p = &b;
	}else{
		p = &a;
	}

	c = a + b;

	/* a or b is overwritten */
	*p = 25;

	d = a + b; /* NOT cse */

}

