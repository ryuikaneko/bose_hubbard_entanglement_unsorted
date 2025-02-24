#!/bin/bash

grep -A 3 "Final set of parameters" dat_fit | grep "+/-" | awk '{print $1,$3,$5}' | sed 's/^a//g' > dat_coeff
