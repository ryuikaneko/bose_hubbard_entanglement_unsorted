// https://auewe.hatenablog.com/entry/2013/05/04/075717

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define SIZE 2

int main(void) {
  int     n             = SIZE;// 正方行列Aの次元
  int     nrhs          = 1   ;// Ax=b の縦ベクトルbの列数
  double  A[SIZE*SIZE]        ;// Ax=bの正方行列A
  A[0]=1; A[2]=2;
  A[1]=3; A[3]=4;
  int     lda           = SIZE;// 普通はSIZEにする
  int     ipiv[SIZE]          ;// LU分解の交換行列のピポット要素。[SIZE]にしておく
  double  b[SIZE]             ;// Ax=bの縦ベクトルb。計算終了後はxベクトルが入る。
  b[0]=5;
  b[1]=6;
  int     ldb           = SIZE;// 普通はSIZEにする
  int     info                ;// 0なら正常終了

  dgesv_(&n,&nrhs,A,&lda,ipiv,b,&ldb,&info);

  printf("x = %5.3lf\n",b[0]);
  printf("y = %5.3lf\n",b[1]);
  return 0;
}
