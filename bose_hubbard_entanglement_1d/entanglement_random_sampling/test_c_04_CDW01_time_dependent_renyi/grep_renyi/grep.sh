#!/bin/bash

for L in \
20 40 60 80 100
#20 40 60
do
  output=dat_L${L}
  cat ../L${L}/dat_renyib_L*_t* | sort -g -k 1 > ${output}
done
