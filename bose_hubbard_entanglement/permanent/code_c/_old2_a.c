#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

// https://stackoverflow.com/questions/66520572/print-a-number-converted-in-base-2-recursively
void base(uint64_t n){
  if(n==0) return;
  base(n/2);
  printf("%llu",n%2);
}

// https://stackoverflow.com/questions/10996418/efficient-integer-compare-function
//  -1 if a < b
//   0 if a = b
//  +1 if a > b
int cmp(uint64_t a, uint64_t b){
  return (a>b)-(a<b);
}

// calculate product
double prod(uint64_t n, double *vec){
  int i;
  double tmp;
  tmp = vec[0];
  for(i=1; i<n; i++){
    tmp *= vec[i];
  }
  return tmp;
}

// calculate sum
int sum0axis(uint64_t n, double **mat, double *vec){
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
void *malloc2d(size_t size, uint64_t n1, uint64_t n2)
{
  uint64_t i;
  uint64_t t=size*n2;
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
double perm(uint64_t n, double **M)
{
  int i;
  int sign;
  uint64_t old_gray;
  uint64_t new_gray;
  uint64_t gray_diff;
  uint64_t gray_diff_index;
  uint64_t num_loops;
  uint64_t bin_index;
  int direction;// takes -2,0,+2
  double total;
//  double row_comb;// TBA
  double *row_comb = (double*)malloc(n*sizeof(double));
  double reduced;
//  double new_vector;// TBA


  int j;
  for(i=0; i<n; i++){
    for(j=0; j<n; j++){
      printf("%f ",M[i][j]);
    }
    printf("\n");
  }
  printf("\n");


  if(n==0) return 1.0;
//  row_comb = 0.0;// TBA
//  for(i=0; i<n; i++){
//    row_comb[i] = 0.0;
//  }
  sum0axis(n,M,row_comb);
  total = 0.0;
  old_gray = 0;
  sign = 1;
  num_loops = 1LLU << (n-1);
  printf("%llu\n",num_loops);

  for(bin_index=1; bin_index<=num_loops; bin_index++){
//    reduced = 0.0;// TBA
    reduced = prod(n,row_comb);
    total += sign * reduced;
    new_gray = bin_index ^ (bin_index / 2);
    gray_diff = old_gray ^ new_gray;
    gray_diff_index = __builtin_ctzll(gray_diff);
//
    printf("# gray_diff %llu\n",gray_diff);
//    base(gray_diff);
//    printf("\n");
    printf("# gray_diff_index %llu\n",gray_diff_index);
//    base(gray_diff_index);
//    printf("\n");
//
//    new_vector = 0.0;// TBA
    direction = 2 * cmp(old_gray,new_gray);
    printf("#direction %d\n",direction);
//
//    for(i=0; i<n; i++){// TBA
//      row_comb[i] += new_vector[i] * direction;
//    }
    for(i=0; i<n; i++){
      row_comb[i] += M[gray_diff_index][i] * direction;
    }
    sign = -sign;
    old_gray = new_gray;
  }
  free(row_comb);
  return total / num_loops;
}


int main(void)
{
  uint64_t i,j;
  uint64_t n;
  uint64_t n1,n2;
  double val;

  n = 4;
  n1 = n;
  n2 = n;

// prepare matrix
  double **f = (double**)malloc2d(sizeof(double),n1,n2);
  for(i=0; i<n1; i++){
    for(j=0; j<n2; j++){
      f[i][j] = i*n2+j+1.0;
    }
  }
  for(i=0; i<n1; i++){
    for(j=0; j<n2; j++){
      printf("%llu %llu %f\n",i,j,f[i][j]);
    }
  }
  printf("\n");

/*
// check log2
  for(i=0; i<64; i++){
// https://stackoverflow.com/questions/26684351/bitwise-operations-on-unsigned-long-long-in-c
//    j = 1 << i;// up to 2^32
    j = 1LLU << i;// up to 2^32
//    j = pow(2,i);
    printf("%llu %llu %d\n",i,j,__builtin_ctzll(j));// int: __builtin_ctz, uint64_t: __builtin_ctzll
  }
  printf("\n");
*/

// calculate permanent
  val = perm(n,f);
  printf("\n");

  printf("%f\n",val);

  free(f);
  return 0;
}

