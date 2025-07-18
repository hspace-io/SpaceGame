from sage.all import *

M = zero_matrix(ZZ, 20)
M[0, 0] = 5958521
M[1, 1] = 1718869
M[2, 2] = 1621
M[3, 3] = 109
M[4, 4] = 2
M[5, 5] = 2
for i in range(6, 20):
    M[i, i] = 1

for i in range(1, 20):
    M[i] += M[i - 1]

for i in range(0, 20):
    for j in range(1, 20):
        M[i, j] += M[i, j - 1]
        
for i in range(0, 20):
    print("{", end="")
    for j in range(0, 20):
        print(str(M[i, j]), end = "")
        if j != 19 : print(", ", end = "")
        #print(f"m1[{i}, {j}] = " + str(M[i, j]))
    print("},")
    