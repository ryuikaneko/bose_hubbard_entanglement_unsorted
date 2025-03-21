#include <stdio.h>
#include <stdlib.h>
//#include <inttypes.h>
#include <sys/time.h>
#include <math.h>

// https://stackoverflow.com/questions/10996418/efficient-integer-compare-function
//  -1 if a < b
//   0 if a = b
//  +1 if a > b
int cmp(int a, int b){
  return (a>b)-(a<b);
}

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
double perm(int n, double **M){
  int i;
  int old_gray;
  int new_gray;
  int gray_diff;
  int gray_diff_index;
  int num_loops;
  int bin_index;
  int sign;// takes -1,1
  int direction;// takes -2,0,+2
  double total;
  double *row_comb = (double*)malloc(n*sizeof(double));
  double reduced;

  if(n==0) return 1.0;
  sum0axis(n,M,row_comb);
  total = 0.0;
  old_gray = 0;
  sign = 1;
  num_loops = 1 << (n-1);
  for(bin_index=1; bin_index<=num_loops; bin_index++){
    reduced = prod(n,row_comb);
    total += sign * reduced;
    new_gray = bin_index ^ (bin_index / 2);
    gray_diff = old_gray ^ new_gray;
    gray_diff_index = __builtin_ctz(gray_diff);
    direction = 2 * cmp(old_gray,new_gray);
    for(i=0; i<n; i++){
      row_comb[i] += M[gray_diff_index][i] * direction;
    }
    sign = -sign;
    old_gray = new_gray;
  }
  free(row_comb);
  return total / num_loops;
}


int main(void){
  int i,j;
  int n;
  double val;

  n = 30;

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
