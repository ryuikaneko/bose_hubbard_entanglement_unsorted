#!/bin/bash

gcc -O3 ./rnd/pcg_basic.c -c
#gcc -O3 ./rnd/pcg32-demo.c -c
#gcc -O3 pcg32-demo.o pcg_basic.o -o pcg32-demo

gcc -O3 a_cdw.c -c
#gcc -O3 a_cdw.o pcg_basic.o -o a_cdw.out
gcc -O3 a_cdw.o pcg_basic.o -o a_cdw.out -lm
