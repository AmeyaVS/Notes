/*P8.6 Program of sorting using merge sort through recursion*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX 1024*128

void merge_sort( int arr[], int low, int up );
void merge( int arr[], int temp[], int low1, int up1, int low2, int up2 );
void copy(int arr[], int temp[], int low, int up );

void populate(int **arr, int n) {
  *arr = (int *) malloc(sizeof(int) * n);
  if (*arr != NULL) {
    // Seed the random number generator.
    srand(time(NULL));

    int  i = 0;
    for( i = 0; i < n; ++i) {
      (*arr)[i] = rand()%n;
    }
  } else {
    fprintf(stderr, "Unable to allocate memory.\n");
    exit(-1);
  }
}


int main(int argc, char *argv[])
{
	int i, n = 0;
  int *arr = NULL;
	
  if (argc > 1) {
    n = atoi(argv[1]);
    if (n > 1) {
      populate(&arr, n);
    } else {
      return -1;
    }
  } else {
    return -1;
  }
	
  printf("\nUnsorted list is :\n");
  for(i=0; i<n; i++)
    printf("%d ", arr[i]);
  printf("\n");

  merge_sort( arr, 0, n-1);

  printf("\nSorted list is :\n");
  for(i=0; i<n; i++)
    printf("%d ", arr[i]);
  printf("\n");

  free(arr);
  return 0;
}/*End of main()*/

void merge_sort(int arr[], int low, int up)
{
	int mid;
	int temp[MAX];
	if(low<up)/* if more than one element*/
	{
		mid = (low+up)/2;
		merge_sort( arr, low , mid );  /* Sort arr[low:mid] */
		merge_sort( arr, mid+1, up );  /* Sort arr[mid+1:up] */
		/*Merge arr[low:mid] and arr[mid+1:up] to temp[low:up]*/
		merge( arr, temp, low, mid, mid+1, up ); 
		/* Copy temp[low:up] to arr[low:up] */ 
		copy(arr,temp,low, up);	
	}
}/*End of merge_sort*/

/*Merges arr[low1:up1] and arr[low2:up2] to temp[low1:up2]*/
void merge( int arr[], int temp[], int low1, int up1, int low2, int up2 )
{
	int i = low1;
	int j = low2 ;
	int k = low1 ;

	while( (i <= up1) && (j <=up2) )
	{
		if(arr[i] <= arr[j])
			temp[k++] = arr[i++] ;
		else
			temp[k++] = arr[j++] ;
	}
	while( i <= up1 )
		temp[k++]=arr[i++];
	while( j <= up2 )
		temp[k++]=arr[j++];
}/*End of merge()*/

void copy(int arr[], int temp[], int low, int up )
{
	int i;
	for(i=low; i<=up; i++)
		arr[i]=temp[i];
}

