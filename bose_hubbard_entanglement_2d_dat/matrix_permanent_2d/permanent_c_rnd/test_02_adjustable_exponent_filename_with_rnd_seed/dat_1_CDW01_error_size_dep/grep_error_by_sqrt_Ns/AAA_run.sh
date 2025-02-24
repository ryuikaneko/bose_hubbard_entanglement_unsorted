#!/bin/bash

declare -a arrLx=(4 6 6 8 8 10 10 12 12)
declare -a arrLy=(4 4 6 6 8 8  10 10 12)
#declare -a arrLx=(4 6 6 8 8)
#declare -a arrLy=(4 4 6 6 8)


for i in \
`seq 0 8`
#`seq 0 4`
do

Lx=${arrLx[${i}]}
Ly=${arrLy[${i}]}

  output=dat_Lx${Lx}_Ly${Ly}
  echo -ne "" > ${output}
  for e in \
  `seq 12 1 34`
#  `seq 12 1 32`
  do
##
## https://stackoverflow.com/questions/53776308/compute-the-mean-and-std-over-a-column-with-awk
## awk '{if($3!=""){count++;sum+=$3};y+=$3^2} END{sq=sqrt(y/NR-(sum/NR)^2);sq=sq?sq:0;print "Mean = "sum/count ORS "S.D = ",sq}'
##
## https://stackoverflow.com/questions/6363441/check-if-a-file-exists-with-a-wildcard-in-a-shell-script
    if compgen -G "../e${e}/dat_renyib_Lx${Lx}_Ly${Ly}_t*_e${e}_r*" > /dev/null; then
      echo -ne "${Lx} ${Ly} ${e} " >> ${output}
      cat ../e${e}/dat_renyib_Lx${Lx}_Ly${Ly}_t*_e${e}_r* | \
        awk '{if($3!=""){count++;sum+=$3};y+=$3^2} END{sq=sqrt(y/NR-(sum/NR)^2);sq=sq?sq:0;print sum/count,sq/sqrt(count)}' \
        >> ${output}
    fi
  done

done
