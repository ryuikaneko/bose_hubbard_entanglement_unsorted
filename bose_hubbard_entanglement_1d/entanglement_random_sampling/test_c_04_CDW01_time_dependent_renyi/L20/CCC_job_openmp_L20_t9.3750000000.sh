#!/bin/bash
#$ -cwd
#$ -V -S /bin/bash
#$ -N EE20
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

L=20
t=9.3750000000
r=$((RANDOM*RANDOM))
s=$((RANDOM*RANDOM))
f=0
#f=1 ## output histtotal
e=`echo ${L} | awk '{printf"%.0f",$1*0.2+12}'`

echo "${L} ${t} ${r} ${s} ${f} ${e}"

date
${prog} \
  -L ${L} \
  -t ${t} \
  -r ${r} \
  -s ${s} \
  -f ${f} \
  -e ${e}
date

#----
