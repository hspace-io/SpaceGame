from flask import Flask
import os
from main import index, game
from main import mysql

app = Flask(__name__)
app.config['MYSQL_HOST'] = os.environ.get('MYSQL_HOST', 'db')
app.config['MYSQL_USER'] = os.environ.get('MYSQL_USER', 'REDACTED')
app.config['MYSQL_PASSWORD'] = os.environ.get('MYSQL_PASSWORD', 'REDACTED')
app.config['MYSQL_DB'] = os.environ.get('MYSQL_DATABASE', 'REDACTED')
app.secret_key = os.environ.get('SECRET_KEY', 'REDACTED')

mysql.init_app(app)

app.register_blueprint(index)
app.register_blueprint(game)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)