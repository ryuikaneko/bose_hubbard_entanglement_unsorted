#!/bin/bash
#$ -cwd
#$ -V -S /bin/bash
#$ -N EE24
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

L=24
t=37.5000000000
r=$((RANDOM*RANDOM))
s=$((RANDOM*RANDOM))
f=0
#f=1 ## output histtotal

echo "${L} ${t} ${r} ${s} ${f}"

date
${prog} \
  -L ${L} \
  -t ${t} \
  -r ${r} \
  -s ${s} \
  -f ${f}
date

#----
