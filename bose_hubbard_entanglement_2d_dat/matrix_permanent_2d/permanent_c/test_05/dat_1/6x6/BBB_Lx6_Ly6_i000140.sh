#!/bin/bash
#$ -cwd
#$ -V -S /bin/bash
#$ -N test_ee
#$ -pe smp 1
#$ -q dp2.q,dp3.q,dp4.q,dp5.q,dp6.q,dp7.q,dp8.q,dp9.q,dp10.q
##$ -q dp.q
##$ -q dp2.q
##$ -q dp3.q
##$ -q dp4.q
##$ -q dp5.q
##$ -q dp6.q
##$ -q dp7.q
##$ -q dp8.q
##$ -q dp9.q
##$ -q dp10.q

#----

date

Lx=6
Ly=6
t=50.400000

echo ${Lx} ${Ly} ${t}

../bin/main -x ${Lx} -y ${Ly} -t ${t}

date
