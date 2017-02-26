#include <stdio.h>

void setint(int* ptr, int val) {
  *ptr = val;
}

int main() {
  int a = 100;
  printf("a: %d\n", a);
  setint(&a, 20);
  printf("a: %d\n", a);

  int* b = NULL;
  setint(b, 50);
  printf("b: %d\n", *b);
  return 0;
}
