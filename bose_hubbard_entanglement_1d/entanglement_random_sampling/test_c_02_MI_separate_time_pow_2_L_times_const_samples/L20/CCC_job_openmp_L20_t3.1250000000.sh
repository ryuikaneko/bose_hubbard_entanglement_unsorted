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
prog=../bin/a.out

#----

L=20
t=3.1250000000

echo "${L} ${t}"

date
${prog} \
  -L ${L} \
  -t ${t}
date

#----
