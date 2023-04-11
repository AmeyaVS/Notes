void func1();

int g_v1;

void func(int x)
{
	int y,z,r;

	y=x+g_v1;

	/* can read/write 
	   into global */
	func1(); 

	/* NOT cse */
	z=x+g_v1;

	r = y * z;
	return(r);
}
