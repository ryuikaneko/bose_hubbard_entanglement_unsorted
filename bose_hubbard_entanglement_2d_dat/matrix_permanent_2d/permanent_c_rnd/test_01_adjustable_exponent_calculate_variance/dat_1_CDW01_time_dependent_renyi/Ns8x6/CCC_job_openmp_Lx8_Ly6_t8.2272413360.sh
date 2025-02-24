#!/bin/bash
#$ -cwd
#$ -V -S /bin/bash
#$ -N EE8x6
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
Ly=6
t=8.2272413360
r=$((RANDOM*RANDOM))
s=$((RANDOM*RANDOM))
f=0
#f=1 ## output histtotal
e=`echo ${Lx} ${Ly} | awk '{printf"%.0f",sqrt($1*$2)*0.2+12}'`

echo "${Lx} ${Ly} ${t} ${r} ${s} ${f} ${e}"

date
${prog} \
  -x ${Lx} \
  -y ${Ly} \
  -t ${t} \
  -r ${r} \
  -s ${s} \
  -f ${f} \
  -e ${e}
date

#----
