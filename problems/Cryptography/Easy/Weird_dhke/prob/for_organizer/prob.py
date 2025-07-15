from Crypto.Util.number import getPrime, isPrime
import requests
import json
from flag import flag

# Let's get some safe primes! https://2ton.com.au/safeprimes/
params = requests.get("https://2ton.com.au/getprimes/random/2048").json()
p = int(params["p"]["base10"])
q = int(params["q"]["base10"])
g = int(params["g"]["base10"])
assert isPrime(p) and isPrime(q)
assert 2 * q + 1 == p

a = getPrime(2048)
b = getPrime(2048)
shared_secret = pow(g, a * b, p)

print(json.dumps({"p": p, "g": g, "a": a, "b": b}))

try:
    challenge = json.loads(input())
    x = int(challenge["x"])
    y = int(challenge["y"])
except:
    print("Something went wrong ):")
else:
    if not (q // 2 < x < q) or not (q // 2 < y < q):
        print("Nope.")
        exit()
    x_a = pow(x, a, p)
    y_b = pow(y, b, p)
    if x_a * y_b % p == shared_secret:
        print(flag)
