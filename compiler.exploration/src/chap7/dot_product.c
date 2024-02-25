/* 
	Constant Folding 
	Access of array elements causes quite 
	a number of elemnets which need constant folding
*/

int arr1[20];
int arr2[20];


int dot_product(int a[20],int b[20] , int n);
int printf();

int main()
{
	int dotp;

	arr1[0]=3;
	arr1[1]=4;
	arr1[2]=5;

	arr2[0]=6;
	arr2[1]=7;
	arr2[2]=8;

	dotp=dot_product(arr1,arr2,3);

	printf("dot product=%d \n",dotp);


}

int dot_product(int a[20],int b[20] , int n)
{

	int i,p;

	p=0;
	i=0;
	while(i<n){
		p=p+a[i]*b[i];
		i=i+1;
	}
	return(p);
}


	


