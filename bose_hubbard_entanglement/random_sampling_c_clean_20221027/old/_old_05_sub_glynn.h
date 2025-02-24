#ifndef SUB_GLYNN_H
#define SUB_GLYNN_H

double complex glynn_child(double complex **matA, int L,
  double complex *vec, double complex *tmp);

int glynn(double complex **matA, int L,
  double complex *vec, double complex *tmp, uint64_t Ntotal, uint64_t Nblock,
  double complex *histtotal, double complex *histblock);

int blocking_jackknife(uint64_t Nblock, double complex *histblock, double complex *val);

#endif
