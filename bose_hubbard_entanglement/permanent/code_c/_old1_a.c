#include <stdio.h>
#include <stdlib.h>

void *malloc2d(size_t size, int n1, int n2)
{
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

int main(void)
{
  int i,j;
  int n1=4,n2=4;

  double **f = (double**)malloc2d(sizeof(double),n1,n2);

//  printf("f:\n");
  for(i=0; i<n1; i++){
    for(j=0; j<n2; j++){
      f[i][j] = i*n2+j+1.0;
    }
  }
  for(i=0; i<n1; i++){
    for(j=0; j<n2; j++){
//      printf("%3d %3d %p %f\n",i,j,&f[i][j],f[i][j]);// clang
//      printf("%3d %3d %u %f\n",i,j,&f[i][j],f[i][j]);// gcc
      printf("%3d %3d %f\n",i,j,f[i][j]);// clang
    }
  }
  printf("\n");

  free(f);
  return 0;
}

