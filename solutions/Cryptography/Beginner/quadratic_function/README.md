# 문제이름
quadratic function
# Usage
- 공란

# Concept
- 2차함수 암호

# Writeup
a, b, c의 값 설정
기본 a,b,c값 변환 과정 시작
a =a x 2, b = b x 2, c = c x 2
a = (Y0%X0)%11 +1
임의의 Y값 = fy
b = (Y0%b)%11+1
fy = (a x b x c) x (a%b) x c
c = (X0%c)%11+1
a, b, c 변환
Y1 = a(X1+b)7c
vn = a(Xn+b)+c
a = (fy%a)%11+1, b = (fy%b)%11+1, a = (Y1%X1)%11+1

c = (fy%c)%11+1
b= (Y1%b)%11+1

c = (X1%c)%11+1
Yn = a(Xn+b)+c의 제곱이 된다
암호화 과정
맨 처음의 임의의 값 y를 통해 abc를 변환하고 이차함수로 암호화한다
그리고 y0의 값을 얻는다
그 후엔 avc값을 원래 변환하는 방식으로 변환하고 이차함수로 암호화한후 반복한다
복호화 과정은
Y =a(X+b)의 제곱+c
루트(Y-c)/a + b =x이 된다
암호화 복호화 과정을 설명했으니 복호화 코드를 작성하자
복호화 코드는 앞에 설명한 과정을 거꾸로하면된다
```python
def twodecrypt():
  ra = 153
  rb = 592
  rc = 104
  ens = input('y: ')
  ens = [int(num) for num in ens.replace('[', '').replace(']', '').split(',')]
  des = []
  a = ra + rb
  b = rb + rc
  c = rc + ra
  fy = ((a * b * c) ** 2)
  a = (fy % (a + b) + a) % (b + c) + 1
  b = (b % (b + c + a) + b) % (c + a) + 1
  c = (fy % (c + a) + c) % (a + b) + (a * b * c) % 3
  x = 0
  for i in ens:
      x = int((((i - c) / a) ** 0.5) - b)
      a = (i % b + a) % (b + a) + 1
      b = (i % a + b) % c + 1
      c = (x % (c * a) + c) % a + (a * b * c) % 11
      print(chr(x), end='')
      des.append(chr(x))
  print()

if __name__ == "__main__":
  twodecrypt()

```
이런식으로 짤수있다
235607456 , 244139045 , 241797200 , 230257265 , 231779621 , 231779621 , 250439141 , 244922168 , 247279061 , 241019093 , 231779621 , 230257265 , 243357176 , 234072560 , 245706545 , 240242240 , 231779621 , 252026705
암호화된숫자가 이거고 이걸 하나씩 다 저 코드안에 넣으면
```
hspace{twocarfunc}
```
가 나온다

