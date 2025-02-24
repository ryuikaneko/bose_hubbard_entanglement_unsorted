#!/bin/bash

output=dat_time
echo -ne "" > ${output}

for L in \
20 40 60 80 100
#20 40 60
do
  echo -ne "${L} " >> ${output}
  cat ../L${L}/dat_time_L*_t* | \
    awk '{if($1!=""){count++;sum+=$1};y+=$1^2} END{sq=sqrt(y/NR-(sum/NR)^2);sq=sq?sq:0;print sum/count,sq/sqrt(count)}' \
    >> ${output}
done
