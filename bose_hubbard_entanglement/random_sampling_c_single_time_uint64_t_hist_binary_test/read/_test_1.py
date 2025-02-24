## https://stackoverflow.com/questions/69752048/how-to-read-write-float-values-from-a-binary-file-in-python-if-the-file-was-cre

import numpy as np
import struct

file = "../dat_hist_binary_L40_t80.0000000000"

num = 2**18

with open(file,"rb") as file:
#    dat = struct.unpack('f'*10, file.read(4*10))
    dat = struct.unpack('d'*num, file.read(8*num))
print(dat)
