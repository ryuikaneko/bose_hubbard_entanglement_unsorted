#!/bin/bash
#$ -cwd
#$ -V -S /bin/bash
#$ -N EE
#$ -pe smp 1
#$ -q dp11.q
##$ -q dp.q
##$ -q dp2.q
##$ -q dp3.q
##$ -q dp4.q
##$ -q dp5.q
##$ -q dp6.q
##$ -q dp7.q

#----

export OMP_NUM_THREADS=1
#prog=../bin/a.out
prog=../bin/main

#----

L=16
t=0.0
r=$((RANDOM*RANDOM))
s=$((RANDOM*RANDOM))
e=12
f=0
#f=1 ## output histtotal

echo "${L} ${t} ${r} ${s} ${f}"

date
${prog} \
  -L ${L} \
  -t ${t} \
  -r ${r} \
  -s ${s} \
  -e ${e} \
  -f ${f}
date

#----
