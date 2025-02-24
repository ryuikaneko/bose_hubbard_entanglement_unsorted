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

// calculate matrix
int calc_matA(double complex **matA, int n){
  int i,j;
  for(i=0; i<n; i++){
    for(j=0; j<n; j++){
      matA[i][j] = i + j*I;
    }
  }
  return 0;
}

// print matrix
int print_matA(double complex **matA, int n){
  int i,j;
  for(i=0; i<n; i++){
    for(j=0; j<n; j++){
      printf("%+.2f%+.2f ",creal(matA[i][j]),cimag(matA[i][j]));
    }
    printf("\n");
  }
  printf("\n");
  return 0;
}


int main(void){
  int i,j;
  int n;

  n = 10;

// prepare matrix
  double complex **matA = (double complex**)malloc2d(sizeof(double complex),n,n);
  calc_matA(matA,n);
  print_matA(matA,n);
// free matrix
  free(matA);
  return 0;
}

