## https://stackoverflow.com/questions/69752048/how-to-read-write-float-values-from-a-binary-file-in-python-if-the-file-was-cre
## https://stackoverflow.com/questions/63533004/how-to-open-complex-number-file-in-python-that-generated-by-c-program
## https://okwave.jp/qa/q9889749.html

import numpy as np

from struct import unpack

filename = "data.bin"

with open(filename,"rb") as f:
    dat = f.read()

print(len(dat))

for i in range(0,len(dat),16):
    re, im = unpack("<dd",dat[i:i+16])
    print(re, im)
