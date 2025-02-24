#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <complex.h>
#include "./rnd/pcg_basic.h"
#include "sub_util.h"

int glynn(double complex **matA, int L,
  double complex *vec, double complex *tmp, uint64_t Nsmp,
  pcg32_random_t rng,
  double *val, double complex *hist){
//  int i,j,k;
  uint64_t i;
  int j,k;
  double rnd;
  double complex tmp_perm;
  val[0] = 0.0;
  val[1] = 0.0;
  for(i=0; i<Nsmp; i++){
    hist[i] = 0.0;
  }
  for(i=0; i<Nsmp; i++){
    for(j=0; j<L; j++){
//      rnd = M_PI * (2.0*drand48()-1.0);
      rnd = M_PI * (2.0 * ldexp(pcg32_random_r(&rng),-32) - 1.0);
      vec[j] = cexp(I*rnd);
    }
    for(j=0; j<L; j++){
      tmp[j] = 0.0;
    }
    for(j=0; j<L; j++){
      for(k=0; k<L; k++){
        tmp[j] += matA[j][k] * vec[k];
      }
    }
    tmp_perm = 1.0;
    for(j=0; j<L; j++){
      tmp_perm *= conj(vec[j]) * tmp[j];
    }
    hist[i] = tmp_perm;
    tmp_perm = creal(tmp_perm);
    val[0] += tmp_perm;
    val[1] += tmp_perm * tmp_perm;
  }
  val[0] /= Nsmp;
  val[1] /= Nsmp;
  val[1] = sqrt(fabs(val[1] - val[0]*val[0])/Nsmp);
  return 0;
}
