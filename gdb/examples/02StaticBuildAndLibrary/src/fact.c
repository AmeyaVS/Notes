#include "fact.h"

uint64_t fact_rec(uint64_t n) {
  return n?n*fact_rec(n-1):1;
}

uint64_t fact_iter(uint64_t n) {
  uint64_t ret = 1;
  for(uint64_t i = n; i >= 1; --i) {
    ret = i * ret;
  }
  return ret;
}

uint64_t fact_dp(uint64_t n) {
  uint64_t f[n+1];

  f[0] = 1;
  f[1] = 1;

  for(uint64_t i = 2; i <= n; ++i) {
    f[i] = i * f[i-1];
  }
  return f[n];
}

static const uint64_t MAXDIGIT = 500;

static int multiply(int x, int res[], int res_size) {
  int carry = 0;

  for(int i = 0; i < res_size; ++i) {
    int prod = res[i] * x + carry;
    res[i] = prod % 10;
    carry = prod / 10;
  }

  while(carry) {
    res[res_size] = carry % 10;
    carry = carry / 10;
    ++res_size;
  }
  return res_size;
}

void fact_long(int n) {
  int res[MAXDIGIT];

  res[0] = 1;
  int res_size = 1;

  for(int x = 2; x <= n; ++x) {
    res_size = multiply(x, res, res_size);
  }

  fprintf(stdout, "fact_long(%d): ", n);
  for(int i = res_size-1; i >= 0; --i) {
    fprintf(stdout, "%d", res[i]);
  }
  fprintf(stdout, "\n");
}
