from Crypto.Util.number import getRandomNBitInteger, isPrime, bytes_to_long

def getprimes(n):
    while True:
        x = getRandomNBitInteger(n)
        p = x**2+(x+1)**2
        q = (x+1)**2+(x+2)**2
        if isPrime(p) and isPrime(q):
            return p,q
        
p1,q1 = getprimes(512)
p2,q2 = getprimes(512)

n = p1*q1*p2*q2
e = 0x10001

flag = b'hspace{oRderEd_PrimE_eQu4res_Rs4_arE_y0u_A_m4ste1_Of_e9uAti0n_Man1pu1at1oN?}'
pt = bytes_to_long(flag)
ct = pow(pt, e, n)

print(f"n = {n}")
print(f"e = {e}")
print(f"ct = {ct}")
