#include <stdio.h>
#include "fact.h"
#include "fibo.h"

void caller( uint64_t (*func)(uint64_t)) {
  const uint64_t count  = 20;
#if 1
  for(uint64_t i = 0; i <= count; ++i) {  
    fprintf(stdout, "func(%llu): %llu\n", i, func(i));
  }
#else
  fprintf(stdout, "func(%llu): %llu\n", count, func(count));
#endif
}

int main() {

  caller(fact_rec);
  caller(fact_iter);
  caller(fact_dp);

  caller(fibo_rec);
  caller(fibo_iter);
  caller(fibo_dp);
  caller(fibo_mat);
  caller(fibo_mat2);
  caller(fibo_logn);


  fact_long(100);
  return 0;
}
