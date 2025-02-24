#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// https://ist.ksc.kwansei.ac.jp/~nishitani/?dsyev
int dsyev_(char *jobz, char *uplo, int *n, double *a,
  int *lda, double *w, double *work, int *lwork, int *info);

// http://www.fmaj7b5.info/wiki/index.php?title=Lapacke%E3%82%A4%E3%83%B3%E3%82%B9%E3%83%88%E3%83%BC%E3%83%AB%E3%83%A1%E3%83%A2
//
// $ ./hoge
// a:
//          1          0          0
//          0          1          1
//          0          1          1
//
// eigen vectors:
//          0          1          0
//     -0.707          0      0.707
//      0.707          0      0.707
//
// eigen values:
//          0          1          2
//
//----
//
// taharaVMC, hartree-fock code
// /Users/rk/Prog_github/private_prog_local_pc/prog_dropbox.tgz:Users/kaneko/Dropbox/MyWork/prog/HartreeFock/prog111204_fra_HartreeFock/Hartree_150123_mod_cor_output_eig
//
// /* obtain eigen vectors */
// int     DSEVvector
// (
//         int     xNsize,
//         double  **A,
//         double  *r,
//         double  **vec
// )
//
int main(){
  int i,j,k;
  char jobz,uplo;
  int n,lda,lwork,info;
  double *a,*w,*work;

  n = 3;
  lda = n;
  lwork = 4*n; // LWORK >= max(1,3*N-1), best: LWORK >= (NB+2)*N
//  jobz = 'N';// ene only
  jobz = 'V';// ene and vec
  uplo = 'U';
  a = (double*)malloc(n*n*sizeof(double));
  w = (double*)malloc(n*sizeof(double));
  work = (double*)malloc(lwork*sizeof(double));

// make matrix
  for (i = 0; i < n; ++i) {
    for (j = 0; j < n; ++j) {
      a[lda*i + j] = (i == j || i*j != 0) ? 1 : 0;
    }
  }

// print matrix
  printf("mat\n");
  for (i = 0; i < n; ++i) {
    for (j = 0; j < n; ++j) {
      printf("%+f ", a[j*lda + i]);
    }
    printf("\n");
  }
  printf("\n");

  dsyev_(&jobz,&uplo,&n,a,&lda,w,work,&lwork,&info);

  if(info != 0){
    printf("info %d",info);
    free(a);
    free(w);
    free(work);
    return 0;
  }

  printf("ene\n");
  for(i=0;i<n;i++){
    printf("%+f ",w[i]);
  }
  printf("\n");
  printf("\n");

  printf("vec\n");
  for(i=0;i<n;i++){
    for(j=0;j<n;j++){
//// taharaVMC code
//      k = n*i+j;
//      printf("%+f ",a[k]);
////----
//// fortran-like
////      vec[i][j] = a[k];
      k = i+n*j;
      printf("%+f ",a[k]);
//      vec[i][j] = a[k];
    }
    printf("\n");
  }
  printf("\n");

  return 0;
}
