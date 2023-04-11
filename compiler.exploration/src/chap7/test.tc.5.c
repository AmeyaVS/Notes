
/* function prototype */
int printf();

/* Global Variables */
int g_var1,g_var2;

/* Function */
int main()
{
	int l_var1,l_var2;

	/* Initialization */
	g_var1=200;
	g_var2=25;

	l_var1=g_var1*g_var2;
	l_var2 = g_var1+g_var2;

	printf("g_var1=%d g_var2=%d l_var1=%d l_var2=%d\n",
	                  g_var1,g_var2,l_var1,l_var2);

	return(0);
}
