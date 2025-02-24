#!/bin/bash

icc -fast ./rnd/pcg_basic.c -c
#icc -fast ./rnd/pcg32-demo.c -c
#icc -fast pcg32-demo.o pcg_basic.o -o pcg32-demo

icc -fast a.c -c
#icc -fast a.o pcg_basic.o -o a.out
icc -fast a.o pcg_basic.o -o a.out -lm
