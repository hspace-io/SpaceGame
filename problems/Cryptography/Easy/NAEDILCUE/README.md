# NAEDILCUE

# Usage
- 공란

# Concept
- Euclidean algorithm

# Writeup

유클리드 호제법(Euclidean algorithm)은 두 개의 정수의 최대공약수를 찾는 방법 중 하나로, 두 개의 정수를 나누어가면서 나머지가 0이 될 때까지 반복적으로 나누는 과정을 통해 최대공약수를 찾습니다. 예를 들어, 두 수를 $a$와 $b$라고 할 때, $a$를 $b$로 나눈 나머지를 $r$이라고 합시다. 그럼, $a$와 $b$의 최대공약수는 $b$와 $r$의 최대공약수와 같습니다. 이러한 원리를 이용하여 반복적으로 나누어가면서 최대공약수를 구할 수 있습니다.

####  chall.py
```python
from sage.all import *

flags = "hspace{????????????????????????????????????????????????}"
remain_mod = [ord(flag) for flag in flags]
remain = []
MOD = 127

x=var('x')
equation = remain_mod[-1]*x==remain_mod[-2]
solution = int(solve_mod(equation,MOD)[0][0])
remain.append(remain_mod[-1])
remain.append(remain_mod[-1]*solution)

lens = len(remain_mod)
for i in range(lens-2):
    equation = remain_mod[lens-i-1]+remain_mod[lens-i-2]*x==remain_mod[lens-i-3]
    solution = int(solve_mod(equation,MOD)[0][0])
    remain.append(remain[i]+remain[i+1]*solution)

print(f"a = {remain[-1]}")
print(f"b = {remain[-2]}")
```

위 코드에서는 이미 최대공약수가 주어진 상태에서, 이를 이용하여 원래의 두 정수를 찾아내는 방식으로 작동합니다. <br>
flag값을 순차적으로 아스키 코드 숫자로 변환합니다. 이 값들은 두 정수와 유클리드 호제법 수행시 나온 나머지를 $mod\ 127$ 한 값을 의미합니다. 이 정보들을 이용해 최대공약수가 125(flag 마지막 값 '}')인 두 정수 값을 알아낼 수 있습니다. <br>
또한 거꾸로 된 유클리드 호제법의 연산을 시뮬레이션하기 위해 SageMath를 사용합니다. 코드는 flag값을 뒤에서부터 두 문자씩 묶어서 유클리드 호제법의 방정식을 풀어나갑니다. 이때, 방정식을 푸는 과정에서는 나머지를 $mod\ 127$ 한 값을 사용합니다.

####  sol.py
```python
from sage.all import *

MOD = 127

def gcd(a,b):
	print(chr(a%MOD),end="")
	print(chr(b%MOD),end="")
	while b>0:
		a,b = b, a % b
		print(chr(b%MOD),end="")
	print()
	return a

gcd(1675964232781566069087817329966284898171618708951165297416912646941584293444329986348660536848801803561047610837986971446700916638533920981792271625,38963832594780901602621357934146878062430309854913062092516368607419969039631564145541925423332620514353280095135728191656216488006190178968989125)
```
두 정수 값과 두 정수 값을 유클리드 호제법을 수행하여 나온 나머지를 $mod\ 127$ 한 값을 모두 아스키 코드 값으로 변환하면 flag값이 나옵니다.