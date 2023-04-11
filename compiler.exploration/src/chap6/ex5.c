#include <stdio.h>

int g;
int *ptr;
int func1(int a,int b);

int main(int argc, char **argv)
{
	ptr = (int *)&argv;
	ptr -=3;
	func1(0x11111111,0x22222222);
	g=0;
	return(g);
}

int func1(int a,int b)
{
	int local1=0x33333333,local2=0x44444444;

	ptr = &b;

	printf("THE ACTIVATION RECORD OF func1 \n");
	printf("====================================== \n");
	printf("Start of Activation Record [addr=%08lx] \n",(unsigned long)ptr);

	printf("Parameter2      : %08lx [addr=%08lx] \n",(unsigned long)*ptr,
	                                             (unsigned long)ptr); 
	ptr--;

	printf("Parameter1      : %08lx [addr=%08lx] \n",(unsigned long)*ptr,
	                                             (unsigned long)ptr); 
	ptr--;

	printf("Return Address  : %08lx [addr=%08lx] \n",(unsigned long)*ptr,
	                                             (unsigned long)ptr); 
	ptr--;

	printf("Saved BP        : %08lx [addr=%08lx] \n",(unsigned long)*ptr,
	                                             (unsigned long)ptr); 
	ptr--;

	printf("Local Variable1 : %08lx [addr=%08lx] \n",(unsigned long)*ptr,
	                                             (unsigned long)ptr); 
	ptr--;

	printf("Local Variable2 : %08lx [addr=%08lx] \n",(unsigned long)*ptr,
	                                             (unsigned long)ptr); 
	ptr--;

	local1=0;local2=0;
	return(0);
}
