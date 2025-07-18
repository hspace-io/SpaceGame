from flask import Flask, request, render_template, render_template_string, make_response
import random
import string
import asyncio
import threading

app = Flask(__name__)
app.config['SECRET_KEY'] = "v3ry_s3cr3t_k3y_t0_g3t_f14g"


def generate_random_string(length):
    return ''.join(random.choices(string.ascii_letters + string.digits, k=length))

async def create_file_async():
    global data
    await asyncio.sleep(5)
    data = generate_random_string(256)
    return data

def background(func):
    loop = asyncio.new_event_loop()
    asyncio.set_event_loop(loop)
    loop.create_task(func())

@app.route('/', methods=['GET'])
def form():
    return render_template('index.html')

@app.route('/finder', methods=['POST'])
def stage1():
    keyword = request.form.get('keyword')
    arr = ['weather', 'news', 'stocks', 'sports', 'politics']

    blacklist = ['script', 'img', '\'', ' ', 'cat cute', '-', '+', '/', ';', 'frame']

    for word in blacklist:
        if word in keyword:
            return render_template_string(f"<h1>Keyword {word} is not allowed!</h1>")

    if keyword and keyword in arr:
        return render_template_string(f"<h1>keyword {keyword} found!</h1>")
    else:
        return render_template_string(f"<h1>keyword {keyword} is not found!</h1>")

@app.route('/admin', methods=['GET', 'POST'])
def stage2():
    cookie = request.cookies.get('cookie')
    if cookie == "v3ry_s3cr3t_k3y_t0_g3t_f14g":
        background(create_file_async)  
        return render_template('admin.html')
    else:
        response = make_response(render_template('no_access.html'))
        return response

@app.route('/match_data', methods=['POST'])
def match_data():
    
    global data
    data = 'tmp'  
    
    data = background(create_file_async)  
    
    input_data = request.form['data']
    if input_data == data:
        with open('flag.txt', 'r') as file:
            flag_content = file.read()
        return render_template('flag.html', flag=flag_content)
    else:
        return "Data does not match!"

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8000, debug=False)
