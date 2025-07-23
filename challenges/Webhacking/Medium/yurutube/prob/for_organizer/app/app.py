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