## https://stackoverflow.com/questions/69752048/how-to-read-write-float-values-from-a-binary-file-in-python-if-the-file-was-cre
## https://stackoverflow.com/questions/63533004/how-to-open-complex-number-file-in-python-that-generated-by-c-program
## https://okwave.jp/qa/q9889749.html

import numpy as np

from struct import unpack

#filename = "../dat_hist_binary_L40_t80.0000000000"
filename = "../dat_histtotal_binary_L40_t80.0000000000"
#filename = "../dat_histblock_binary_L40_t80.0000000000"

with open(filename,"rb") as f:
    dat = f.read()
#print("# bits",len(dat))
#print("# lines",len(dat)//16)

for i in range(0,len(dat),16):
    re, im = unpack("<dd",dat[i:i+16])
    print(re, im)
