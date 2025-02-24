## https://stackoverflow.com/questions/69752048/how-to-read-write-float-values-from-a-binary-file-in-python-if-the-file-was-cre
## https://stackoverflow.com/questions/63533004/how-to-open-complex-number-file-in-python-that-generated-by-c-program

import numpy as np

from struct import unpack

filename = "../dat_hist_binary_L40_t80.0000000000"

numbers = [] 
with open(filename, "rb") as f:
    data = f.read()  # read all data
#count = len(data) // 8  # each complex number takes 8 bytes
count = len(data) // 16  # each complex number takes 8 bytes
i = 0
for index in range(count):
    # "f" means float (4 bytes); unpack returns a tuple with one value
    real = unpack("d", data[i:i + 8])[0]
    i += 8
    im = unpack("d", data[i:i + 8])[0]
    i += 8
#    real = unpack("f", data[i:i + 4])[0]
#    i += 4
#    im = unpack("f", data[i:i + 4])[0]
#    i += 4
    numbers.append(complex(real, im))

print(numbers)
