import subprocess

from flask import Flask, request, render_template

app = Flask(__name__)

try:
    FLAG = open('./flag.txt', 'r').read()
except:
    FLAG = 'hspace{wh1t35pac3_can_b3_r3plac3d_w1th_IFS}'

keywords = ['&', 'cp', 'mv', 'rm', 'sh', 'bash', ' ', '*', '/', '=', '\n', '\r', '\t', '\x0b', '\x0c', '-', '+', 'flag', 'cat', ]
def check_input(data):
    for keyword in keywords:
        if keyword in data.lower():
            return True
    return False

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/commandroom', methods=['GET', 'POST'])
def command():
    if request.method == 'POST':
        cmd = request.form.get('cmd')
        print(cmd)
        if check_input(cmd):
            return render_template('command.html', result='do not hack ㅡㅡ')
        try:
            output = subprocess.check_output(['/bin/sh', '-c', cmd], timeout=5)
            resp = render_template('command.html', result=output.decode('utf-8'))
        except Exception as e:
            print(e)
            resp = render_template('command.html', result='error...')
        return resp
    return render_template('command.html')

app.run(host='0.0.0.0', port=8080)
