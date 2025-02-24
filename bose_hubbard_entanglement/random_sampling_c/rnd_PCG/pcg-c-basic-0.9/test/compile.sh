#!/bin/bash

gcc -O3 pcg_basic.c -c
gcc -O3 pcg32-demo.c -c
gcc -O3 pcg32-demo.o pcg_basic.o -o pcg32-demo
