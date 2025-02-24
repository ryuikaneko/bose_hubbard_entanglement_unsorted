#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include "./dSFMT/dSFMT.h"

dsfmt_t dsfmt;

int main(){
  int i;
  int seed;
  double rnd;
  seed = 12345;
  dsfmt_init_gen_rand(&dsfmt,seed);
  for(i=0; i<100; i++){
    rnd = dsfmt_genrand_close_open(&dsfmt);
    printf("%f\n",rnd);
  }
  return 0;
}
