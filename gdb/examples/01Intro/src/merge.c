/*P8.6 Program of sorting using merge sort through recursion*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX 1000000

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
	int i, n;//, arr[MAX];
  int *arr = NULL;
	
	//printf("Enter the number of elements : ");
	//scanf("%d",&n);
  n= 0;
  if (argc > 1) {
    n = atoi(argv[1]);
    populate(&arr, n);
  } else {
    return -1;
  }
	
#if 0
	for(i=0; i<n; i++)
	{
		printf("Enter element %d : ",i+1);
		scanf("%d",&arr[i]);
	}
#endif

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

#if 0
/* C program for Merge Sort */
/**
 * Refer: http://quiz.geeksforgeeks.org/merge-sort/
 */
#include<stdlib.h>
#include<stdio.h>
//
#include <time.h>

// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge(int arr[], int l, int m, int r)
{
	int i, j, k;
	int n1 = m - l + 1;
	int n2 =  r - m;

	/* create temp arrays */
	int L[n1], R[n2];

	/* Copy data to temp arrays L[] and R[] */
	for (i = 0; i < n1; i++)
		L[i] = arr[l + i];
	for (j = 0; j < n2; j++)
		R[j] = arr[m + 1+ j];

	/* Merge the temp arrays back into arr[l..r]*/
	i = 0; // Initial index of first subarray
	j = 0; // Initial index of second subarray
	k = l; // Initial index of merged subarray
	while (i < n1 && j < n2)
	{
		if (L[i] <= R[j])
		{
			arr[k] = L[i];
			i++;
		}
		else
		{
			arr[k] = R[j];
			j++;
		}
		k++;
	}

	/* Copy the remaining elements of L[], if there
		 are any */
	while (i < n1)
	{
		arr[k] = L[i];
		i++;
		k++;
	}

	/* Copy the remaining elements of R[], if there
		 are any */
	while (j < n2)
	{
		arr[k] = R[j];
		j++;
		k++;
	}
}

/* l is for left index and r is right index of the
	 sub-array of arr to be sorted */
void mergeSort(int arr[], int l, int r)
{
	if (l < r)
	{
		// Same as (l+r)/2, but avoids overflow for
		// large l and h
		int m = l+(r-l)/2;

		// Sort first and second halves
		mergeSort(arr, l, m);
		mergeSort(arr, m+1, r);

		merge(arr, l, m, r);
	}
}

/* UTILITY FUNCTIONS */
/* Function to print an array */
void printArray(int A[], int size)
{
	int i;
	for (i=0; i < size; i++)
		printf("%d ", A[i]);
	printf("\n");
}

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


/* Driver program to test above functions */
int main(int argc, char *argv[])
{
  int *arr = NULL;
  int arr_size = 0;

  if (argc > 1) {
    arr_size = atoi(argv[1]);
    populate(&arr, arr_size);
  } else {
    fprintf(stderr, "Provide value n\ne.g.: %s <n>\n", argv[0]);
    return -1;
  }
#if 0
	int arr[] = {12, 11, 13, 5, 6, 7};
	int arr_size = sizeof(arr)/sizeof(arr[0]);
#endif

	printf("Given array is \n");
	printArray(arr, arr_size);

	mergeSort(arr, 0, arr_size - 1);

	printf("\nSorted array is \n");
	printArray(arr, arr_size);

  free(arr);
  arr = NULL;
	return 0;
}
#endif
