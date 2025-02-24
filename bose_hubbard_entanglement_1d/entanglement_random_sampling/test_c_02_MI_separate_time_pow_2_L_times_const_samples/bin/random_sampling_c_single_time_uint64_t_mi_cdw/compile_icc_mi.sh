#!/bin/bash

icc -O3 ./rnd/pcg_basic.c -c
#icc -O3 ./rnd/pcg32-demo.c -c
#icc -O3 pcg32-demo.o pcg_basic.o -o pcg32-demo

icc -O3 a_mi.c -c
#icc -O3 a_mi.o pcg_basic.o -o a_mi.out
icc -O3 a_mi.o pcg_basic.o -o a_mi.out -lm
