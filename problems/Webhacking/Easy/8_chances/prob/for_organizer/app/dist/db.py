import os
import pymysql

db_config = {
    'host':'db',
    'port':3306,
    'user': os.environ.get('MYSQL_USER'),
    'password':os.environ.get('MYSQL_PASSWORD'),
    'database':'user_db'
}

def dbConnection():
    connection = pymysql.connect(
        host=db_config['host'],
        port=db_config['port'],
        user=db_config['user'],
        password=db_config['password'],
        database=db_config['database'],
        cursorclass=pymysql.cursors.DictCursor,
        charset='utf8mb4'
    )

    return connection