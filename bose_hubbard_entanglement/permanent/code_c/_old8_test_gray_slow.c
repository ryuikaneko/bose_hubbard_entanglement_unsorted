#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/time.h>
#include <math.h>

int gray(int n, int *a){
  int i,j;
  int loops;
  loops = 0;
  a[0] = 1-a[0];
  j = 1;
  while(1-a[j-1]==1){
    j = j+1;
    loops++;
  };
  j = j+1;
  a[j-1] = 1-a[j-1];
  return loops;
}

int main(void){
  int i;
  int j;
  int n;
  double loops_ave;

  loops_ave = 0.0;

//  n = 4;
  n = 12;
  int *a = (int*)malloc(n*sizeof(int));
  for(i=0; i<n; i++){
    a[i] = 0;
  }

  for(i=1; i<(1<<(n-2))+1; i++){
    printf("%4d ",i);
    printf("%4d ",i^(i/2));
    j = gray(4,a);
    printf("%4d ",j);
    printf("\n");
    loops_ave += 1.0*j;
  }

  printf("%f\n",loops_ave/(1<<(n-2)));

  free(a);
  return 0;
}
