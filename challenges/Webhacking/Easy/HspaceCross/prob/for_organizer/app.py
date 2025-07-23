from flask import Flask, request, make_response, render_template
from db import get_user_by_login, init_db

app = Flask(__name__)
FLAG = "hspace{fG7dX3vZk9QmBnR2HtTyU6pLsW8cJjVxAaMzN1bY4rKq}"

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/login", methods=["GET", "POST"])
def login():
    if request.method == "GET":
        return render_template("login.html")

    username = request.form.get("username")
    password = request.form.get("password")
    user = get_user_by_login(username, password)

    if user:
        response = make_response()
        if user[1] == "admin":
            response.set_data("✅ 관리자 로그인 성공")
            response.set_cookie("session", "admin_session_token", httponly=True, samesite="Lax")
        else:
            response.set_data(f"👤 일반 사용자 로그인 성공: {user[1]}")
        return response

    return "❌ 로그인 실패", 403

@app.route("/flag", methods=["OPTIONS", "POST"])
def flag():
    origin = request.headers.get("Origin")
    session = request.cookies.get("session")
    method = request.method

    if method == "OPTIONS":
        if origin and "hspace.com" in origin:
            response = make_response()
            response.headers["Access-Control-Allow-Origin"] = origin
            response.headers["Access-Control-Allow-Credentials"] = "true"
            response.headers["Access-Control-Allow-Methods"] = "POST"
            response.headers["Access-Control-Allow-Headers"] = "Content-Type"
            return response, 200
        return "Preflight rejected", 403

    if method == "POST":
        response = make_response()
        if origin and "hspace.com" in origin and session == "admin_session_token":
            response.headers["Access-Control-Allow-Origin"] = origin
            response.headers["Access-Control-Allow-Credentials"] = "true"
            response.set_data(FLAG)
        else:
            response.set_data("Access denied.")
        return response

    return "Method Not Allowed", 405

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)