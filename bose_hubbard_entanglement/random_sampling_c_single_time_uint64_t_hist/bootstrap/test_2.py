## https://www.digitalocean.com/community/tutorials/bootstrap-sampling-in-python

import numpy as np
import random

ntot = 2**8
np.random.seed(12345)
x = np.random.normal(loc=300.0,size=ntot)
#print("x",x)
print("ave(x),std(x)",np.average(x),np.std(x))

y = []
nboot = (2**4)*ntot
for i in range(nboot):
    rnd = np.random.randint(ntot,size=ntot)
    y.append(np.average(x[rnd]))
#print("y",y)
print("ave(y),std(y)",np.average(y),np.std(y))
print("1/sqrt(ntot)",1.0/np.sqrt(ntot))
