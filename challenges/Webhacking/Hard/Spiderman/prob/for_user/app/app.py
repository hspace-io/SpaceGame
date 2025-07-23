from flask import Flask, request, render_template, redirect, url_for, render_template_string, abort
from flask_limiter import Limiter
from flask_limiter.util import get_remote_address
from selenium import webdriver
from selenium.webdriver.chrome.service import Service
from webdriver_manager.chrome import ChromeDriverManager
import time
import re

from utils import register_user, login_user, logout_user, get_user_by_token, session_manager
register_user('admin', __import__('random').randbytes(64).hex())
admin_session = session_manager.create_session('admin')

app = Flask(__name__)
limiter = Limiter(
    get_remote_address,
    app=app,
    default_limits=["200 per day", "50 per hour"],
    storage_uri="memory://",
)

def login_only(func):
    def wrapper(*args, **kwargs):
        session = request.args.get('session')
        user = get_user_by_token(session)
        
        if user: 
            return func(*args, **kwargs)
        else:
            return redirect(url_for('login'))
        
    wrapper.__name__ = func.__name__
    return wrapper

def admin_only(func):
    def wrapper(*args, **kwargs):
        session = request.args.get('session')
        user = get_user_by_token(session)
        
        if user and user.user_id == 'admin': 
            return func(*args, **kwargs)
        else:
            return redirect(url_for('index'))

    wrapper.__name__ = func.__name__
    return wrapper

@app.after_request
def set_headers(response):
    response.headers['Cross-Origin-Opener-Policy'] = 'cross-origin'
    response.headers['X-Frame-Options'] = 'DENY'
    response.headers['X-Content-Type-Options'] = 'nosniff'
    response.headers['Cache-Control'] = 'no-store'
    response.headers['Document-Policy'] = 'force-load-at-top'
    response.headers['Referrer-Policy'] = 'no-referrer'
    response.headers['Content-Security-Policy'] = "default-src 'self'; script-src 'self' 'unsafe-inline'; img-src *; style-src 'unsafe-inline' cdnjs.cloudflare.com; font-src cdnjs.cloudflare.com"
    return response

@app.url_defaults
def keep_context(path, values):
    if str(path) in ['login', 'register', 'logout'] or not request.args.get('session', False): return
    for k, v in request.args.items():
        values.setdefault(k, v)

@app.route('/')
def index():
    return render_template('index.html')

@app.route("/login", methods=["GET", "POST"])
def login():
    if request.method == "POST":
        user_id = request.form.get('id')
        pw = request.form.get('pw')
        session = login_user(user_id, pw)
        if session:
            return redirect(url_for('profile', session=session))
    return render_template('login.html')

@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "POST":
        user_id = request.form.get('id')
        pw = request.form.get('pw')
        if not re.match(r'^[a-zA-Z0-9\?\!]+$', user_id):
            return abort(400)
        success = register_user(user_id, pw)
        if success:
            return redirect(url_for('login'))
    return render_template('register.html')


@app.route("/logout")
def logout():
    session = request.args.get('session')
    success= logout_user(session)
    if success:
        return redirect(url_for('home'))

@app.route('/profile', methods=['GET'])
@login_only
def profile():
    session = request.args.get('session')
    user = get_user_by_token(session)
    
    return render_template('profile.html', user=user)

@app.route('/update_profile', methods=['POST'])
@login_only
def update_profile():
    session = request.args.get('session')
    image_url = request.json.get('image_url')
    
    user = get_user_by_token(session)
    if user and image_url:
        user.update(image_url)
    return 'good'

@app.route('/view_profile/<session>', methods=['GET'])
def view_profile(session):
    user = get_user_by_token(session)
    return render_template('view_profile.html', user=user)

# TODO
@app.route('/render_profile', methods=['GET'])
@admin_only
def render_profile():
    token = str(request.args.get('token'))
    ssti = str(request.args.get('ssti'))
    if any(_ in ssti for _ in ['{{', '{%', '{#', '#', '}}', '%}', '#}']):
        return abort(400)
    user = get_user_by_token(token)
    if not user:
        return abort(400)
    return render_template_string(('%s' % ssti).format(user=user))

@app.get("/report")
def report_get():
        return render_template("report.html")

@app.post("/report")
@limiter.limit("1/minute", override_defaults=False)
def report():
    token = str(request.form.get("token"))
    if not re.match(r"[a-z0-9]{64}", token):
        return abort(400)
    if visit(token):
        return render_template("report.html", msg="success")
    else:
        return render_template("report.html", msg="fail")

def visit(token):
    global admin_session
    try:
        session_manager.invalidate_session(admin_session)
        admin_session = session_manager.create_session('admin')
        service = Service(ChromeDriverManager().install())
        options = webdriver.ChromeOptions()
        options.add_argument("--headless=new")
        options.add_argument('--no-sandbox')
        options.add_argument('--disable-dev-shm-usage')
        options.add_argument('--disable-gpu')
        options.add_argument('--js-flags=--noexpose_wasm,--jitless')
        driver = webdriver.Chrome(service=service, options=options)
        driver.set_page_load_timeout(10)
        driver.get(f'http://localhost/view_profile/{token}?session={admin_session}')
        time.sleep(10)
        return True
    except Exception as e:
        print(f"Error in visit: {str(e)}", flush=True)
        return False
    finally:
        driver.quit()
        return True

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=80, debug=False)