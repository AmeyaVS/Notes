#include <stdio.h>

int max(int f1,int f2);
int main()
{
	int p1,p2;
	int out;

	p1 = 1;
	p2 = 2;

	out = max(p1,p2);
	return(0);
}

int max(int f1,int f2)
{
	int tmp;

	if(f1 > f2){
		tmp=f1;
	}else{
		tmp=f2;
	}

	return(tmp);
}
