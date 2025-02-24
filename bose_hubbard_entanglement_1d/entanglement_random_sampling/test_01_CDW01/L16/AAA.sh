#!/bin/bash

nmax=64

for L in \
16
do

for n in \
`seq 1 $((nmax))`
do
  t=`echo ${L} ${n} ${nmax} | awk '{printf("%.10f",2.0*$1*$2/$3)}'`
  echo ${L} ${n} ${t}
  sed 's/^t=.*/t='${t}'/g' _CCC_job_openmp.sh | \
  sed 's/^L=.*/L='${L}'/g' | \
  sed 's/^#$ -N EE/#$ -N EE'${L}'/g' \
  > CCC_job_openmp_L${L}_t${t}.sh
#
  qsub CCC_job_openmp_L${L}_t${t}.sh
done

done
