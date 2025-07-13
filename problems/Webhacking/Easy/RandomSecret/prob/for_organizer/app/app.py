from flask import Flask, render_template, session, redirect, url_for, request,render_template_string
import os
import threading
import time


try:
    FLAG = open("./flag.txt", "r").read()
except :
    FLAG = "hspace{Th3_s3cret_key_Mu5t_nOT_Be_le4KeD!!!}"

app = Flask(__name__)
secret_key_lock = threading.Lock()
users = {
    "admin": "vEry_d1FF1cu1T_pAs5wOrD1234",
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
            return redirect(url_for('admin'))
        else:
            return render_template_string(f"{username} is wrong username or password")
    return render_template('login.html')




@app.route('/guest')
def guest():
    if 'guest' not in session:
        return render_template_string('<script>alert("You are not guest!");location.href="/login"</script>')
    else:
        return "Hello guest"


@app.route('/admin')
def admin():
    if 'admin' not in session:
        return render_template_string('<script>alert("You are not admin!");location.href="/login"</script>')
    else:
        return "Hello admin here is "+FLAG
    

if __name__ == '__main__':
    app.secret_key = generate_new_secret_key()
    app.run(host='0.0.0.0', port=4000)
