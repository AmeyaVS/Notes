/* Function */
int func( int v1,int v2)
{
	int v3,v4;

	v3=v1+v2;
	v4=v1-v2;

	/* 
	Undefined variable 'v5' used below in return statement
	It is a Semantic Error.
	However it is NOT a syntax error 
	*/
	return(v5*v4); 
}
