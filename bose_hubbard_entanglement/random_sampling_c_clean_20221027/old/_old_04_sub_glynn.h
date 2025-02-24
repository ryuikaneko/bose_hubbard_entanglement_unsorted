#ifndef SUB_GLYNN_H
#define SUB_GLYNN_H

double complex glynn_child(double complex **matA, int L,
  double complex *vec, double complex *tmp);

int glynn(double complex **matA, int L,
  double complex *vec, double complex *tmp, uint64_t Nsmp,
  double *val, double complex *hist);

#endif
