int *p;
int arr[20];
int a,b;

int func(int i,int j)
{

	/* 
	Using pointer to anchor 
	within an array 
	*/
	p = &arr[5];

	a = arr[i];
	/* 
	assigning into array using 
	pointer
	*/
	p[j]=100;
	b = arr[i];
}
