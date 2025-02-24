#!/bin/bash

gcc -O3 ./rnd/pcg_basic.c -c
#gcc -O3 ./rnd/pcg32-demo.c -c
#gcc -O3 pcg32-demo.o pcg_basic.o -o pcg32-demo

gcc -O3 a.c -c
#gcc -O3 a.o pcg_basic.o -o a.out
gcc -O3 a.o pcg_basic.o -o a.out -lm
