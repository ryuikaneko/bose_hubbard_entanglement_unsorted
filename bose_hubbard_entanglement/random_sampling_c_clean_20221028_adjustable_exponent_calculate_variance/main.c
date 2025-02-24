#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
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
  uint64_t Ntotal;
  uint64_t Nblock;// for blocking analysis
  uint64_t Nboot;// for bootstrap method
  int L,L_A;
  int flag_histtotal;// if flag=1, write histtotal file
  uint64_t seed[2];
  int exponent;
  double tstep;
  double complex valj[2];// average and error after jackknife: valj[0]-->ave, valj[1]-->err
  double complex valb[2];// average and error after bootstrap: valb[0]-->ave, valb[1]-->err
  double complex valvarj[2];
  double complex valvarb[2];
  double time_count[2];

  L = 40;
  seed[0] = 42u;
  seed[1] = 54u;
  exponent = 18;
  tstep = 0.0;
  flag_histtotal = 0;// default setting: do not write histtotal file

//// prameter settings
  for(i = 0; i < argc; ++i){
    if(*argv[i] == '-'){
      opt = *(argv[i]+1);
      if(opt == 'L'){
        L = atoi(argv[i+1]);
      }else if(opt == 't'){
        tstep = atof(argv[i+1]);
      }else if(opt == 'f'){
        flag_histtotal = atoi(argv[i+1]);
      }else if(opt == 'r'){
        seed[0] = atoll(argv[i+1]);
      }else if(opt == 's'){
        seed[1] = atoll(argv[i+1]);
      }else if(opt == 'e'){
        exponent = atoi(argv[i+1]);
      }
    }
  }

  L_A = L/2;
  Ntotal = 1LLU<<exponent;
//  Ntotal = 1LLU<<18;
//  Ntotal = (uint64_t) pow(2.0,L*0.25+7.0)+0.1;// should be larger than Nblock (L>=12)
  Nblock = 1LLU<<10;
  Nboot = 1LLU<<12;
//  printf("%d\n",Ntotal);
//  printf("%llu\n",Ntotal);

//// prepare matrix
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
  double complex *histblock = (double complex*)malloc(sizeof(double complex)*Nblock);
  double complex *histvarblock = (double complex*)malloc(sizeof(double complex)*Nblock);
  double complex *tmp_block = (double complex*)malloc(sizeof(double complex)*Nboot);

//// open files
  FILE *fp_setup;
  FILE *fp_time;
  FILE *fp_renyij;
  FILE *fp_renyib;
  FILE *fp_varj;
  FILE *fp_varb;
  FILE *fp_histblock;
  FILE *fp_histblock_binary;
  char buf_setup[256];
  char buf_time[256];
  char buf_renyij[256];
  char buf_renyib[256];
  char buf_varj[256];
  char buf_varb[256];
  char buf_histblock[256];
  char buf_histblock_binary[256];
  sprintf(buf_setup,"dat_setup_L%d_t%.10f_e%d",L,tstep,exponent);
  sprintf(buf_time,"dat_time_L%d_t%.10f_e%d",L,tstep,exponent);
  sprintf(buf_renyij,"dat_renyij_L%d_t%.10f_e%d",L,tstep,exponent);
  sprintf(buf_renyib,"dat_renyib_L%d_t%.10f_e%d",L,tstep,exponent);
  sprintf(buf_varj,"dat_varj_L%d_t%.10f_e%d",L,tstep,exponent);
  sprintf(buf_varb,"dat_varb_L%d_t%.10f_e%d",L,tstep,exponent);
  sprintf(buf_histblock_binary,"dat_histblock_binary_L%d_t%.10f_e%d",L,tstep,exponent);
  sprintf(buf_histblock,"dat_histblock_L%d_t%.10f_e%d",L,tstep,exponent);
  fp_setup = fopen(buf_setup,"w");
  fp_time = fopen(buf_time,"w");
  fp_renyij = fopen(buf_renyij,"w");
  fp_renyib = fopen(buf_renyib,"w");
  fp_varj = fopen(buf_varj,"w");
  fp_varb = fopen(buf_varb,"w");
//  fp_histblock = fopen(buf_histblock,"w");
  fp_histblock_binary = fopen(buf_histblock_binary,"w");

//// output parameters
  fprintf(fp_setup,"# L,tstep,seed[0],seed[1],exponent,Ntotal,Nblock,Nblocksize\n");
  fprintf(fp_setup,"%d %.10f %llu %llu %i %llu %llu %llu\n",L,tstep,seed[0],seed[1],exponent,Ntotal,Nblock,Ntotal/Nblock);

//// set random seed
//// https://www.pcg-random.org/using-pcg-c-basic.html
  pcg32_srandom(seed[0], seed[1]);// use the global RNG
//  printf("%f\n",ldexp(pcg32_random(),-32));

  time_count[0] = gettimeofday_sec();

//// calculate entanglement
  calc_matA(matA,matX,matY,matZ,vecEPS,L,L_A,tstep);
  if(flag_histtotal==1){
    FILE *fp_histtotal_binary;
    char buf_histtotal_binary[256];
    double complex *histtotal = (double complex*)malloc(sizeof(double complex)*Ntotal);
    sprintf(buf_histtotal_binary,"dat_histtotal_binary_L%d_t%.10f_e%d",L,tstep,exponent);
    fp_histtotal_binary = fopen(buf_histtotal_binary,"w");
//    glynn_all(matA,2*L,vec,tmp,Ntotal,Nblock,histtotal,histblock);// MI
    glynn_all(matA,L,vec,tmp,Ntotal,Nblock,histtotal,histblock);// CDW
    fwrite(histtotal,sizeof(double complex),Ntotal,fp_histtotal_binary);
    fclose(fp_histtotal_binary);
    free(histtotal);
//  }else{
////    glynn(matA,2*L,vec,tmp,Ntotal,Nblock,histblock);// MI
//    glynn(matA,L,vec,tmp,Ntotal,Nblock,histblock);// CDW
  }
  glynn_var(matA,L,vec,tmp,Ntotal,Nblock,histblock,histvarblock);// CDW
//
  blocking_jackknife(Nblock,histblock,valj);
  blocking_bootstrap(Nblock,Nboot,histblock,tmp_block,valb);
  blocking_jackknife(Nblock,histvarblock,valvarj);
  blocking_bootstrap(Nblock,Nboot,histvarblock,tmp_block,valvarb);
  printf("%g %g %g\n",tstep,-log(fabs(creal(valj[0]))),-log(fabs(creal(valb[0]))));
//  for(i=0; i<Nboot; i++){
//    printf("%g\n",tmp_block[i]);
//  }

//// output data
//// print tstep, ln(perm(A)), delta(ln(perm(A))), ln(perm(A+delta)), ln(perm(A-delta))
////       perm(A).re, error, perm(A).im, error
  fprintf(fp_renyij,"%.10g %.10g %.10g %.10g %.10g %.10g %.10g %.10g %.10g %d %d\n",
    tstep,-log(fabs(creal(valj[0]))),creal(valj[1])/creal(valj[0]),
    -log(fabs(creal(valj[0]+valj[1]))),-log(fabs(creal(valj[0]-valj[1]))),
    creal(valj[0]),creal(valj[1]),cimag(valj[0]),cimag(valj[1]),
    L,exponent
    );
  fprintf(fp_renyib,"%.10g %.10g %.10g %.10g %.10g %.10g %.10g %.10g %.10g %d %d\n",
    tstep,-log(fabs(creal(valb[0]))),creal(valb[1])/creal(valb[0]),
    -log(fabs(creal(valb[0]+valb[1]))),-log(fabs(creal(valb[0]-valb[1]))),
    creal(valb[0]),creal(valb[1]),cimag(valb[0]),cimag(valb[1]),
    L,exponent
    );
  fprintf(fp_varj,"%.10g %.10g %.10g %.10g %.10g %d %d\n",
    tstep,
    creal(valvarj[0]),creal(valvarj[1]),cimag(valvarj[0]),cimag(valvarj[1]),
    L,exponent
    );
  fprintf(fp_varb,"%.10g %.10g %.10g %.10g %.10g %d %d\n",
    tstep,
    creal(valvarb[0]),creal(valvarb[1]),cimag(valvarb[0]),cimag(valvarb[1]),
    L,exponent
    );
//// output binary file
//  for(i=0; i<Nblock; i++){
//    fprintf(fp_histblock,"%13.6g %13.6g\n",creal(histblock[i]),cimag(histblock[i]));
//  }
  fwrite(histblock,sizeof(double complex),Nblock,fp_histblock_binary);

  time_count[1] = gettimeofday_sec();

  fprintf(fp_time,"%13.6f\n",time_count[1]-time_count[0]);

//// close files
  fclose(fp_setup);
  fclose(fp_time);
  fclose(fp_renyij);
  fclose(fp_renyib);
  fclose(fp_varj);
  fclose(fp_varb);
//  fclose(fp_histblock);
  fclose(fp_histblock_binary);

//// free matrix
  free(matA);
  free(matX);
  free(matY);
  free(matZ);
  free(vecEPS);
  free(vec);
  free(tmp);
  free(tmp_block);
  free(histblock);

  return 0;
}
