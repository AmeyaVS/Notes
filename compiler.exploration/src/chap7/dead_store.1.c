int a,b,c,d;
int x,y,z;

void func ()
{ 
	x = a + b; /* Dead Store */
	y = b + c;
	x = d + a;
}

