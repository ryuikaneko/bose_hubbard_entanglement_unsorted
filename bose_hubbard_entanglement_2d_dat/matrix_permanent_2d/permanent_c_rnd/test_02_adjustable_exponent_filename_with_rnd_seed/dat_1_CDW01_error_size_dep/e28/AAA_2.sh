#!/bin/bash

declare -a arrLx=(10 10 12 12)
declare -a arrLy=(8  10 10 12)


for e in \
28
#12 13 14 15 16 17 18 19 20
do

for i in \
`seq 0 3`
do

Lx=${arrLx[${i}]}
Ly=${arrLy[${i}]}
t=`echo ${Lx} ${Ly} | awk '{printf("%.20f",2.0*sqrt($1*$2))}'`
echo ${Lx} ${Ly} ${t}
sed 's/^t=.*/t='${t}'/g' _CCC_job_openmp.sh | \
sed 's/^Lx=.*/Lx='${Lx}'/g' | \
sed 's/^Ly=.*/Ly='${Ly}'/g' | \
sed 's/^e=.*/e='${e}'/g' | \
sed 's/^#$ -N EE/#$ -N EE'${Lx}'x'${Ly}'/g' \
> CCC_job_openmp_Lx${Lx}_Ly${Ly}_t${t}_e${e}.sh

for rnd in \
`seq 1 32`
do
  qsub CCC_job_openmp_Lx${Lx}_Ly${Ly}_t${t}_e${e}.sh
done

done

done
