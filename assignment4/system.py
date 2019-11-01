import numpy as np

x = np.array(input().split(),dtype=float)
h = np.array(input().split(),dtype=float)
r = np.zeros((1,x.size + h.size - 1))

for i in range(0,x.size):
    r[0][i:i+h.size] = r[0][i:i+h.size]+ h*x[i]
for i in range(0,r[0].size):
    print(r[0][i]," ",end="")
