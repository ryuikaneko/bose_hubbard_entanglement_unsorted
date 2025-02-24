#!/bin/bash

source ${HOME}/miniconda3/etc/profile.d/conda.sh ; conda activate
conda activate ml

for i in \
make_fig_*.py
do
  python ${i}
done

conda deactivate
