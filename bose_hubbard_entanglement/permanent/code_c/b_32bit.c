#include <stdio.h>
#include <stdlib.h>
//#include <inttypes.h>
#include <sys/time.h>
#include <math.h>

// calculate product
double prod(int n, double *vec){
  int i;
  double tmp;
  tmp = vec[0];
  for(i=1; i<n; i++){
    tmp *= vec[i];
  }
  return tmp;
}

// calculate sum along 0 axis
int sum0axis(int n, double **mat, double *vec){
  int i;
  int j;
  for(i=0; i<n; i++){
    vec[i] = 0.0;
  }
  for(i=0; i<n; i++){
    for(j=0; j<n; j++){
      vec[j] += mat[i][j];
    }
  }
  return 0;
}

// calculate sum along 1 axis
int sum1axis(int n, double **mat, double *vec){
  int i;
  int j;
  for(i=0; i<n; i++){
    vec[i] = 0.0;
  }
  for(i=0; i<n; i++){
    for(j=0; j<n; j++){
      vec[i] += mat[i][j];
    }
  }
  return 0;
}

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

// permnanent
// using daxpy ( y <- alpha*x + y) in lapack might be faster
double perm(int n, double **a){
  int i,j;
  int ii;
  double f;
  int *x = (int*)malloc(n*sizeof(int));
  double *w = (double*)malloc(n*sizeof(double));
  int x0;
  double *a1;
  f = 0.0;
  for(i=0; i<n; i++){
    x[i] = 0;
  }
  sum0axis(n,a,w);
  a1 = a[n-1];
  for(i=0; i<n; i++){
//    w[i] = a[n-1][i] - 0.5*w[i];
    w[i] = a1[i] - 0.5*w[i];
  }
//  for(ii=1, ii<(1LLU<<(n-2))+1, ii++){
  for(ii=1; ii<(1<<(n-2))+1; ii++){
    f -= prod(n,w);
    x[0] = 1-x[0];
    x0 = 2*x[0]-1;
    a1 = a[0];
    for(i=0; i<n; i++){
//      w[i] += (2.0*x[0]-1.0)*a[0][i];
      w[i] += x0*a1[i];
    }
    f += prod(n,w);
    j = 1;
    while(1-x[j-1]==1){
      j++;
    };
    j++;
    x[j-1] = 1-x[j-1];
    x0 = 2*x[j-1]-1;
    a1 = a[j-1];
    for(i=0; i<n; i++){
//      w[i] += (2.0*x[j-1]-1.0)*a[j-1][i];
      w[i] += x0*a1[i];
    }
  }
  f *= 2.0 * pow(-1.0,n);
  free(x);
  free(w);
  return f;
}


int main(void){
  int i,j;
  int n;
  double val;

  n = 30;
//  n = 4;

// prepare matrix
  double **f = (double**)malloc2d(sizeof(double),n,n);
  for(i=0; i<n; i++){
    for(j=0; j<n; j++){
//      f[i][j] = i*n+j+1.0;
      f[i][j] = 1.0;
    }
  }

// prinf matrix
/*
  for(i=0; i<n; i++){
    for(j=0; j<n; j++){
      printf("%f ",f[i][j]);
    }
    printf("\n");
  }
  printf("\n");
*/

// calculate permanent
  val = perm(n,f);

  printf("%g\n",val);

  free(f);
  return 0;
}
