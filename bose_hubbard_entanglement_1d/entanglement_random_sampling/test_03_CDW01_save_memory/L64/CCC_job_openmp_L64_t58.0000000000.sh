#!/bin/bash
#$ -cwd
#$ -V -S /bin/bash
#$ -N EE64
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
source ${HOME}/miniconda3/etc/profile.d/conda.sh
conda activate quspin_no_omp
python=python
#prog=../bin/test_b_001.py
prog=../bin/test_b_002_save_memory.py

#----

L=64
t=58.0000000000

echo "${L} ${t}"

date
${python} ${prog} \
  -L ${L} \
  -t ${t} \
  > dat_log_L${L}_t${t}
  date

#----

conda deactivate
