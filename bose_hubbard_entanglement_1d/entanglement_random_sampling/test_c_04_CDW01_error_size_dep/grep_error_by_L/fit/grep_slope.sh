#!/bin/bash

grep -A 3 "Final set of parameters" dat_fit_coeff | grep "+/-" | awk '{print $1,$3,$5}' | sed 's/^a//g' > dat_slope
grep -A 4 "Final set of parameters" dat_fit_coeff | tail -n 1 | grep "+/-" | awk '{print $1,$3,$5}' | sed 's/^b//g' >> dat_slope
