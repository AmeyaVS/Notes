int v1,v2,v3,v4,v5,v6;

int func(int c)
{
	if( c > 100 ){
		v3 = v1 + v2; 
	}else{
		v4 = v1 + v2 ;
	}

	/* 'v1+v2' is available here as CSE */
	v5 = v1 + v2 ;
}
