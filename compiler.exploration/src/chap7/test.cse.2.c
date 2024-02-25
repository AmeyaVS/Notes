int v1,v2,v3,v4,v5,v6,v7;

int func(int c)
{
	v3 = v1 + v2 ;

	if( c > 100 ){

		/* 'v1+v2' is available here as CSE */
		v4 = v1 + v2; 

 		/* CSE  Killed */
		v1 = v3+v4;

		/* computing 'v1+v2' again */
		v7 = v1 + v2;
	}

	/* 'v1+v2' is available here as CSE */
	v5 = v1 + v2 ;
}
