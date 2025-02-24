#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/time.h>
#include <math.h>
#include <complex.h>
#include "sub_util.h"
#include "sub_calmat.h"
#include "sub_perm.h"

int main(int argc, char *argv[]){
  int i;
  char opt;
  int Ns,N_A;
  int Lx,Ly;
  int Nhbond[1];
  int num[1];
  double tstep;
  double complex val;
  double time_count[2];

  tstep = 0.0;

  Lx = 2;
  Ly = 2;

//// prameter settings
  for(i = 0; i < argc; ++i){
    if(*argv[i] == '-'){
      opt = *(argv[i]+1);
      if(opt == 'x'){
        Lx = atoi(argv[i+1]);
      }else if(opt == 'y'){
        Ly = atoi(argv[i+1]);
      }else if(opt == 't'){
        tstep = atof(argv[i+1]);
      }
    }
  }

  Ns = Lx*Ly;
  N_A = Ns/2;
//  Nhbond[0] = 2*Lx*Ly-Lx-Ly;
//  num[0] = Lx*Ly/2

//// prepare matrix
  int *i1s = (int*)malloc(sizeof(int)*(2*Lx*Ly-Lx-Ly));
  int *i2s = (int*)malloc(sizeof(int)*(2*Lx*Ly-Lx-Ly));
  double *Js = (double*)malloc(sizeof(double)*(2*Lx*Ly-Lx-Ly));
  int *init_confs = (int*)malloc(sizeof(int)*(Lx*Ly));
  int *init_is = (int*)malloc(sizeof(int)*(Lx*Ly/2));// CDW
  double *matH = (double*)malloc(sizeof(double)*Ns*Ns);
  double *work = (double*)malloc(sizeof(double)*Ns*4);
  double complex **matA = (double complex**)malloc2d(sizeof(double complex),Ns,Ns);// CDW
  double **matX = (double**)malloc2d(sizeof(double),Ns,Ns);
  double complex **matY = (double complex**)malloc2d(sizeof(double complex),Ns,Ns);
  double complex **matZ = (double complex**)malloc2d(sizeof(double complex),Ns/2,Ns/2);// CDW
  double *vecEPS = (double*)malloc(sizeof(double)*Ns);

//// open files
  FILE *fp_setup;
  FILE *fp_time;
  FILE *fp_renyi;
  char buf_setup[256];
  char buf_time[256];
  char buf_renyi[256];
  sprintf(buf_setup,"dat_setup_Lx%d_Ly%d_t%.10f",Lx,Ly,tstep);
  sprintf(buf_time,"dat_time_Lx%d_Ly%d_t%.10f",Lx,Ly,tstep);
  sprintf(buf_renyi,"dat_renyi_Lx%d_Ly%d_t%.10f",Lx,Ly,tstep);
  fp_setup = fopen(buf_setup,"w");
  fp_time = fopen(buf_time,"w");
  fp_renyi = fopen(buf_renyi,"w");

//// output parameters
  fprintf(fp_setup,"# Lx,Ly,tstep\n");
  fprintf(fp_setup,"%d %d %.10f\n",Lx,Ly,tstep);

  time_count[0] = gettimeofday_sec();

//// calculate entanglement
  make_sites(Lx,Ly,i1s,i2s,Js,Nhbond);
  make_init_CDW(Lx,Ly,init_confs,init_is,num);
  make_ham(Ns,Nhbond,i1s,i2s,Js,matH);
  linalg_eigh(Ns,matH,vecEPS,work,matX);

  calc_matA(matA,matX,matY,matZ,vecEPS,init_is,num,Ns,N_A,tstep);
  val = perm(Ns,matA);// CDW
  printf("%g %g %g %g\n",tstep,-log(fabs(creal(val))),creal(val),cimag(val));

//// output data
//// print tstep, -ln(perm(A)), perm(A).re, perm(A).im
  fprintf(fp_renyi,"%.10g %.10g %.10g %.10g\n",
    tstep,-log(fabs(creal(val))),creal(val),cimag(val)
    );

  time_count[1] = gettimeofday_sec();

  fprintf(fp_time,"%13.6f\n",time_count[1]-time_count[0]);

//// close files
  fclose(fp_setup);
  fclose(fp_time);
  fclose(fp_renyi);

//// free matrix
  free(i1s);
  free(i2s);
  free(Js);
  free(init_confs);
  free(init_is);
  free(matH);
  free(work);

  free(matA);
  free(matX);
  free(matY);
  free(matZ);
  free(vecEPS);

  return 0;
}
