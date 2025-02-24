// https://stackoverflow.com/questions/39319252/deriving-nth-gray-code-from-the-n-1th-gray-code?noredirect=1&lq=1
// https://stackoverflow.com/questions/4648716/how-do-i-find-next-bit-to-change-in-a-gray-code-in-constant-time

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/time.h>
#include <math.h>

int next_gray_code_1(int code){
  return (0xEAFD9B80574C2631ULL >> (code << 2)) & 15;
}

int next_gray_code_2(int code){
  code = code ^ (code >> 2);
  code = code ^ (code >> 1);
  code = (code + 1) & 15;
  return code ^ (code >> 1);
}

int main(void){
  int i;
  int n;

  n = 4;
  for(i=0; i<1<<n; i++){
    printf("%4d ",i);
    printf("%4d ",i^(i/2));
    if(i>0){
      printf("%4d ",next_gray_code_1((i-1)^((i-1)/2)));
      printf("%4d ",next_gray_code_2((i-1)^((i-1)/2)));
    }
    printf("\n");
  }

  return 0;
}
