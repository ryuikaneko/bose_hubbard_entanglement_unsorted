#!/bin/bash
#$ -cwd
#$ -V -S /bin/bash
#$ -N EE8x8
#$ -pe smp 1
#$ -q dp2.q,dp3.q,dp4.q,dp5.q,dp6.q,dp7.q,dp8.q,dp9.q,dp10.q
##$ -q dp11.q
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

Lx=8
Ly=8
t=16.00000000000000000000
r=$((RANDOM*RANDOM))
s=$((RANDOM*RANDOM))
e=29
f=0
#f=1 ## output histtotal

echo "${Lx} ${Ly} ${t} ${r} ${s} ${f}"

date
${prog} \
  -x ${Lx} \
  -y ${Ly} \
  -t ${t} \
  -r ${r} \
  -s ${s} \
  -e ${e} \
  -f ${f}
date

#----
