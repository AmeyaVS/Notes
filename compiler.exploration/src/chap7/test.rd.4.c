
int func(int y,int a)
{

	int x,z;

	x=y;
	z=45;

	while( y < 100 ){
		z = x + a; /* x cannot be replaced by y */
		y = y + 1;
	}

	return(z);
}


