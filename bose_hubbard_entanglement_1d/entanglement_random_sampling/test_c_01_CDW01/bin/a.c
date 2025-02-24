#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <complex.h>
#include "./rnd/pcg_basic.h"

#ifndef M_PI
  #define M_PI 3.14159265358979323846
#endif

// get time
double gettimeofday_sec()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + (double)tv.tv_usec*1e-6;
}

// memory allocation
void *malloc2d(size_t size, int n1, int n2){
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

// calculate matrix
int calc_matA(double complex **matA, double complex **matX,
  double complex **matY, double complex **matZ, double complex *vecEPS,
  int L, int L_A, double tstep){
  int i,j,k;
  for(i=0; i<L; i++){
    for(j=0; j<L; j++){
      matX[i][j] = sin((i+1.0)*(j+1.0)*M_PI/(L+1.0)) * sqrt(2.0/(L+1.0));
    }
  }
  for(i=0; i<L; i++){
    vecEPS[i] = -2.0*cos((i+1.0)*M_PI/(L+1.0));
  }
  for(i=0; i<L; i++){
    for(j=0; j<L; j++){
      matY[i][j] = 0.0;
    }
  }
  for(i=0; i<L; i++){
    for(j=0; j<L; j++){
      for(k=0; k<L; k++){
        matY[i][j] += cexp(-I*vecEPS[k]*tstep) * conj(matX[k][i]) * matX[k][j];
      }
    }
  }
  for(i=0; i<L/2; i++){
    for(j=0; j<L/2; j++){
      matZ[i][j] = 0.0;
    }
  }
//// MI
//  for(i=0; i<L; i++){
//    for(j=0; j<L; j++){
//// CDW
  for(i=0; i<L/2; i++){
    for(j=0; j<L/2; j++){
      for(k=0; k<L_A; k++){
//        matZ[i][j] += conj(matY[i][k]) * matY[j][k];// MI
        matZ[i][j] += conj(matY[2*i][k]) * matY[2*j][k];// CDW
      }
    }
  }
//// MI
//  for(i=0; i<2; i++){
//    for(j=0; j<2; j++){
//// CDW
  for(i=0; i<L/2; i++){
    for(j=0; j<L/2; j++){
      matA[i][j] = matZ[i][j];
      matA[i][j+L/2] = -matZ[i][j];
      matA[i+L/2][j] = -matZ[i][j];
      matA[i+L/2][j+L/2] = matZ[i][j];
    }
    matA[i][i+L/2] += 1.0;
    matA[i+L/2][i] += 1.0;
  }
  return 0;
}

// print matrix
int print_mat(double complex **matA, int L){
  int i,j;
  for(i=0; i<L; i++){
    for(j=0; j<L; j++){
      printf("%+.6f%+.6fI ",creal(matA[i][j]),cimag(matA[i][j]));
    }
    printf("\n");
  }
  printf("\n");
  return 0;
}

// print vector
int print_vec(double complex *vecA, int L){
  int i;
  for(i=0; i<L; i++){
    printf("%+.6f%+.6fI ",creal(vecA[i]),cimag(vecA[i]));
  }
  printf("\n\n");
  return 0;
}

int glynn(double complex **matA, int L,
  double complex *vec, double complex *tmp, int Nsmp,
  pcg32_random_t rng,
  double *val){
  int i,j,k;
  double rnd;
  double complex tmp_perm;
  val[0] = 0.0;
  val[1] = 0.0;
  for(i=0; i<Nsmp; i++){
    for(j=0; j<L; j++){
//      rnd = M_PI * (2.0*drand48()-1.0);
      rnd = M_PI * (2.0 * ldexp(pcg32_random_r(&rng),-32) - 1.0);
      vec[j] = cexp(I*rnd);
    }
    for(j=0; j<L; j++){
      tmp[j] = 0.0;
    }
    for(j=0; j<L; j++){
      for(k=0; k<L; k++){
        tmp[j] += matA[j][k] * vec[k];
      }
    }
    tmp_perm = 1.0;
    for(j=0; j<L; j++){
      tmp_perm *= conj(vec[j]) * tmp[j];
    }
    tmp_perm = creal(tmp_perm);
    val[0] += tmp_perm;
    val[1] += tmp_perm * tmp_perm;
  }
  val[0] /= Nsmp;
  val[1] /= Nsmp;
  val[1] = sqrt(fabs(val[1] - val[0]*val[0])/Nsmp);
  return 0;
}


int main(void){
  int step;
  int Nsmp;
  int Nmax;
  int L,L_A;
//  int seed;
  uint64_t seed[2];
  double tstep;
  double tmax;
  double val[2];
  double time_count[2];
  FILE *fp_time;
  FILE *fp_renyi;
  char buf_time[256];
  char buf_renyi[256];

//  L = 48;
//  L = 60;
//  L = 72;
//  L = 84;
  L = 96;
  L_A = L/2;
  Nmax = 64;
  tmax = 2.0*L;
//  seed = 12345;
  seed[0] = 42u;
  seed[1] = 54u;
//  srand48(seed);
  Nsmp = 1<<((int)(L/4)+4);
//  printf("%d\n",Nsmp);

// prepare matrix
  double complex **matA = (double complex**)malloc2d(sizeof(double complex),L,L);
  double complex **matX = (double complex**)malloc2d(sizeof(double complex),L,L);
  double complex **matY = (double complex**)malloc2d(sizeof(double complex),L,L);
//  double complex **matZ = (double complex**)malloc2d(sizeof(double complex),L,L);// MI
  double complex **matZ = (double complex**)malloc2d(sizeof(double complex),L/2,L/2);// CDW
  double complex *vecEPS = (double complex*)malloc(sizeof(double complex)*L);
  double complex *vec = (double complex*)malloc(sizeof(double complex)*L);
  double complex *tmp = (double complex*)malloc(sizeof(double complex)*L);

  sprintf(buf_time,"dat_time_L%d",L);
  sprintf(buf_renyi,"dat_renyi_L%d",L);
  fp_time = fopen(buf_time,"w");
  fp_renyi = fopen(buf_renyi,"w");

//// https://www.pcg-random.org/using-pcg-c-basic.html
  pcg32_random_t rng;
//  pcg32_srandom_r(&rng, 42u, 54u);
  pcg32_srandom_r(&rng, seed[0], seed[1]);
//  printf("%f\n",ldexp(pcg32_random_r(&rng),-32));

  time_count[0] = gettimeofday_sec();

  for(step=0; step<=Nmax; step++){
    tstep = step*tmax/Nmax;
    calc_matA(matA,matX,matY,matZ,vecEPS,L,L_A,tstep);
//    glynn(matA,L,vec,tmp,Nsmp,val);
    glynn(matA,L,vec,tmp,Nsmp,rng,val);
    printf("%g %g\n",tstep,-log(fabs(val[0])));
    fprintf(fp_renyi,"%13.6f %13.6f %13.6f %13.6f\n",
      tstep,-log(fabs(val[0])),-log(fabs(val[0]+val[1])),-log(fabs(val[0]-val[1])));
  }

  time_count[1] = gettimeofday_sec();

  fprintf(fp_time,"%13.6f\n",time_count[1]-time_count[0]);

  fclose(fp_time);
  fclose(fp_renyi);

// free matrix
  free(matA);
  free(matX);
  free(matY);
  free(matZ);
  free(vecEPS);
  free(vec);
  free(tmp);

  return 0;
}

