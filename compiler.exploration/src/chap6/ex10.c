void	swap(a,b) 
{ 
	int temp; 
	temp = a; 
	a = b; 
	b = temp ;
}

int x[10];
int i;

int main()
{

	i = 1;
	x[1] = 4;
	x[2] = 5 ;
	x[3] = 7 ;
	x[4] = 9 ;


	swap(i,x[i]);

	return(0);

}
	
