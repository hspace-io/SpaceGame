from flask import Blueprint, render_template, session, abort, request, redirect, url_for, flash
from . import check
import string
import random
from db import dbConnection

bp = Blueprint('main', __name__, url_prefix='/')
chance = 8

@bp.route('/', methods = ['GET', 'POST'])
def index():
    username = session.get('username')
    
    if request.method == 'POST':
        global chance
        username = request.form['user']
        password = request.form['pass']
        testquery = request.form['testquery']
        tp = request.form['type']

        if tp == 'reset':
            chance = 8
            return redirect(url_for('main.index'))
        
        if tp == 'test':
            if check.waf1(testquery):
                return abort(400)

            connection = dbConnection()

            try:
                with connection.cursor() as cursor:
                    if chance >= 8 :
                        new_password = ""
                        for _ in range(38):
                            new_chr = random.choice(string.ascii_letters)
                            new_password += new_chr
                        cursor.execute(f"UPDATE users SET password='{new_password}' WHERE username='admin'")
                        connection.commit()
                        chance = 0
                        
                    chance += 1
                    if chance > 8 : 
                        return redirect(url_for('main.index'))
                        
                    cursor.execute(testquery)
                    user = cursor.fetchone()
                    result = list(user.values())
                    if len(result) == 1 :
                        if type(result[0]) == int:
                            flash(result[0])
                    
                    return redirect(url_for('main.index'))
            except Exception:
                return redirect(url_for('main.index'))
            finally:
                if connection:
                    connection.close()
            
        if tp == 'real':
            if check.waf2(username):
                return abort(400)
            
            if check.waf2(password):
                return abort(400)

            connection = dbConnection()
            try:
                with connection.cursor() as cursor:
                    cursor.execute(f"SELECT * FROM users WHERE username='{username}' AND password='{password}'")
                    user = cursor.fetchone()
                    if user:
                        session['username'] = user['username']

                        if session.get('username') == 'admin':
                            return render_template("flag.html")
                        else:
                            return redirect(url_for('main.index'))
                    else:
                        return redirect(url_for('main.index'))
            except Exception:
                return redirect(url_for('main.index'))
            finally:
                if connection:
                    connection.close()
    
    return render_template('index.html')

@bp.errorhandler(400)
def handle_400_error(_):
    return render_template('400.html'), 400