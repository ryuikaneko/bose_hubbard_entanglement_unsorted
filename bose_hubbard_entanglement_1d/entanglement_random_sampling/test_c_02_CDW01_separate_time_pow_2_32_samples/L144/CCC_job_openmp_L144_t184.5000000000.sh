#!/bin/bash
#$ -cwd
#$ -V -S /bin/bash
#$ -N EE144
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

L=144
t=184.5000000000

echo "${L} ${t}"

date
${prog} \
  -L ${L} \
  -t ${t}
date

#----
