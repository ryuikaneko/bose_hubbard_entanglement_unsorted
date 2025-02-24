#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <complex.h>
#include "./rnd/pcg_basic.h"
#include "sub_util.h"
#include "sub_calmat.h"
#include "sub_glynn.h"

int main(int argc, char *argv[]){
  int i;
  char opt;
//  int step;
  uint64_t Nsmp;
//  int Nmax;
  int L,L_A;
//  int seed;
  uint64_t seed[2];
  double tstep;
//  double tmax;
  double val[2];
  double time_count[2];
  FILE *fp_time;
  FILE *fp_renyi;
  FILE *fp_hist;
  FILE *fp_hist_binary;
  char buf_time[256];
  char buf_renyi[256];
  char buf_hist[256];
  char buf_hist_binary[256];

  L = 40;
//  L = 48;
//  L = 60;
//  L = 72;
//  Nmax = 64;
//  tmax = 2.0*L;
//  seed = 12345;
  seed[0] = 42u;
  seed[1] = 54u;
//  srand48(seed);
  tstep = 0.0;

// prameter settings
  for(i = 0; i < argc; ++i){
    if(*argv[i] == '-'){
      opt = *(argv[i]+1);
      if(opt == 'L'){
        L = atoi(argv[i+1]);
      }else if(opt == 't'){
        tstep = atof(argv[i+1]);
      }
    }
  }

  L_A = L/2;
//  Nsmp = 1LLU<<18;
  Nsmp = (uint64_t) pow(2.0,L*0.25+7.0);
//  printf("%d\n",Nsmp);
//  printf("%llu\n",Nsmp);

// prepare matrix
//  double complex **matA = (double complex**)malloc2d(sizeof(double complex),2*L,2*L);// MI
  double complex **matA = (double complex**)malloc2d(sizeof(double complex),L,L);// CDW
  double complex **matX = (double complex**)malloc2d(sizeof(double complex),L,L);
  double complex **matY = (double complex**)malloc2d(sizeof(double complex),L,L);
//  double complex **matZ = (double complex**)malloc2d(sizeof(double complex),L,L);// MI
  double complex **matZ = (double complex**)malloc2d(sizeof(double complex),L/2,L/2);// CDW
  double complex *vecEPS = (double complex*)malloc(sizeof(double complex)*L);
//  double complex *vec = (double complex*)malloc(sizeof(double complex)*2*L);// MI
  double complex *vec = (double complex*)malloc(sizeof(double complex)*L);// CDW
//  double complex *tmp = (double complex*)malloc(sizeof(double complex)*2*L);// MI
  double complex *tmp = (double complex*)malloc(sizeof(double complex)*L);// CDW
  double complex *hist = (double complex*)malloc(sizeof(double complex)*Nsmp);

//  sprintf(buf_time,"dat_time_L%d",L);
//  sprintf(buf_renyi,"dat_renyi_L%d",L);
  sprintf(buf_time,"dat_time_L%d_t%.10f",L,tstep);
  sprintf(buf_renyi,"dat_renyi_L%d_t%.10f",L,tstep);
  sprintf(buf_hist,"dat_hist_L%d_t%.10f",L,tstep);
  sprintf(buf_hist_binary,"dat_hist_binary_L%d_t%.10f",L,tstep);
  fp_time = fopen(buf_time,"w");
  fp_renyi = fopen(buf_renyi,"w");
  fp_hist = fopen(buf_hist,"w");
  fp_hist_binary = fopen(buf_hist_binary,"w");

//// https://www.pcg-random.org/using-pcg-c-basic.html
  pcg32_random_t rng;
//  pcg32_srandom_r(&rng, 42u, 54u);
  pcg32_srandom_r(&rng, seed[0], seed[1]);
//  printf("%f\n",ldexp(pcg32_random_r(&rng),-32));

  time_count[0] = gettimeofday_sec();

//  for(step=0; step<=Nmax; step++){
//    tstep = step*tmax/Nmax;
    calc_matA(matA,matX,matY,matZ,vecEPS,L,L_A,tstep);
//    glynn(matA,2*L,vec,tmp,Nsmp,rng,val,hist);// MI
    glynn(matA,L,vec,tmp,Nsmp,rng,val,hist);// CDW
    printf("%g %g\n",tstep,-log(fabs(val[0])));
    fprintf(fp_renyi,"%13.6f %13.6f %13.6f %13.6f\n",
      tstep,-log(fabs(val[0])),-log(fabs(val[0]+val[1])),-log(fabs(val[0]-val[1])));
//  }

  for(i=0; i<Nsmp; i++){
    fprintf(fp_hist,"%13.6g %13.6g\n",creal(hist[i]),cimag(hist[i]));
//    fwrite(&hist[i],sizeof(double complex),1,fp_hist_binary);
  }
//  printf("%zu %zu\n",sizeof(hist),sizeof(hist[0]));
  fwrite(hist,sizeof(hist[0]),Nsmp,fp_hist_binary);

  time_count[1] = gettimeofday_sec();

  fprintf(fp_time,"%13.6f\n",time_count[1]-time_count[0]);

  fclose(fp_time);
  fclose(fp_renyi);
  fclose(fp_hist);
  fclose(fp_hist_binary);

// free matrix
  free(matA);
  free(matX);
  free(matY);
  free(matZ);
  free(vecEPS);
  free(vec);
  free(tmp);
  free(hist);

  return 0;
}

