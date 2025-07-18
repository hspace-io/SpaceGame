# Wired dhke

# Concept
- Diffie Hellman


# Write Up
주어진 문제는 Diffie-Hellman Key Exchange에서 2048-bit safe prime p와 GF(p)의 generator g가 주어집니다. 특이한 점은 shared secret을 계산하는 데 사용되는 2048-bit 소수 a, b를 공개한다는 것입니다.

문제의 조건을 보면 flag를 얻기 위해서는 다음을 만족하는 x, y를 찾아야 한다는 것을 알 수 있습니다.

$$
x^a * y^b = g^{ab}\mod p\newline x,\ y \in (q/2,\ q)
$$

간단하게 $x=g^b,\ y=0$ 같은 solution을 생각해 볼 수 있지만, 이는 주어진 범위 조건에 어긋납니다.

잘 생각해보면, GF(p)의 모든 원소는 $g^n$ 꼴로 나타낼 수 있습니다. 따라서 $x=g^s,\ y=g^t$로 놓으면 주어진 식은 

$$
g^{sa}*g^{tb}=g^{sa+tb}=g^{ab}\mod p
$$

를 만족하는 s, t를 찾는 문제가 됩니다.

# Solution 1

1차 부정방정식 $sa+tb = ab$를 만족하는 s, t를 찾아 범위를 만족하는 값을 제출해도 정답입니다. 

Extended euclidean algorithm을 사용하여 $s'a+t'b=1$인 s’, t’을 찾고, 양변에 $ab/\gcd(a,b) = ab$를 곱하면 됩니다.

$$
s = abs' - k*b\newline t = abt' + k*a
$$

```python
from Crypto.Util.number import inverse
from pwn import *
import json

def xgcd(a, b):
    x0, x1, y0, y1 = 1, 0, 0, 1
    while b:
        q, a, b = a // b, b, a % b
        x0, x1 = x1, x0 - q * x1
        y0, y1 = y1, y0 - q * y1
    return a, x0, y0

# context.log_level = 'debug'
r = process(['python', './prob.py'])
params = json.loads(r.recvline().strip())

p = params['p']
g = params['g']
a = params['a']
b = params['b']
q = (p - 1) // 2

s0, t0 = xgcd(a, b)[1:]
for k in range(1000):
    s = a*b*s0 - b*k
    t = a*b*t0 + a*k
    if (q//2 < pow(g, s, p) < q) and (q//2 < pow(g, t, p) < q):
        log.info(f'k: {k}')
        break

assert s * a + t * b == a*b

log.info(f'Found s: {hex(s)}, t: {hex(t)}')

r.sendline(json.dumps({'x': pow(g, s, p), 'y': pow(g, t, p)}).encode())
log.success(f'Flag: {r.recvline().strip().decode()}')
```

# Solution 2

다른 방법으로는, 페르마의 소정리를 이용할 수 있습니다. 

$$
sa + tb = ab\mod p-1\newline\Leftrightarrow sa+tb = ab + k(p-1)\text{ for some }k
$$

$$
g^{sa+tb}=g^{ab+k(p-1)}=g^{ab}\mod p\text{ (by fermat's little thm)}
$$

따라서 k를 0, -1, -2, … 로 변화시켜 방정식의 trivial solution을 생각해 볼 수 있습니다.

$\gcd(a,b)=1$이므로 $N = ab+k(p-1),\ k\in \mathbb{N}^{-}$라 하면  $t_p = N*b^{-1}\mod a$, $s_p = \frac{N-bt_p}{a}$로 주어지며, 정수 m에 대해 

$$
s = s_p + b*m\newline t = t_p - a*m
$$

로 계산됩니다. k값을 변화시키며 trivial solution을 쉽게 찾을 수 있습니다.

```python
from Crypto.Util.number import inverse
from pwn import *
import json

# context.log_level = 'debug'
r = process(['python', './prob.py'])
params = json.loads(r.recvline().strip())

p = params['p']
g = params['g']
a = params['a']
b = params['b']
q = (p - 1) // 2

for k in range(100):
    N = a * b - k * (p-1)
    t = N * inverse(b, a) % a
    s = (N - t * b) // a
    if (q//2 < pow(g, s, p) < q) and (q//2 < pow(g, t, p) < q):
        log.info(f'k: {k}')
        break

assert s * a + t * b == N

log.info(f'Found s: {hex(s)}, t: {hex(t)}')

r.sendline(json.dumps({'x': pow(g, s, p), 'y': pow(g, t, p)}).encode())
log.success(f'Flag: {r.recvline().strip().decode()}')
```

<aside>
🚩

HSPACE{d10ph4n71n3_3qu4710n_F7W}

</aside>