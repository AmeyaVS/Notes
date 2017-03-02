#include <stdio.h>

typedef union _elems {
  char str[8];
  int n[2];
  long long int lln;
} elems;

typedef struct _my_struct {
  elems e;
} my_struct;


void set_my_struct(my_struct *ms) {
  for(int i = 0; i < 8; ++i) {
    ms->e.str[i] = 0x30 + i;
  }
}

void print_my_struct(my_struct *ms) {
  for(int i = 0; i < 8; ++i) {
    char val = ms->e.str[i];
    printf("0x%02x: %c\n", val, val);
  }
}

void setint(int* ptr, int val) {
  *ptr = val;
}

int main() {
  int a = 100;
  printf("a: %d\n", a);
  setint(&a, 20);
  printf("a: %d\n", a);
#if 1
  int* b = NULL;
  setint(b, 50);
  printf("b: %d\n", *b);
#endif

  my_struct ms;

  set_my_struct(&ms);
  print_my_struct(&ms);

  setint(&ms.e.n[1], 0x33323130);

  print_my_struct(&ms);
  return 0;
}
