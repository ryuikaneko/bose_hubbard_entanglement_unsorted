#!/bin/bash

prog=../bin/main
L=40
e=20
f=1


t=1.0

${prog} \
  -L ${L} \
  -t ${t} \
  -e ${e} \
  -f ${f} \
> dat_log_t${t}


t=80.0

${prog} \
  -L ${L} \
  -t ${t} \
  -e ${e} \
  -f ${f} \
> dat_log_t${t}
