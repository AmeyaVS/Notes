/* Prototype */
int printf();

int *p;
int x,y;

/* Function */
int main()
{
	int tmp;

	/* Initialize */
	y=10;
	x=25;

	printf("Before : x=%d y=%d \n",x,y);

	/* Move the value of y into x */
	p=&x;
	tmp = *p;
	*p=y;
	p = &y;
	*p = tmp;

	printf("After  : x=%d y=%d \n",x,y);

	return(0);
}
