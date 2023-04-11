
/* function prototype */
int printf();

/* Global Variables */
int h;
int a1[20];

/* Function */
int main()
{

	/* Initialization */
	a1[15]=100;
	h=30;

	printf("Before : a1[10]=%d h=%d \n",a1[10],h);

	/* Array accesses */
	a1[10]=h;
	h = a1[15];

	printf("After  : a1[10]=%d h=%d \n",a1[10],h);

	return(0);
}
