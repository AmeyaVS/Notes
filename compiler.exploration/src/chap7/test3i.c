void func1(int *p);

void func()
{
	int b;
	int *p;

	/* NOT dead store */
	b=45;

	p = &b;

	/* can read from p */
	func1(p);

	return;
}
