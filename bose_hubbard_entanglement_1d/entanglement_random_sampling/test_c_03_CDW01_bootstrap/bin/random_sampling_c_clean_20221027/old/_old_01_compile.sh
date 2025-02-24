#!/bin/bash

cc=gcc
option="-O3"

${cc} ${option} ./rnd/pcg_basic.c -c
${cc} ${option} sub_util.c -c
${cc} ${option} sub_calmat.c -c
${cc} ${option} sub_glynn.c -c
${cc} ${option} main.c -c
${cc} ${option} main.o pcg_basic.o sub_util.o sub_calmat.o sub_glynn.o \
  -o main.out -lm
