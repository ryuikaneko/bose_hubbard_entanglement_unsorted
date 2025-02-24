#!/bin/bash

#SBATCH --get-user-env
#SBATCH -p batch
#SBATCH -J test_ee
#SBATCH -n 1         #num of total mpi processes
#SBATCH -c 1         #num of total mpi processes
##SBATCH -t 00:02:00
##SBATCH -o o

#export KMP_AFFINITY=scatter
#export KMP_AFFINITY=compact

#----

date

Lx=@Lx@
Ly=@Ly@
t=@t@

echo ${Lx} ${Ly} ${t}

../bin/main -x ${Lx} -y ${Ly} -t ${t}

date
