#!/bin/bash

for L in \
`seq 16 2 40`
#20 40 60
do
  t=$((2*L))
  for e in \
  12 13 14 15 16 17 18 19 20
  do
    ../main -L ${L} -t ${t} -e ${e}
  done
done
