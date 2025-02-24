#!/bin/bash

Lx=4
Ly=1

for t in \
`seq -f"%.1f" 0.0 0.1 80.01`
do
  echo ${t}
  ../main -x ${Lx} -y ${Ly} -t ${t}
done
