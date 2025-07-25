# Mine Circuit

# Usage
마인크래프트 맵이 주어질 예정이므로 빌드 X

# Concept
- misc
- logic circuit

# Writeup

주어진 마인크래프트 맵에서 레드스톤 회로를 분석해서 flag 인풋값을 찾아내는 것이 목표이다.

맵 적용은 ```C:\Users\[User]\AppData\Roaming\.minecraft\saves``` 위치에 맵을 옮겨놓으면 마인크래프트 내에서 맵을 플레이할 수 있다.

주어진 맵에서 레드 스톤 회로를 분석하면 로직 게이트가 여러 개 존재한다는 것을 알 수 있다.

로직 게이트는 다음과 같다.

[그림1] AND Gate

[그림2] OR Gate

[그림3] XOR Gate

[그림4] NAND Gate

[그림5] NOR Gate

[그림6] XNOR Gate

[그림7] NOT Gate

[그림8] 3-AND Gate

[그림9] 3-NOR Gate

맵은 총 4개가 주어진다.
위의 사진을 참고해서 레드스톤로 구성된 회로를 분석한다.

구성된 레드스톤 회로를 분석하면 각 맵의 논리식은 다음과 같다.

1번 맵 : a[31] & (~a[29] ^ ~a[31]) & (~a[28] ^ a[29]) & ~(~(~a[27] & a[25]) | ~(~a[27] & ~a[24])) & ~(~(~a[21] & a[19]) | ~a[18]) & ~(~(a[17] & ~a[16]) | ~(a[15] & ~a[12])) & (a[10] ^ ~a[9] ^ ~a[10]) & a[10] & ((~a[8] ^ a[5]) ^ ~(~a[8] | ~a[5])) & ((~a[3] ^ ~a[2]) ^ ~(~a[3] | a[2])) & (a[1] ^ a[0] ^ ~a[1]) & a[1] & (
    (~(a[30] | ~a[26] | a[23]) & ~(1 ^ ~a[20]) & (1 ^ a[22]) | ~(a[14] | ~a[13]) & ~(a[11] | ~a[7]) & (1 ^ ~a[6]) & (0 ^ ~a[4])) & (
    ~(~a[30] | a[26]) & a[23] & (a[22] ^ ~a[20]) & a[22] | ~(~a[14] | a[13] | ~a[11]) & ~(a[7] | a[6] | ~a[4])
    )
)
2번 맵 : ((a[31] & ~a[30] & ~a[29]) ^ (a[31] & ~a[30])) & ((a[27] & a[25]) | a[24]) & ~(a[23] ^ ~a[24]) & ~(a[22] ^ a[23]) & (a[21] ^ a[24]) & ~(~(a[21] & a[20]) | ~(a[19] & ~a[16])) & (a[14] ^ ~a[10]) & a[14] & (a[8] ^ a[9] | a[8]) & ~(~(a[5] & ~a[4]) | a[8]) & ((a[3] & a[2]) ^ (a[1] | a[0])) & a[3] & a[2] & (
    (~(a[28] | a[26]) & ~(a[18] | ~a[17]) & (a[15] ^ 1) | ~(a[13] | a[12] | ~a[11]) & (a[7] ^ ~a[6]) & a[7]) & (
        ~(~a[28] | ~(a[26] & ~(~a[18] | a[17]))) & (a[15] ^ 0) | ~(~a[13] | ~a[12] | a[11]) & ((~a[6] ^ a[7]) ^ (a[6] & a[7]))
    )
)

3번 맵 : ((a[29] ^ a[27]) ^ ~(a[29] | ~a[27])) & ~(a[26] ^ 1) & ~((~a[25] | a[24]) | ~(a[23] & ~a[22])) & (a[21] ^ (a[20] | a[19])) & ~(~a[20] | a[19]) & (a[17] ^ a[16]) & (a[18] ^ ~a[17]) & ~(~(a[15] & ~a[16]) | ~(a[15] & ~a[14])) & (~(a[12] | ~a[11]) ^ (~a[10] | a[9])) & ~(a[12] | ~a[11]) & (a[3] ^ a[8]) & ~(~(a[3] & a[1]) | a[0]) & (
    (~((~a[31] ^ 0) | a[30] | ~a[28]) & a[13] & a[7] | ~((a[7] | a[6]) | ~(~a[5] & ~a[4])) & a[2]) & (
        ~(a[31] | ~a[30]) & ~(a[28] | a[13]) | ~(~(a[6] & a[5]) | ~(a[4] & ~a[2]))
    )
)

4번 맵 : (((a[31] ^ a[30]) & ~a[29]) ^ (~a[31] & a[30] & ~a[29])) & ((a[28] ^ a[27]) ^ ~(a[28] | ~a[27])) & (a[26] ^ ~a[25] ^ ~a[26]) & a[26] & ((~a[24] & a[23]) | a[20]) & (a[17] ^ a[20]) & a[17] & ((~a[16] & a[13] & ~a[12]) ^ (~a[16] & a[13])) & ~(~(a[11] & ~a[8]) | (a[8] | a[5])) & (~(~a[3] | ~a[2]) ^ (~a[1] | a[0])) & a[3] & a[2] & (
    (~(~a[22] | a[21] | a[19]) & ~((a[18] ^ 0) | ~a[15] | ~a[14]) | ~(a[10] ^ 1) & ~((a[9] ^ 0) | (~a[7] ^ 1)) & ~((1 ^ a[6]) | a[4])) & (
        ~((a[22] | ~a[21]) | ~(a[19] & a[18])) & (~a[15] ^ a[14]) & ~a[15] | ((~a[10] & a[9]) | ~a[7]) & ~(~a[7] | a[6] | ~a[4])
    )
)

마인크래프트 레드스톤의 끝이 하나만 존재하기 때문에 해당 레드스톤 회로의 결과가 1이 되어야 한다는 것을 알 수 있다.  
논리식은 z3로도 해결할 수 있지만, 각 맵에서 수식이 복잡하기 때문에 우선 눈에 보기 쉽게 수식을 정리해줄 필요가 있다.  
파이썬의 pyeda 모듈은 전자 설계 자동화를 위한 Python 라이브러리로, 복잡한 논리식이 있을 때, 이를 간단히 나타내줄 수 있다.  


```python
# simplify.py
from pyeda.inter import *

a = []
for i in range(64):
    a.append(expr(f'a[{i}]'))

f_10 = a[31] & (~a[29] ^ ~a[31]) & (~a[28] ^ a[29]) & ~(~(~a[27] & a[25]) | ~(~a[27] & ~a[24])) & ~(~(~a[21] & a[19]) | ~a[18]) & ~(~(a[17] & ~a[16]) | ~(a[15] & ~a[12])) & (a[10] ^ ~a[9] ^ ~a[10]) & a[10] & ((~a[8] ^ a[5]) ^ ~(~a[8] | ~a[5])) & ((~a[3] ^ ~a[2]) ^ ~(~a[3] | a[2])) & (a[1] ^ a[0] ^ ~a[1]) & a[1] & (
    (~(a[30] | ~a[26] | a[23]) & ~(1 ^ ~a[20]) & (1 ^ a[22]) | ~(a[14] | ~a[13]) & ~(a[11] | ~a[7]) & (1 ^ ~a[6]) & (0 ^ ~a[4])) & (
    ~(~a[30] | a[26]) & a[23] & (a[22] ^ ~a[20]) & a[22] | ~(~a[14] | a[13] | ~a[11]) & ~(a[7] | a[6] | ~a[4])
    )
)

f1m = espresso_exprs(f_10.to_dnf())[0]

print(f1m)
#Or(And(~a[0], a[1], a[2], ~a[3], a[4], ~a[5], ~a[6], ~a[7], ~a[8], a[9], a[10], a[11], ~a[12], ~a[13], a[14], a[15], ~a[16], a[17], a[18], a[19], ~a[20], ~a[21], ~a[22], ~a[23], ~a[24], a[25], a[26], ~a[27], ~a[28], ~a[29], ~a[30], a[31]), And(~a[0], a[1], a[2], ~a[3], ~a[4], ~a[5], a[6], a[7], ~a[8], a[9], a[10], ~a[11], ~a[12], a[13], ~a[14], a[15], ~a[16], a[17], a[18], a[19], a[20], ~a[21], a[22], a[23], ~a[24], a[25], ~a[26], ~a[27], ~a[28], ~a[29], a[30], a[31]))
```
위 예시 코드처럼 구한 수식을 pyeda의 ```espresso_exprs``` 함수를 이용하면 간소화된 식을 얻을 수 있다.  
식을 보면 a0 ~ a31까지 AND로 연산된 두 개의 식이 최종적으로 OR로 묶여있다.  
이는 이 식에서 1을 출력해주는 input 값(a0 ~ a31)이 딱 2개 존재한다는 의미이며, 이를 각 맵에서 구한 식을 적용해보면 각 맵마다 2개씩, 총 8개의 입력이 존재한다는 것을 알 수 있다.

32bit의 입력값이 8개 존재하는데, 이를 ascii로 변환하면 다음과 같다.  
```4n_U```, ```7s?}```, ```_5im```, ```ce{C```, ```hspa```, ```p1if```, ```r<ui```, ```y_Ci```

플래그처럼 보이지만, 인덱스를 확인할 수 없다.  
여기서 2가지 방법이 존재하는데, 우선, ```hspa```와 ```ce{C```, ```7s?}```는 플래그 형식에 의해 각 인덱스가 0, 1, 7이라는 것을 유추할 수 있고, 나머지 5개를 모든 경우의 수(5! = 720)를 코드로 비교하여 적절한 플래그를 찾아낼 수 있다.  

두 번째 방법은, 각 맵에 1을 출력해주는 값을 넣으면 해당 입력값의 플래그에서 index 값을 출력해준다. 정답 사진1~8 참고

이를 이용해서 아래와 같이 코드를 작성해주면 플래그를 획득할 수 있다.

```python
from pyeda.inter import *

a = []
for i in range(64):
    a.append(exprvar(f'a[{i}]'))

f_10 = a[31] & (~a[29] ^ ~a[31]) & (~a[28] ^ a[29]) & ~(~(~a[27] & a[25]) | ~(~a[27] & ~a[24])) & ~(~(~a[21] & a[19]) | ~a[18]) & ~(~(a[17] & ~a[16]) | ~(a[15] & ~a[12])) & (a[10] ^ ~a[9] ^ ~a[10]) & a[10] & ((~a[8] ^ a[5]) ^ ~(~a[8] | ~a[5])) & ((~a[3] ^ ~a[2]) ^ ~(~a[3] | a[2])) & (a[1] ^ a[0] ^ ~a[1]) & a[1] & (
    (~(a[30] | ~a[26] | a[23]) & ~(1 ^ ~a[20]) & (1 ^ a[22]) | ~(a[14] | ~a[13]) & ~(a[11] | ~a[7]) & (1 ^ ~a[6]) & (0 ^ ~a[4])) & (
    ~(~a[30] | a[26]) & a[23] & (a[22] ^ ~a[20]) & a[22] | ~(~a[14] | a[13] | ~a[11]) & ~(a[7] | a[6] | ~a[4])
    )
)
f_27 =((a[31] & ~a[30] & ~a[29]) ^ (a[31] & ~a[30])) & ((a[27] & a[25]) | a[24]) & ~(a[23] ^ ~a[24]) & ~(a[22] ^ a[23]) & (a[21] ^ a[24]) & ~(~(a[21] & a[20]) | ~(a[19] & ~a[16])) & (a[14] ^ ~a[10]) & a[14] & (a[8] ^ a[9] | a[8]) & ~(~(a[5] & ~a[4]) | a[8]) & ((a[3] & a[2]) ^ (a[1] | a[0])) & a[3] & a[2] & (
    (~(a[28] | a[26]) & ~(a[18] | ~a[17]) & (a[15] ^ 1) | ~(a[13] | a[12] | ~a[11]) & (a[7] ^ ~a[6]) & a[7]) & (
        ~(~a[28] | ~(a[26] & ~(~a[18] | a[17]))) & (a[15] ^ 0) | ~(~a[13] | ~a[12] | a[11]) & ((~a[6] ^ a[7]) ^ (a[6] & a[7]))
    )
)

f_34 = ((a[29] ^ a[27]) ^ ~(a[29] | ~a[27])) & ~(a[26] ^ 1) & ~((~a[25] | a[24]) | ~(a[23] & ~a[22])) & (a[21] ^ (a[20] | a[19])) & ~(~a[20] | a[19]) & (a[17] ^ a[16]) & (a[18] ^ ~a[17]) & ~(~(a[15] & ~a[16]) | ~(a[15] & ~a[14])) & (~(a[12] | ~a[11]) ^ (~a[10] | a[9])) & ~(a[12] | ~a[11]) & (a[3] ^ a[8]) & ~(~(a[3] & a[1]) | a[0]) & (
    (~((~a[31] ^ 0) | a[30] | ~a[28]) & a[13] & a[7] | ~((a[7] | a[6]) | ~(~a[5] & ~a[4])) & a[2]) & (
        ~(a[31] | ~a[30]) & ~(a[28] | a[13]) | ~(~(a[6] & a[5]) | ~(a[4] & ~a[2]))
    )
)

f_56 = (((a[31] ^ a[30]) & ~a[29]) ^ (~a[31] & a[30] & ~a[29])) & ((a[28] ^ a[27]) ^ ~(a[28] | ~a[27])) & (a[26] ^ ~a[25] ^ ~a[26]) & a[26] & ((~a[24] & a[23]) | a[20]) & (a[17] ^ a[20]) & a[17] & ((~a[16] & a[13] & ~a[12]) ^ (~a[16] & a[13])) & ~(~(a[11] & ~a[8]) | (a[8] | a[5])) & (~(~a[3] | ~a[2]) ^ (~a[1] | a[0])) & a[3] & a[2] & (
    (~(~a[22] | a[21] | a[19]) & ~((a[18] ^ 0) | ~a[15] | ~a[14]) | ~(a[10] ^ 1) & ~((a[9] ^ 0) | (~a[7] ^ 1)) & ~((1 ^ a[6]) | a[4])) & (
        ~((a[22] | ~a[21]) | ~(a[19] & a[18])) & (~a[15] ^ a[14]) & ~a[15] | ((~a[10] & a[9]) | ~a[7]) & ~(~a[7] | a[6] | ~a[4])
    )
)
fx = f_10 | f_27 | f_34 | f_56
def find_satisfying_assignments(expr):
    # 솔루션 리스트 초기화
    solutions = []
    # pyeda의 만족도구 사용
    sat = expr.satisfy_all()
    for solution in sat:
        values = []
        for i in range(32):
            values.append(solution[a[i]])
        solutions.append(values)
    return solutions

def bin_to_char(lists):
    answer = []
    for bin_list in lists:
        num = 0
        for i in range(len(bin_list)):
            num |= bin_list[i] << (31 - i)
        answer.append(int.to_bytes(num, 4, byteorder='big'))
    return answer

answer = find_satisfying_assignments(fx)
flag = bin_to_char(answer)
index = [4, 3, 0, 2, 5, 7, 6, 1]

for i in index:
    print(flag[i].decode(), end='')
print()
# hspace{C4n_U_5imp1ify_Cir<ui7s?}
```