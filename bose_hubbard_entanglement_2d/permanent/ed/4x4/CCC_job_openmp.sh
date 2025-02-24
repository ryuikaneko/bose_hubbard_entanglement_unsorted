#!/bin/bash
#$ -cwd
#$ -V -S /bin/bash
#$ -N test_quench
##$ -N test
#$ -pe smp 1
#$ -q dp10.q
##$ -q dp.q,dp2.q,dp3.q,dp4.q,dp5.q,dp6.q,dp7.q,dp8.q,dp9.q
##$ -q dp.q,dp3.q,dp4.q,dp5.q,dp6.q,dp7.q,dp8.q,dp9.q,dp10.q
##$ -q dp.q,dp2.q,dp3.q,dp4.q,dp5.q,dp6.q,dp7.q,dp8.q,dp9.q,dp10.q
##$ -q dp.q
##$ -q dp2.q
##$ -q dp3.q
##$ -q dp4.q
##$ -q dp5.q
##$ -q dp6.q
##$ -q dp7.q

#----

export OMP_NUM_THREADS=1
source ${HOME}/miniconda3/etc/profile.d/conda.sh
conda activate quspin_no_omp
python=python
prog=square_bose_hubbard.py

#----

date
${python} ${prog} \
 > dat
date

#----

conda deactivate
