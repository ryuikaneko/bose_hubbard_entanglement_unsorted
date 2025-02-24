#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

int main(){
    int i;
    int N;
    N = 256;
    double complex a[N];
    FILE *fp;

    for(i=0; i<N; i++){
        a[i] = i;
        /* a[N]に値を代入 */
    }

    fp = fopen("data.bin", "w");
//    fwrite(&a, sizeof(*a), 1, fp);
    fwrite(a, sizeof(a), 1, fp);
    fclose(fp);

    return 0;
}
