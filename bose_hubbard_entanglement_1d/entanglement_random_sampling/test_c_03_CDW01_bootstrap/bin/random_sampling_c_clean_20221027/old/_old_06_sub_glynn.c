#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <complex.h>
#include "./rnd/pcg_basic.h"
#include "sub_util.h"

double complex glynn_child(double complex **matA, int L,
  double complex *vec, double complex *tmp){
  int i,j;
  double rnd;
  double complex tmp_perm;
  for(i=0; i<L; i++){
    rnd = M_PI * (2.0 * ldexp(pcg32_random(),-32) - 1.0);// rnd in (-pi,pi]
    vec[i] = cexp(I*rnd);
  }
  for(i=0; i<L; i++){
    tmp[i] = 0.0;
  }
  for(i=0; i<L; i++){
    for(j=0; j<L; j++){
      tmp[i] += matA[i][j] * vec[j];
    }
  }
  tmp_perm = conj(vec[0]) * tmp[0];
  for(i=1; i<L; i++){
    tmp_perm *= conj(vec[i]) * tmp[i];
  }
  return tmp_perm;
}

int glynn_all(double complex **matA, int L,
  double complex *vec, double complex *tmp, uint64_t Ntotal, uint64_t Nblock,
  double complex *histtotal, double complex *histblock){
  uint64_t i,j;
  uint64_t Nblocksize;
  double complex tmp_perm;
  double complex tmp_ave;
  Nblocksize = (uint64_t) Ntotal/Nblock;
  for(i=0; i<Nblock; i++){
    tmp_ave = 0.0;
    for(j=0; j<Nblocksize; j++){
      tmp_perm = glynn_child(matA,L,vec,tmp);
      histtotal[i*Nblocksize+j] = tmp_perm;
      tmp_ave += tmp_perm;
    }
    histblock[i] = tmp_ave/Nblocksize;
  }
  return 0;
}

// no histtotal
int glynn(double complex **matA, int L,
  double complex *vec, double complex *tmp, uint64_t Ntotal, uint64_t Nblock,
  double complex *histblock){
  uint64_t i,j;
  uint64_t Nblocksize;
  double complex tmp_perm;
  double complex tmp_ave;
  Nblocksize = (uint64_t) Ntotal/Nblock;
  for(i=0; i<Nblock; i++){
    tmp_ave = 0.0;
    for(j=0; j<Nblocksize; j++){
      tmp_perm = glynn_child(matA,L,vec,tmp);
      tmp_ave += tmp_perm;
    }
    histblock[i] = tmp_ave/Nblocksize;
  }
  return 0;
}

int blocking_jackknife(uint64_t Nblock, double complex *histblock, double complex *val){
  uint64_t i;
  double ave_re;
  double ave_im;
  double err_re;
  double err_im;
  double complex tmp;
  // average
  ave_re = 0.0;
  ave_im = 0.0;
  for(i=0; i<Nblock; i++){
    tmp = histblock[i];
    ave_re += creal(tmp);
    ave_im += cimag(tmp);
  }
  ave_re /= Nblock;
  ave_im /= Nblock;
  // error
  err_re = 0.0;
  err_im = 0.0;
  for(i=0; i<Nblock; i++){
    tmp = histblock[i];
    err_re += (creal(tmp)-ave_re)*(creal(tmp)-ave_re);
    err_im += (cimag(tmp)-ave_im)*(cimag(tmp)-ave_im);
  }
  err_re = sqrt(err_re/(Nblock*(Nblock-1.0)));
  err_im = sqrt(err_im/(Nblock*(Nblock-1.0)));
  val[0] = ave_re + I*ave_im;
  val[1] = err_re + I*err_im;
  return 0;
}
