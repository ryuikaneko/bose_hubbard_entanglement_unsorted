#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <complex.h>
#include "./rnd/pcg_basic.h"
#include "sub_util.h"

double complex glynn_child(double complex **matA, int L,
  double complex *vec, double complex *tmp){
  int j,k;
  double rnd;
  double complex tmp_perm;
  for(j=0; j<L; j++){
    rnd = M_PI * (2.0 * ldexp(pcg32_random(),-32) - 1.0);// rnd in (-pi,pi]
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
  tmp_perm = conj(vec[0]) * tmp[0];
  for(j=1; j<L; j++){
    tmp_perm *= conj(vec[j]) * tmp[j];
  }
//  tmp_perm = 1.0;
//  for(j=0; j<L; j++){
//    tmp_perm *= conj(vec[j]) * tmp[j];
//  }
  return tmp_perm;
}

int glynn(double complex **matA, int L,
  double complex *vec, double complex *tmp, uint64_t Nsmp,
  double *val, double complex *hist){
  uint64_t i;
  double complex tmp_perm;
  val[0] = 0.0;
  val[1] = 0.0;
  for(i=0; i<Nsmp; i++){
    tmp_perm = glynn_child(matA,L,vec,tmp);
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
