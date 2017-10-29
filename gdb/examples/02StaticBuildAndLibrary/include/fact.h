#ifndef FACT_H_
#define FACT_H_

#include <stdio.h>

typedef unsigned long long uint64_t;

uint64_t fact_rec(uint64_t n);
uint64_t fact_iter(uint64_t n);
uint64_t fact_dp(uint64_t n);
void fact_long(int n);

#endif // FACT_H_
