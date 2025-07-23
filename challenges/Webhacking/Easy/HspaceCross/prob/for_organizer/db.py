import sqlite3

DB_FILE = "users.db"

def init_db():
    conn = sqlite3.connect(DB_FILE)
    cur = conn.cursor()
    cur.execute("DROP TABLE IF EXISTS users")
    cur.execute("""
        CREATE TABLE users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT,
            password TEXT
        )
    """)
    cur.execute("INSERT INTO users (username, password) VALUES ('admin', 'supersecret')")
    cur.execute("INSERT INTO users (username, password) VALUES ('guest', '1234')")
    conn.commit()
    conn.close()

def get_user_by_login(username, password):
    forbidden = [" or ", "--", ";", "union", "#"]
    for keyword in forbidden:
        if keyword in username.lower() or keyword in password.lower():
            return None

    conn = sqlite3.connect(DB_FILE)
    cur = conn.cursor()
    query = f"SELECT * FROM users WHERE username = '{username}' AND password = '{password}'"
    try:
        cur.execute(query)
        return cur.fetchone()
    except sqlite3.Error:
        return None
    finally:
        conn.close()
