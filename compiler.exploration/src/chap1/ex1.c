
#define SUCCESS 0

/* The function prototype for printf found in stdio.h */
extern int printf(const char *, ...);

int main()
{
	printf("Hello World\n");

	/* returning 0 to the Operating system */
	return(SUCCESS);
}
