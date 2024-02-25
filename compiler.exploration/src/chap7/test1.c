int largest(int p,int q, int r)
{
	int tmp;

	if(p > q ){
		tmp=p;
	}else{
		tmp=q;
	}
	if(r > tmp){
		tmp=r;
	}

	return(tmp);
}
