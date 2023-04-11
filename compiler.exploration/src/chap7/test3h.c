void func1(int *p);

int a,b;

void func(int x,int y)
{
	int *p;

	a=x+y;

	p = &x;

	/* can read/write 
	   into p */
	func1(p);

	/* NOT cse */
	b= x+y;

	return;
}
