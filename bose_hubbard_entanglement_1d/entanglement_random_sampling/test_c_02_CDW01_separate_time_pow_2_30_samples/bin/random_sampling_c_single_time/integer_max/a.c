#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

int main(){
  printf("1<<30:%d\n",1<<30);
  printf("1073741824:%d\n",1073741824);
  printf("2147483647:%d\n",2147483647);
  printf("1<<31:%d\n",1<<31);
  printf("2147483648:%d\n",2147483648);
  printf("\n");
  printf("1LLU<<31:%llu\n",1LLU<<31);
  printf("1LLU<<32:%llu\n",1LLU<<32);
  printf("1LLU<<33:%llu\n",1LLU<<33);
  return 0;
}
