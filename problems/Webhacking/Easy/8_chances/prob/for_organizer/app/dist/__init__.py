from flask import Flask
import secrets

def create_app():
    app = Flask(__name__)
    
    app.secret_key = secrets.token_hex()

    from views import index
    app.register_blueprint(index.bp)

    return app

if __name__ == "__main__":
    app = create_app()
    app.run(host="0.0.0.0", threaded=True, port=5000)