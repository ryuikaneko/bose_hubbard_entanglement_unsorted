#!/bin/bash

for e in \
12 13
#12 13 14 15 16 17 18 19 20
do
  for rnd in \
  `seq 1 32`
  do
    for L in \
    `seq 16 2 18`
#    `seq 16 2 40`
    do
      t=$((2*L))
      ../main -L ${L} -t ${t} -e ${e} -r $((RANDOM*RANDOM)) -s $((RANDOM*RANDOM))
    done
  done
done
