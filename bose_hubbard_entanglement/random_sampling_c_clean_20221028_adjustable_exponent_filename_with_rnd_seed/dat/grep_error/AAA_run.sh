#!/bin/bash

for L in \
`seq 16 2 18`
#`seq 16 2 40`
do
#  t=$((2*L))
  output=dat_L${L}
  echo -ne "" > ${output}
  for e in \
  12 13
  #12 13 14 15 16 17 18 19 20
  do
##
## https://stackoverflow.com/questions/53776308/compute-the-mean-and-std-over-a-column-with-awk
## awk '{if($3!=""){count++;sum+=$3};y+=$3^2} END{sq=sqrt(y/NR-(sum/NR)^2);sq=sq?sq:0;print "Mean = "sum/count ORS "S.D = ",sq}'
##
    echo -ne "${L} ${e} " >> ${output}
    cat ../dat_renyib_L${L}_t*_e${e}_r* | \
      awk '{if($3!=""){count++;sum+=$3};y+=$3^2} END{sq=sqrt(y/NR-(sum/NR)^2);sq=sq?sq:0;print sum/count,sq/sqrt(count)}' \
      >> ${output}
  done
done
