## https://www.digitalocean.com/community/tutorials/bootstrap-sampling-in-python

import numpy as np
import random

np.random.seed(12345)
x = np.random.normal(loc=300.0,size=1000)
print(np.mean(x))

sample_mean = []
for i in range(50):
  y = random.sample(x.tolist(), 4)
  avg = np.mean(y)
  sample_mean.append(avg)

print(np.mean(sample_mean))

