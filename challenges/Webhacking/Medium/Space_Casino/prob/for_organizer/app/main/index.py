from flask import Blueprint, render_template, request, redirect, url_for, session
import hashlib
import os
import re
from .extension import mysql

index = Blueprint('index', __name__)

@index.route('/')
def root():
    if 'userid' not in session:
        return render_template('index.html', userid=None, cash=0)
    cur = mysql.connection.cursor()
    cur.execute(f"SELECT cash FROM users WHERE user_id = '{session['userid']}'")
    user = cur.fetchone()
    cash = user[0]
    return render_template('index.html', userid=session.get('userid'), cash=cash)

@index.route('/login', methods=['GET'])
def login():
    return render_template('login.html')

@index.route('/login', methods=['POST'])
def login_post():
    try:
        userid = str(request.form['userid'])
        password = str(request.form['password'])
    except:
        error = 'Invalid userid or password'
        return render_template('login.html', error=error)
    if not re.match(r'^[a-zA-Z0-9_]+$', userid):
        error = 'Invalid userid'
        return render_template('login.html', error=error)
    cur = mysql.connection.cursor()
    cur.execute(f"SELECT * FROM users WHERE user_id = '{userid}' AND password = '{hashlib.sha256(password.encode()).hexdigest()}'")
    user = cur.fetchone()
    if user is None:
        error = 'Invalid userid or password'
        return render_template('login.html', error=error)
    session['userid'] = user[1]
    return redirect(url_for('index.root'))
    
@index.route('/logout')
def logout():
    session.pop('userid', None)
    session.pop('username', None)
    return redirect(url_for('index.root'))

@index.route('/register', methods=['GET'])
def register():
    return render_template('register.html')

@index.route('/register', methods=['POST'])
def register_post():
    try:
        userid = str(request.form['userid'])
        password = str(request.form['password'])
    except:
        error = 'Invalid userid or password'
        return render_template('register.html', error=error)
    if not re.match(r'^[a-zA-Z0-9_]+$', userid):
        error = 'Invalid userid'
        return render_template('register.html', error=error)
    cur = mysql.connection.cursor()
    cur.execute(f"SELECT * FROM users WHERE user_id = '{userid}'")
    user = cur.fetchone()
    if user is not None:
        error = 'User already exists'
        return render_template('register.html', error=error)
    cur.execute(f"INSERT INTO users (user_id, password) VALUES ('{userid}', '{hashlib.sha256(password.encode()).hexdigest()}')")
    cur.execute(f"INSERT INTO coupons (user_id, coupon_id, coupon_name) VALUES ('{userid}', '{os.urandom(16).hex()}', 'Welcome Coupon')")
    mysql.connection.commit()
    return redirect(url_for('index.login'))

@index.route('/coupons')
def coupons():
    if 'userid' not in session:
        return redirect(url_for('index.login'))
    cur = mysql.connection.cursor()
    cur.execute(f"SELECT * FROM coupons WHERE user_id = '{session['userid']}'")
    coupons = cur.fetchall()
    return render_template('coupons.html', coupons=coupons)

@index.route('/coupons/<coupon_id>')
def coupon(coupon_id):
    if 'userid' not in session:
        return redirect(url_for('index.login'))
    coupon_id = str(coupon_id)
    if re.match(r'^[a-f0-9]{32}$', coupon_id) is None:
        return redirect(url_for('index.coupons'))
    cur = mysql.connection.cursor()
    cur.execute(f"SELECT * FROM coupons WHERE user_id = '{session['userid']}' AND coupon_id = '{coupon_id}'")
    coupon = cur.fetchone()
    if coupon is None:
        return redirect(url_for('index.coupons'))
    if coupon[4]:
        return redirect(url_for('index.coupons'))
    cur.execute(f"UPDATE coupons SET used = 1 WHERE user_id = '{session['userid']}' AND coupon_id = '{coupon_id}'")
    cur.execute(f"UPDATE users SET cash = cash + 1000 WHERE user_id = '{session['userid']}'")
    mysql.connection.commit()
    return redirect(url_for('index.coupons'))

@index.route('/flag')
def flag():
    if 'userid' not in session:
        return redirect(url_for('index.login'))
    cur = mysql.connection.cursor()
    cur.execute(f"SELECT cash FROM users WHERE user_id = '{session['userid']}'")
    user = cur.fetchone()
    if user[0] < 31337:
        return render_template('flag.html', error='Not enough money')
    return render_template('flag.html', flag=os.environ['FLAG'])