# 문제 이름
ObfuSWF

# 출제자명
오성훈(5un9hun)

# 컨셉
- swf reversing
- swf obfuscation

# Write up
주어진 파일은 swf 파일로, adobe flash를 이용해서 실행할 수 있습니다.
또한, swf 리버싱 툴인 [jpexs-decompiler](https://github.com/jindrapetrik/jpexs-decompiler)를 통해 코드를 확인할 수 있습니다.

Class `§1t175s0§` 
1. `§1t175s0§` 클래스를 살펴보면 렌더링되는 컴포넌트들을 설정하고, 그래픽에 추가하는 로직이 있습니다. `§1t251s0§` 메소드에서는 InputField와 Button, Button Label을 생성하고, Button의 Listener를 설정하게 됩니다. Button을 클릭하게 되면 기존의 컴포넌트들을 제거하고, `§1t233s0§` 메소드를 호출하게됩니다.
2. `§1t233s0§` 메소드에서는 input값을 인자로 받아서 `§1t216s0§` 클래스의 check 메서드를 호출한 뒤, `§1t216s0§`의 `§1t198s0§` 변수를 확인해서 wrong 또는 correct를 출력하게 됩니다.

Class `§1t216s0§`
`§1t216s0§` 클래스의 `check` 메소드는 크게 3개의 메소드를 호출하고 값을 검증하게 됩니다.
그 전에 먼저 input값의 길이가 null이거나 8의 배수아닌지 체크하고 만약 거짓이라면 `§1t198s0§` 변수를 false로 설정하고 리턴합니다.
1. `§1t210s0§` 
    - "=" 문자열을 제거합니다.
    - input의 각 바이트 값들이 32개의 문자열(ervngpacwidfbuzhklmhso_954207381)인 `§1t209s0§` 변수 안에 있는지 체크
    - `§1t209s0§` 변수는 총 32바이트이므로, index는 최대 31까지밖에 없기 때문에 index값의 최대 bit는 5bit가 됩니다. 따라서, 문자열의 각 문자를 `§1t209s0§` 변수에서 인덱스(0~31)로 변환해 5bit 값으로 만듭니다. 이후, 각 5bit 값을 차례로 이어붙여 하나의 긴 비트 스트림을 만듭니다.
    - 1byte(8bit) 단위로 끊어서 바이트 배열로 만들게 됩니다.
    - 즉, base32 decode 로직과 같습니다. 하지만 일반적인 base32가 아닌, table이 정의된 custom base32 decode입니다.
    - 나온 결과값이 null이 아닌지 체크하고, 길이가 20인지 체크합니다.
2. `§1t211s0§`
    - 1번의 결과 값에 대해 bit swap을 수행합니다. (1 -> 8, 2 -> 7 ... 7 -> 2, 8 -> 1)
3. `§1t200s0§`
    - 2번의 결과 값에 대해 xor을 진행합니다. key값은 `§1t197s0§` 변수를 이용하고 해당 값은 `spaceWar`가 됩니다.
    - i % len(key)의 인덱스의 값과 xor을 진행합니다.

3번의 결과값으로 나온 `_loc4_` 값과 크기가 20인 배열의 `§1t196s0§` 값과 비교해서 값이 일치하다면 `§1t198s0§` 변수를 true로 설정하게 됩니다.

따라서 `§1t198s0§` 변수가 true이므로 `§1t233s0§` 메소드에서는 correct를 출력하고 flag를 출력하게 됩니다. (flag는 input값이 됩니다.)

따라서 이를 역연산하면 flag를 획득할 수 있습니다.
1. `§1t196s0§` xor `§1t197s0§`(spaceWar)
2. bit swap
3. custom base32 encode (table = `§1t209s0§`(ervngpacwidfbuzhklmhso_954207381))

다음은 poc입니다.

```py
BASE32_ALPHABET = "ervngpacwidfbuzhklmhso_954207381"
key = 'spaceWar'

def encode(data):
    bits = 0
    value = 0
    output = []

    for byte in data:
        value = (value << 8) | byte
        bits += 8
        while bits >= 5:
            output.append(BASE32_ALPHABET[(value >> (bits - 5)) & 0b11111])
            bits -= 5
    if bits > 0:
        output.append(BASE32_ALPHABET[(value << (5 - bits)) & 0b11111])
    
    while len(output) % 8 != 0:
        output.append("=")
    return "".join(output)

def bitswap(data):
    out = []
    for b in data:
        result = 0
        for i in range(8):
            result = (result << 1) | ((b >> i) & 1)
        out.append(result)
    return out

def xor(a, b):
    result = []
    for i in range(len(a)):
        result.append(a[i] ^ ord(b[i % len(b)]))
    return result

if __name__ == "__main__":
    flag = [131, 210, 106, 164, 162, 243, 124, 13, 168, 96, 219, 14, 198, 186, 129, 129, 143, 253, 186, 62]
    flag = xor(flag, key)
    flag = bitswap(flag)
    flag = encode(flag)

    print(f'hspace{{{flag}}}')
```