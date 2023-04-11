int *p;
int arr[10];
int x;

/* Function */
int main()
{
	arr[3]=10;

	/* Move 10 into x */
	p=&arr[3];
	x=*p;
}
