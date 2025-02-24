import numpy as np

a = np.load("out.npz")
print(a)
print(a.files)
#print(a["myVar1"])
#print(a["myVar2"])
print(a["arr1"])
