/*
This input file tests the TC
generation for the following
IC operators

ADDR_OF,R_INDEX_ASSIGN,L_INDEX_ASSIGN

*/


int printf();
int arr[35];

int main()
{
	int i;

	printf("BEFORE: The first 5 elements in arr are %d %d %d %d %d\n",
	                arr[0],arr[1],arr[2],arr[3],arr[4]);
	
	i=0;
	while(i<5){
		arr[i]=i;
		i=i+1;
	}

	printf("AFTER : The first 5 elements in arr are %d %d %d %d %d\n",
	                arr[0],arr[1],arr[2],arr[3],arr[4]);

	return(0);
}
