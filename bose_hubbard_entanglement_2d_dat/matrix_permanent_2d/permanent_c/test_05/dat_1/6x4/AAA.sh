#!/bin/bash

Lx=6
Ly=4

template=_qsub_BBB_template.sh
subprog=qsub
#subprog=sbatch

for i in \
`seq 0 200`
do
  ipad=`printf "%06d" "${i}"`
  t=`echo ${i} ${Lx} ${Ly} | awk '{printf "%.6f",0.01*$1*$2*$3}'`
  echo generate ${Lx} ${Ly} ${ipad} ${t}
  cat ${template} | sed 's/@Lx@/'${Lx}'/g' | sed 's/@Ly@/'${Ly}'/g' | sed 's/@t@/'${t}'/g' > BBB_Lx${Lx}_Ly${Ly}_i${ipad}.sh
done


for i in \
`seq 0 200`
do
  ipad=`printf "%06d" "${i}"`
  t=`echo ${i} ${Lx} ${Ly} | awk '{printf "%.6f",0.01*$1*$2*$3}'`
  echo submit ${Lx} ${Ly} ${ipad} ${t}
  ${subprog} BBB_Lx${Lx}_Ly${Ly}_i${ipad}.sh
done
