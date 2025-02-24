#!/bin/bash

for L in \
`seq 16 4 60`
do
#  t=$((2*L))
  output=dat_L${L}
  echo -ne "" > ${output}
  for e in \
  `seq 12 1 32`
  do
##
## https://stackoverflow.com/questions/53776308/compute-the-mean-and-std-over-a-column-with-awk
## awk '{if($3!=""){count++;sum+=$3};y+=$3^2} END{sq=sqrt(y/NR-(sum/NR)^2);sq=sq?sq:0;print "Mean = "sum/count ORS "S.D = ",sq}'
##
## https://stackoverflow.com/questions/6363441/check-if-a-file-exists-with-a-wildcard-in-a-shell-script
#    if compgen -G "../L${L}/dat_renyib_L${L}_t*_e${e}_r*" > /dev/null; then
    if compgen -G "../e${e}/dat_renyib_L${L}_t*_e${e}_r*" > /dev/null; then
      echo -ne "${L} ${e} " >> ${output}
#      cat ../dat_renyib_L${L}_t*_e${e}_r* | \
#      cat ../L${L}/dat_renyib_L${L}_t*_e${e}_r* | \
      cat ../e${e}/dat_renyib_L${L}_t*_e${e}_r* | \
        awk '{if($3!=""){count++;sum+=$3};y+=$3^2} END{sq=sqrt(y/NR-(sum/NR)^2);sq=sq?sq:0;print sum/count,sq/sqrt(count)}' \
        >> ${output}
    fi
  done
done
