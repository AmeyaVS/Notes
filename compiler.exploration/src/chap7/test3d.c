int a,b,c,d;

int fun ()
{

	int *p;

	c = a + b;
	p = &a ;
	*p = 100;

	/* NOT cse */
	d = a + b;

}
