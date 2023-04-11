int v1,v2,v3,v4,v5;

int func(int c)
{
	v3 = v1 + v2 ;

	if( c > 100 ){

		/* 'v1+v2' is available 
		   here */
		v4 = v1 + v2; 

 		/* CSE  Killed */
		v1 = 0;
	}

	/* 'v1+v2' is NOT available 
	   here */
	v5 = v1 + v2 ;
}
