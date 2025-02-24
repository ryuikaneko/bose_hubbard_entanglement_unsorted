#!/bin/bash

Lx=2
Ly=2

template=_qsub_BBB_template.sh
subprog=qsub
#subprog=sbatch

for t in \
`seq -f%.6f 0.0 0.1 20.0`
do
  echo generate ${Lx} ${Ly} ${t}
  cat ${template} | sed 's/@Lx@/'${Lx}'/g' | sed 's/@Ly@/'${Ly}'/g' | sed 's/@t@/'${t}'/g' > BBB_Lx${Lx}_Ly${Ly}_t${t}.sh
done


for t in \
`seq -f%.6f 0.0 0.1 20.0`
do
  echo submit ${Lx} ${Ly} ${t}
  ${subprog} BBB_Lx${Lx}_Ly${Ly}_t${t}.sh
done
