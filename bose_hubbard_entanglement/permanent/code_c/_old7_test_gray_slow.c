#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/time.h>
#include <math.h>

int gray(int n, int *a){
  int i,j;
  a[0] = 1-a[0];
  j = 0;
    printf("\n");
  do{
    printf("j %d\n",j);
    j = j+1;
  }while(1-a[j-1]==1);
//  }while(a[j]==1 && j<n-2);
//  }while(a[j]==1 && j<n-2);
//  }while(a[j]==1);
//  }while(1-a[j]==1);
//  }while(1-a[j-1] && j<n-1);
//  }while(1-a[j-1] && j<n);
  j = j+1;
  a[j-1] = 1-a[j-1];
  return j;
}

int main(void){
  int i;
  int j;
  int n;

  n = 4;
  int *a = (int*)malloc(n*sizeof(int));
  for(i=0; i<n; i++){
    a[i] = 0;
  }

    for(j=0; j<n; j++){
      printf("%d ",a[j]);
    }
  for(i=1; i<(1<<(n-2))+1; i++){
    printf("\n");
    printf("%4d ",i);
    printf("%4d ",i^(i/2));
    printf("%4d ",gray(4,a));
    printf("\n");
    for(j=0; j<n; j++){
      printf("%d ",a[j]);
    }
    printf("\n");
  }

  free(a);
  return 0;
}
