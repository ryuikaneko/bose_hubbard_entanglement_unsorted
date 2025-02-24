#ifndef SUB_CALMAT_H
#define SUB_CALMAT_H

int site2idx(int Lx, int Ly, int x, int y);
int make_sites(int Lx, int Ly,
  int *i1s, int *i2s, double *Js, int *Nhbond);
int make_init_CDW(int Lx, int Ly,
  int *init_confs, int *init_is, int *num);
int make_ham(int Ns, int *Nhbond, int *i1s, int *i2s, double *Js,
  double *matH);
int linalg_eigh(int n, double *a, double *w, double *work, double **vec);
int calc_matA(double complex **matA, double **matX,
  double complex **matY, double complex **matZ, double *vecEPS,
  int *init_is, int *num,
  int Ns, int N_A, double tstep);

#endif
