#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <complex.h>

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


int main(void){
  int i,j;
  int n;

  n = 10;

// prepare matrix
//  double **matA = (double**)malloc2d(sizeof(double),n,n);
  double complex **matA = (double complex**)malloc2d(sizeof(double complex),n,n);
  for(i=0; i<n; i++){
    for(j=0; j<n; j++){
//      matA[i][j] = 1.0 + 1.0*I;
      matA[i][j] = i + j*I;
    }
  }

// print matrix
  for(i=0; i<n; i++){
    for(j=0; j<n; j++){
      printf("%+.2f%+.2f ",creal(matA[i][j]),cimag(matA[i][j]));
    }
    printf("\n");
  }
  printf("\n");

// free matrix
  free(matA);
  return 0;
}

