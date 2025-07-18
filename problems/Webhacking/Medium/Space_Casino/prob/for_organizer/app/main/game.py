from flask import Blueprint, render_template, session, redirect, url_for, request
from .extension import mysql
import random

game = Blueprint('game', __name__, url_prefix='/games')

@game.route('/')
def games():
    if 'userid' not in session:
        return redirect(url_for('index.login'))
    return render_template('games.html')

@game.route('/roulette', methods=['GET'])
def roulette():
    if 'userid' not in session:
        return redirect(url_for('index.login'))
    return render_template('roulette.html')

@game.route('/roulette', methods=['POST'])
def roulette_post():
    if 'userid' not in session:
        return redirect(url_for('index.login'))
    try:
        bet = str(request.form['bet'])
        red_black = str(request.form['rb'])
    except:
        return render_template('roulette.html', error='Invalid bet')
    if not bet.isdigit():
        return render_template('roulette.html', error='Invalid bet')
    bet = int(bet)
    if bet <= 0 or bet > 1000000:
        return render_template('roulette.html', error='Invalid bet')
    if red_black != 'red' and red_black != 'black':
        return render_template('roulette.html', error='Invalid red_black')
    
    cur = mysql.connection.cursor()
    cur.execute(f"SELECT cash FROM users WHERE user_id = '{session['userid']}'")
    user = cur.fetchone()
    if user[0] < bet:
        return render_template('roulette.html', error='Not enough money')
    cur.execute(f"UPDATE users SET cash = cash - {bet} WHERE user_id = '{session['userid']}'")
    mysql.connection.commit()

    result = random.randint(0, 17)
    if red_black == 'red':
        result *= 2
    else:
        result *= 2
        result += 1
    
    if (red_black == 'red' and result % 2 == 1) or (red_black == 'black' and result % 2 == 0):
        cur.execute(f"UPDATE users SET cash = cash + {bet * 2} WHERE user_id = '{session['userid']}'")
        mysql.connection.commit()
        return render_template('roulette_result.html', result=result, win=True)
    else:
        return render_template('roulette_result.html', result=result, win=False)

@game.route('/toto', methods=['GET'])
def toto():
    if 'userid' not in session:
        return redirect(url_for('index.login'))
    cur = mysql.connection.cursor()
    cur.execute(f"SELECT name, bet FROM toto")
    teams = cur.fetchall()
    bets = sum([x[1] for x in teams])
    return render_template('toto.html', teams=teams, bets=bets)

@game.route('/toto', methods=['POST'])
def toto_post():
    if 'userid' not in session:
        return redirect(url_for('index.login'))
    cur = mysql.connection.cursor()
    cur.execute(f"SELECT name, bet FROM toto")
    teams = cur.fetchall()
    bets = sum([x[1] for x in teams])
    try:
        bet = str(request.form['bet'])
        team = str(request.form['team'])
    except:
        return render_template('toto.html', teams=teams, bets=bets, error='Invalid bet')
    if not bet.isdigit():
        return render_template('toto.html', teams=teams, bets=bets, error='Invalid bet')
    bet = int(bet)
    if bet <= 0 or bet > 1000000:
        return render_template('toto.html', teams=teams, bets=bets, error='Invalid bet')
    cur = mysql.connection.cursor()
    team = [x for x in teams if x[0] == team]
    if len(team) != 1:
        return render_template('toto.html', teams=teams, bets=bets, error='Invalid team')
    team = team[0][0]
    cur.execute(f"SELECT cash FROM users WHERE user_id = '{session['userid']}'")
    user = cur.fetchone()
    if user[0] < bet:
        return render_template('toto.html', teams=teams, bets=bets, error='Not enough money')
    cur.execute(f"UPDATE users SET cash = cash - {bet} WHERE user_id = '{session['userid']}'")
    cur.execute(f"UPDATE toto SET bet = bet + {bet} WHERE name = '{team}'")
    mysql.connection.commit()
    return redirect(url_for('game.toto'))