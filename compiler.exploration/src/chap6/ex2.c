#include <stdio.h>
#include <string.h>

char str[100]="Compiler";

void string_reverse(char *a,int b);
void swap(int a,int b);
void print(char c1,char c2);

int main()
{
	int len;

	len=strlen(str);
	printf("%s\n",str);
	string_reverse(str,len);	
	printf("%s\n",str);
	return(0);
}

void string_reverse(char *c,int len)
{
	int i,mid,l1;

	l1=(len-1);
	mid=l1/2;

	for(i=0;i<=mid;i++){
		swap(i,(l1-i));
	}
}

void swap(int a,int b)
{
	char tmp,c1,c2;

	c1=str[a];
	c2=str[b];

	print(c1,c2);

	tmp=str[a];
	str[a]=str[b];
	str[b]=tmp;
}

void print(char c1,char c2)
{
	printf("Swapping %c with %c\n",c1,c2);
}
	
