# challenge_name
- simple_some/ip

# Usage
- 사용자에게만 문제파일을 제공하면됩니다.

# Concept
- crypto misc

# Writeup

- 4444method ID를 통해 request ID를 구함

3333method로 들어오는 요청은 해당 request ID가 없으면 불가.

아무요청을 보내면  request ID를  그응답갑안에  포함되어 다시 돌아옴.
ient received (Method 3333): 

ECC parameters and public key: a=0,b=-21,p=434252269029337012720086440207,Gx=394839663302675518626403237489,Gy=34756457949101399445824435551,order=434252269029337012720086440208,public_key_x=11178039696838839383575413170,public_key_y=4791644216513807409890442682

from sage.all import *

# 타원 곡선 파라미터
a = 0
b = -21
p = 434252269029337012720086440207
Gx = 394839663302675518626403237489
Gy = 34756457949101399445824435551
order = 434252269029337012720086440208

# 서버의 public key 좌표
public_key_x = 51658588559097521072468863330
public_key_y = 224904824121548850350134438482

# 타원 곡선 정의
E = EllipticCurve(GF(p), [a, b])
G = E(Gx, Gy)
P = E(public_key_x, public_key_y)
Q = P

def movAttack(G, Q):
    k = 1
    while (p**k - 1) % E.order():
        k += 1
    print(f"Found embedding degree k = {k}")
    
    Ee = EllipticCurve(GF(p**k, 'y'), [a, b])
    
    R = Ee.random_point()
    m = R.order()
    d = gcd(m, G.order())
    B = (m // d) * R
    
    assert G.order() / B.order() in ZZ
    assert G.order() == B.order()
    
    Ge = Ee(G)
    Qe = Ee(Q)
    
    n = G.order()
    alpha = Ge.weil_pairing(B, n)
    beta = Qe.weil_pairing(B, n)
    
    print('Computing discrete logarithm...')
    nQ = beta.log(alpha)
    
    # 결과값을 order로 모듈러 연산
    nQ = nQ % order
    return nQ

print("Starting MOV attack...")
try:
    nQ = movAttack(G, Q)
    print(f"\nPrivate key found: {nQ}")
    
    # 검증
    check_point = G * nQ
    if check_point == P:
        print("\nVerification successful!")
        print(f"G * private_key = ({check_point[0]}, {check_point[1]})")
        print(f"Public key P = ({P[0]}, {P[1]})")
    else:
        print("\nVerification failed!")
    
except Exception as e:
    print(f"Attack failed: {str(e)}")