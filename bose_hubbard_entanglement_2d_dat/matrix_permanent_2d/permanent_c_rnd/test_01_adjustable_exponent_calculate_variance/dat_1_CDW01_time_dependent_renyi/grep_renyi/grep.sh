#!/bin/bash

declare -a arrLx=(4 6 6 8 8 10 10 12 12)
declare -a arrLy=(4 4 6 6 8 8  10 10 12)

for i in \
`seq 0 8`
#`seq 0 4`
do

Lx=${arrLx[${i}]}
Ly=${arrLy[${i}]}

output=dat_Ns${Lx}x${Ly}
cat ../Ns${Lx}x${Ly}/dat_renyib_L*_t* | sort -g -k 1 > ${output}

done

