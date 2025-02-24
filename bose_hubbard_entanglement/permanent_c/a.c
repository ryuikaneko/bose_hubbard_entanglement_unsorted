#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/time.h>
#include <math.h>
#include <complex.h>

// https://stackoverflow.com/questions/10996418/efficient-integer-compare-function
//  -1 if a < b
//   0 if a = b
//  +1 if a > b
int cmp(int a, int b){
  return (a>b)-(a<b);
}

// calculate product
double complex prod(int n, double complex *vec){
  int i;
  double complex tmp;
  tmp = vec[0];
  for(i=1; i<n; i++){
    tmp *= vec[i];
  }
  return tmp;
}

// calculate sum along 0 axis
int sum0axis(int n, double complex **mat, double complex *vec){
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
double complex perm(int n, double complex **M){
  int i;
  int old_gray;
  int new_gray;
  int gray_diff;
  int gray_diff_index;
  int num_loops;
  int bin_index;
  int sign;// takes -1,1
  int direction;// takes -2,0,+2
  double complex total;
  double complex *row_comb = (double complex*)malloc(n*sizeof(double complex));
  double complex reduced;

  if(n==0) return 1.0;
  sum0axis(n,M,row_comb);
  total = 0.0;
  old_gray = 0;
  sign = 1;
//  num_loops = 1LLU << (n-1);
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
  return creal(total / num_loops);
}


//----


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
//// MI
//  for(i=0; i<L; i++){
//    for(j=0; j<L; j++){
//// CDW
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
//  for(i=0; i<L; i++){
//    for(j=0; j<L; j++){
//// CDW
  for(i=0; i<L/2; i++){
    for(j=0; j<L/2; j++){
//      matA[i][j] = matZ[i][j];
//      matA[i][j+L] = -matZ[i][j];
//      matA[i+L][j] = -matZ[i][j];
//      matA[i+L][j+L] = matZ[i][j];
      matA[i][j] = matZ[i][j];
      matA[i][j+L/2] = -matZ[i][j];
      matA[i+L/2][j] = -matZ[i][j];
      matA[i+L/2][j+L/2] = matZ[i][j];
    }
//    matA[i][i+L] += 1.0;
//    matA[i+L][i] += 1.0;
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


//----


int main(void){
  int i;
  int Nmax;
  int L,L_A;
  double tstep;
  double tmax;
  double val;

//  L = 10;// MI
  L = 20;// CDW
  L_A = L/2;
  Nmax = 64;
  tmax = 2.0*L;

// prepare matrix
//  double complex **matA = (double complex**)malloc2d(sizeof(double complex),2*L,2*L);// MI
  double complex **matA = (double complex**)malloc2d(sizeof(double complex),L,L);// CDW
  double complex **matX = (double complex**)malloc2d(sizeof(double complex),L,L);
  double complex **matY = (double complex**)malloc2d(sizeof(double complex),L,L);
//  double complex **matZ = (double complex**)malloc2d(sizeof(double complex),L,L);// MI
  double complex **matZ = (double complex**)malloc2d(sizeof(double complex),L/2,L/2);// CDW
  double complex *vecEPS = (double complex*)malloc(sizeof(double complex)*L);

  for(i=0; i<=Nmax; i++){
    tstep = i*tmax/Nmax;
    calc_matA(matA,matX,matY,matZ,vecEPS,L,L_A,tstep);
//    print_mat(matA,L);
//    val = perm(2*L,matA);// MI
    val = perm(L,matA);// CDW
    printf("%g %g\n",tstep,-log(val));
  }

// free matrix
  free(matA);
  free(matX);
  free(matY);
  free(matZ);
  free(vecEPS);

  return 0;
}
