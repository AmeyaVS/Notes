/* 
	Common Subexpression 
	Access of array elements causes quite 
	a number of elemnets which have Common Subexpression
*/

int arr[5];
int printf();
int sort_array(int n);

int sort_array(int n)
{
	int det;

	int g,r,c;
	r=0;
	while(r<(n-1) ){
		g=r+1;
		while ( g<n) {
			if ( arr[r]  < arr[g] ) {
				c=arr[r];
				arr[r] = arr[g];
				arr[g] = c;
			}
			g=g+1;
		}
		r=r+1;
	}
}

int main()
{
	int i;

	arr[0]=6;
	arr[1]=3;
	arr[2]=2;
	arr[3]=8;
	arr[4]=3;

	sort_array(5);

	i=0;
	while(i<5){
		printf("%d ",arr[i]);
		i=i+1;
	}

	printf("\n");
}
