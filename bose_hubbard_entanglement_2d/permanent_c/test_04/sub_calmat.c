#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/time.h>
#include <math.h>
#include <complex.h>
#include "sub_util.h"

int dsyev_(char *jobz, char *uplo, int *n, double *a,
  int *lda, double *w, double *work, int *lwork, int *info);

int site2idx(int Lx, int Ly, int x, int y){
  return x + Lx*y;
}

int make_sites(int Lx, int Ly,
  int *i1s, int *i2s, double *Js, int *Nhbond){
  int x,y;
  int i0,ix,iy;
  int cnt = 0;
  for(y=0; y<Ly; y++){
    for(x=0; x<Lx; x++){
      i0 = site2idx(Lx,Ly,x,y);
      if(x+1<Lx){
        ix = site2idx(Lx,Ly,x+1,y);
        i1s[cnt] = i0;
        i2s[cnt] = ix;
        Js[cnt] = -1.0;
        cnt++;
      }
      if(y+1<Ly){
        iy = site2idx(Lx,Ly,x,y+1);
        i1s[cnt] = i0;
        i2s[cnt] = iy;
        Js[cnt] = -1.0;
        cnt++;
      }
    }
  }
  Nhbond[0] = cnt;// should be 2*Lx*Ly-Lx-Ly
  return 0;
}

int make_init_CDW(int Lx, int Ly,
  int *init_confs, int *init_is, int *num){
  int x,y;
  int i;
  int tmp;
  for(y=0; y<Ly; y++){
    for(x=0; x<Lx; x++){
      i = site2idx(Lx,Ly,x,y);
      init_confs[i] = (x%2 + y%2)%2;
    }
  }
  tmp = 0;
  for(i=0; i<Lx*Ly; i++){
    tmp += init_confs[i];
  }
  num[0] = tmp;
  tmp = 0;
  for(i=0; i<Lx*Ly; i++){
    if(init_confs[i] == 1){
      init_is[tmp] = i;
      tmp++;
    }
  }
  return 0;
}

int make_ham(int Ns, int *Nhbond, int *i1s, int *i2s, double *Js,
//  double **matH){
  double *matH){
  int i,j;
//  for(i=0; i<Ns; i++){
//    for(j=0; j<Ns; j++){
//      matH[i][j] = 0.0;
//    }
//  }
  for(i=0; i<Ns*Ns; i++){
    matH[i] = 0.0;
  }
  for(i=0; i<Nhbond[0]; i++){
//    matH[i1s[i]][i2s[i]] = Js[i];
//    matH[i2s[i]][i1s[i]] = Js[i];
    matH[i1s[i]*Ns+i2s[i]] = Js[i];
    matH[i2s[i]*Ns+i1s[i]] = Js[i];
  }
  return 0;
}

// a: Hamiltonian (input, will be overwritten), eigenvectors (output)
// w: eigenvalues (output)
// work: memory (input)
// vec: eigenvectors (output, 2d array)
int linalg_eigh(int n, double *a, double *w, double *work, double **vec){
  int i,j,k;
  char jobz,uplo;
  int lda,lwork,info;
  lda = n;
  lwork = 4*n; // LWORK >= max(1,3*N-1), best: LWORK >= (NB+2)*N
//  jobz = 'N';// ene only
  jobz = 'V';// ene and vec
  uplo = 'U';
  dsyev_(&jobz,&uplo,&n,a,&lda,w,work,&lwork,&info);
  if(info != 0){
    printf("info %d",info);
    return 0;
  }
  for(i=0;i<n;i++){
    for(j=0;j<n;j++){
//      k = n*i+j;
      k = i+n*j;
      vec[i][j] = a[k];
    }
  }
  return 0;
}


// calculate matrix
//int calc_matA(double complex **matA, double complex **matX,
//  double complex **matY, double complex **matZ, double complex *vecEPS,
//  int *init_is, int *num,
//  int Ns, int N_A, double tstep){
int calc_matA(double complex **matA, double **matX,
  double complex **matY, double complex **matZ, double *vecEPS,
  int *init_is, int *num,
  int Ns, int N_A, double tstep){
  int i,j,k;
  int ii,ij;
//  for(i=0; i<Ns; i++){
//    for(j=0; j<Ns; j++){
//      matX[i][j] = sin((i+1.0)*(j+1.0)*M_PI/(L+1.0)) * sqrt(2.0/(L+1.0));
//    }
//  }
//  for(i=0; i<Ns; i++){
//    vecEPS[i] = -2.0*cos((i+1.0)*M_PI/(L+1.0));
//  }
  for(i=0; i<Ns; i++){
    for(j=0; j<Ns; j++){
      matY[i][j] = 0.0;
    }
  }
  for(i=0; i<Ns; i++){
    for(j=0; j<Ns; j++){
      for(k=0; k<Ns; k++){
//        matY[i][j] += cexp(-I*vecEPS[k]*tstep) * conj(matX[k][i]) * matX[k][j];
//        matY[i][j] += cexp(-I*vecEPS[k]*tstep) * matX[k][i] * matX[k][j];// correct for 2x2, but not for 3x4
        matY[i][j] += cexp(-I*vecEPS[k]*tstep) * matX[i][k] * matX[j][k];// correct for 2x2, 4x1, 3x4
      }
    }
  }
  for(i=0; i<num[0]; i++){
    for(j=0; j<num[0]; j++){
      matZ[i][j] = 0.0;
    }
  }
  for(i=0; i<num[0]; i++){
    ii = init_is[i];
    for(j=0; j<num[0]; j++){
      ij = init_is[j];
      for(k=0; k<N_A; k++){
        matZ[i][j] += conj(matY[ii][k]) * matY[ij][k];// correct for 2x2, 4x1, 3x4
//        matZ[i][j] += conj(matY[k][ii]) * matY[k][ij];
      }
    }
  }
  for(i=0; i<num[0]; i++){
    for(j=0; j<num[0]; j++){
      matA[i][j]               =  matZ[i][j];
      matA[i][j+num[0]]        = -matZ[i][j];
      matA[i+num[0]][j]        = -matZ[i][j];
      matA[i+num[0]][j+num[0]] =  matZ[i][j];
    }
    matA[i][i+num[0]] += 1.0;
    matA[i+num[0]][i] += 1.0;
  }
  return 0;
}
