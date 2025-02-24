// https://stackoverflow.com/questions/47137488/c-generating-random-numbers-using-a-pcg-implementation

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <math.h>

#include "pcg_basic.h"

double myrand(pcg32_random_t *rngptr){
  return ldexp(pcg32_random_r(rngptr) ,-32);
}

int main(void)
{
  int i;
  pcg32_random_t rngptr;
  pcg32_srandom_r(&rngptr, time(NULL), (intptr_t)&rngptr);
  for(i=0; i<100; i++){
    printf("%f\n",myrand(&rngptr));
  }
}
