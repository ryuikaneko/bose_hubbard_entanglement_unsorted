#!/bin/bash

nmax=64
#nmax=48

for e in \
12
#12 13 14 15 16 17 18 19 20
do

for L in \
16 20 24 28 32 36 40 44 48 52 56 60
#16
do

for n in \
${nmax}
#`seq 1 $((nmax))`
do
  t=`echo ${L} ${n} ${nmax} | awk '{printf("%.10f",2.0*$1*$2/$3)}'`
#  t=`echo ${L} ${n} ${nmax} | awk '{printf("%.10f",1.5*$1*$2/$3)}'`
  echo ${L} ${n} ${t}
  sed 's/^t=.*/t='${t}'/g' _CCC_job_openmp.sh | \
  sed 's/^L=.*/L='${L}'/g' | \
  sed 's/^e=.*/e='${e}'/g' | \
  sed 's/^#$ -N EE/#$ -N EE'${L}'/g' \
  > CCC_job_openmp_L${L}_t${t}_e${e}.sh
#
for rnd in \
`seq 1 32`
do
  qsub CCC_job_openmp_L${L}_t${t}_e${e}.sh
done

done

done

done
