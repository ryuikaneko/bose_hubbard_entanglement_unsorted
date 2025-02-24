#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <complex.h>

#ifndef M_PI
  #define M_PI 3.14159265358979323846
#endif

// memory allocation
void *malloc2d(size_t size, int n1, int n2){
  int i;
  int t=size*n2;
  char **a1, *a2;
  a1 = (char**)malloc((sizeof(*a1) + t) * n1);
  if(a1){
    a2 = (char*)(a1 + n1);
    for(i=0; i<n1; i++){
      a1[i] = a2;
      a2 += t;
    }
    return a1;
  }
  return NULL;
}

// calculate matrix
int calc_matA(double complex **matA, double complex **matX,
  double complex **matY, double complex **matZ, double complex *vecEPS,
  int L, int L_A, double tstep){
  int i,j,k;
  for(i=0; i<L; i++){
    for(j=0; j<L; j++){
      matX[i][j] = sin((i+1.0)*(j+1.0)*M_PI/(L+1.0)) * sqrt(2.0/(L+1.0));
    }
  }
  for(i=0; i<L; i++){
    vecEPS[i] = -2.0*cos((i+1.0)*M_PI/(L+1.0));
  }
  for(i=0; i<L; i++){
    for(j=0; j<L; j++){
      matY[i][j] = 0.0;
    }
  }
  for(i=0; i<L; i++){
    for(j=0; j<L; j++){
      for(k=0; k<L; k++){
        matY[i][j] += cexp(-I*vecEPS[k]*tstep) * conj(matX[k][i]) * matX[k][j];
      }
    }
  }
  for(i=0; i<L/2; i++){
    for(j=0; j<L/2; j++){
      matZ[i][j] = 0.0;
    }
  }
//// MI
//  for(i=0; i<L; i++){
//    for(j=0; j<L; j++){
//// CDW
  for(i=0; i<L/2; i++){
    for(j=0; j<L/2; j++){
      for(k=0; k<L_A; k++){
//        matZ[i][j] += conj(matY[i][k]) * matY[j][k];// MI
        matZ[i][j] += conj(matY[2*i][k]) * matY[2*j][k];// CDW
      }
    }
  }
//// MI
//  for(i=0; i<2; i++){
//    for(j=0; j<2; j++){
//// CDW
  for(i=0; i<L/2; i++){
    for(j=0; j<L/2; j++){
      matA[i][j] = matZ[i][j];
      matA[i][j+L/2] = -matZ[i][j];
      matA[i+L/2][j] = -matZ[i][j];
      matA[i+L/2][j+L/2] = matZ[i][j];
    }
    matA[i][i+L/2] += 1.0;
    matA[i+L/2][i] += 1.0;
  }
  return 0;
}

// print matrix
int print_mat(double complex **matA, int L){
  int i,j;
  for(i=0; i<L; i++){
    for(j=0; j<L; j++){
      printf("%+.6f%+.6fI ",creal(matA[i][j]),cimag(matA[i][j]));
    }
    printf("\n");
  }
  printf("\n");
  return 0;
}

// print vector
int print_vec(double complex *vecA, int L){
  int i;
  for(i=0; i<L; i++){
    printf("%+.6f%+.6fI ",creal(vecA[i]),cimag(vecA[i]));
  }
  printf("\n\n");
  return 0;
}


int main(void){
  int i,j;
  int L,L_A;
  double tstep;

  tstep = 1.0;
  L = 8;
  L_A = L/2;

// prepare matrix
  double complex **matA = (double complex**)malloc2d(sizeof(double complex),L,L);
  double complex **matX = (double complex**)malloc2d(sizeof(double complex),L,L);
  double complex **matY = (double complex**)malloc2d(sizeof(double complex),L,L);
//  double complex **matZ = (double complex**)malloc2d(sizeof(double complex),L,L);// MI
  double complex **matZ = (double complex**)malloc2d(sizeof(double complex),L/2,L/2);// CDW
  double complex *vecEPS = (double complex*)malloc(sizeof(double complex)*L);
  calc_matA(matA,matX,matY,matZ,vecEPS,L,L_A,tstep);
//  print_mat(matX,L);
//  print_vec(vecEPS,L);
//  print_mat(matY,L);
//  print_mat(matZ,L/2);
  print_mat(matA,L);
// free matrix
  free(matA);
  free(matX);
  free(matY);
  free(matZ);
  free(vecEPS);

  return 0;
}

