#include "fibo.h"

uint64_t fibo_rec(uint64_t n) {
  uint64_t ret = 0;
  switch(n) {
    case 0:
      break;
    case 1:
      ret = 1;
      break;
    default:
      ret = fibo_rec(n-1) + fibo_rec(n-2);
      break;
  }
  return ret;
}

uint64_t fibo_iter(uint64_t n) {
  uint64_t a = 0, b = 1, c= 0;
  if(n == 0) {
    return a;
  }
  for(uint64_t i = 2; i <= n; ++i) {
    c = a + b;
    a = b;
    b = c;
  }
  return b;
}

uint64_t fibo_dp(uint64_t n) {
  uint64_t f[n+1];

  f[0] = 0;
  f[1] = 1;

  for(uint64_t i = 2; i<= n; ++i) {
    f[i] = f[i-1] + f[i-2];
  }
  return f[n];
}

static void multiply(uint64_t F[2][2], uint64_t M[2][2]) {
  uint64_t w = F[0][0]*M[0][0] + F[0][1]*M[1][0];
  uint64_t x = F[0][0]*M[0][1] + F[0][1]*M[1][1];
  uint64_t y = F[1][0]*M[0][0] + F[1][1]*M[1][0];
  uint64_t z = F[1][0]*M[0][1] + F[1][1]*M[1][1];

  F[0][0] = w;
  F[0][1] = x;
  F[1][0] = y;
  F[1][1] = z;
}

static void power(uint64_t F[2][2], uint64_t n) {
  uint64_t M[2][2] = {{1, 1},
                      {1, 0}};

  for(uint64_t i = 2; i <= n; ++i) {
    multiply(F, M);
  }
}

uint64_t fibo_mat(uint64_t n) {
  uint64_t F[2][2] = {{1, 1},
                      {1, 0}};
  if(n == 0) {
    return 0;
  }
  power(F, n-1);
  return F[0][0];
}

static void power_opt(uint64_t F[2][2], uint64_t n) {
  if( n == 0 || n == 1) {
    return;
  }
  uint64_t M[2][2] = {{1, 1}, {1, 0}};

  power_opt(F, n/2);
  multiply(F, F);
  
  if(n%2 != 0) {
    multiply(F, M);
  }
}

uint64_t fibo_mat2(uint64_t n) {
  uint64_t F[2][2] = {{1, 1}, {1, 0}};
  if(n == 0) {
    return 0;
  }
  power_opt(F, n-1);
  return F[0][0];
}

#define MAX 1000
static uint64_t f_logn[MAX] = {0};

uint64_t fibo_logn(uint64_t n) {
  if(n == 0) {
    return 0;
  }
  if(n == 1 || n == 2) {
    return (f_logn[n] = 1);
  }

  if(f_logn[n]) {
    return f_logn[n];
  }

  uint64_t k = (n & 1)? (n+1)/2: n/2;

  f_logn[n] = (n & 1)? (fibo_logn(k)*fibo_logn(k) + fibo_logn(k-1)*fibo_logn(k-1))
                : (2*fibo_logn(k-1) + fibo_logn(k))*fibo_logn(k);

  return f_logn[n];
}
