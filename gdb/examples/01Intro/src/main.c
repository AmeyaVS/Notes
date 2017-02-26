#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

typedef unsigned int uint32_t;

void populate(uint32_t **arr, uint32_t n) {
  *arr = (uint32_t *) malloc(sizeof(uint32_t) * n);
  if (*arr != NULL) {
    // Seed the random number generator.
    srand(time(NULL));

    uint32_t  i = 0;
    for( i = 0; i < n; ++i) {
      (*arr)[i] = rand()%n;
    }
  } else {
    fprintf(stderr, "Unable to allocate memory.\n");
    exit(-1);
  }
}

void print(uint32_t *arr, uint32_t n) {
  uint32_t i = 0;
  printf("Array: ");
  for(i = 0; i < n ; ++i) {
    printf("%u, ", arr[i]);
  }
  printf("\n");
}

void sort(uint32_t **arr, uint32_t n) {
  uint32_t i = 0, j= 0;
  uint32_t swapCount = 0;
  for(i = 0; i < (n - 1); ++i) {
    swapCount = 0;
    for(j = 0; j < (n - 1 - i); ++j) {
      if ((*arr)[j] > (*arr)[j + 1]) {
        uint32_t temp = (*arr)[j];
        (*arr)[j] = (*arr)[j + 1];
        (*arr)[j + 1] = temp;
        ++swapCount;
      }
    }
    if (!swapCount) {
      break;
    }
  }
}

void do_work(uint32_t n) {
  uint32_t *arr = NULL;
  // Populate the int[] with random numbers.
  populate(&arr, n);
  // Print the array.
  print(arr, n);
  // Sort the array.
  sort(&arr, n);
  // Print the sorted array.
  print(arr, n);
  // release the memory allocated by populate.
  free(arr);
}

void usage(const char* pgName) {
  fprintf(stderr, "Usage %s -n <number>\n\t where number is greater than 1.\n", pgName);
}

int main(int argc, char *argv[]) {
  char option = '\0';

  if (argc == 1) {
    goto error;
  }
  uint32_t n = 0;
  while((option = getopt(argc, argv, "n:h")) != EOF) {
    switch(option) {
      case 'n':
        n = atoi(optarg);
        break;
      case 'h':
        goto error;
        break;
      default:
        fprintf(stderr, "Try %s -h\n", argv[0]);
        exit(-1);
        break;
    }
  }
  if (n > 1) {
    do_work(n);
  } else {
    goto error;
  }
  return 0;
error:
  usage(argv[0]);
  exit(-1);
}
