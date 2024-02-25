/*
sum of 1+2+3+4....n is given by
sum_n= ((n) *(n+1))/2


sum of squares 1*1 + 2*2 + 3*3 + 4*4 ....n*n is 
given by
sum_n2=((n)*(n+1)*(2*n+1))/6

sum of cubes 1*1*1 + 2*2*2 + 3*3*3 + 4*4*4 ....n*n*n is 
given by
sum_n3=(((n)*(n+1))/2)*(((n)*(n+1))/2)


*/

int sum_n,sum_n2,sum_n3;

int sum(int n)
{
	sum_n= ((n) *(n+1))/2;
	sum_n2=((n)*(n+1)*(2*n+1))/6;
	sum_n3=(((n)*(n+1))/2)*(((n)*(n+1))/2);
}
