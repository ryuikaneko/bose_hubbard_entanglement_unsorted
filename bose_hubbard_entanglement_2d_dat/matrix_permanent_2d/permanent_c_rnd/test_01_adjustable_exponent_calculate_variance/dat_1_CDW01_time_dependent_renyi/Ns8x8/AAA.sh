#!/bin/bash

nmax=64

Lx=8
Ly=8

for n in \
`seq 1 $((nmax))`
do
  t=`echo ${Lx} ${Ly} ${n} ${nmax} | awk '{printf("%.10f",2.0*sqrt($1*$2)*$3/$4)}'`
  echo ${Lx} ${Ly} ${n} ${t}
  sed 's/^t=.*/t='${t}'/g' _CCC_job_openmp.sh | \
  sed 's/^Lx=.*/Lx='${Lx}'/g' | \
  sed 's/^Ly=.*/Ly='${Ly}'/g' | \
  sed 's/^#$ -N EE/#$ -N EE'${Lx}'x'${Ly}'/g' \
  > CCC_job_openmp_Lx${Lx}_Ly${Ly}_t${t}.sh
#
  qsub CCC_job_openmp_Lx${Lx}_Ly${Ly}_t${t}.sh
done
