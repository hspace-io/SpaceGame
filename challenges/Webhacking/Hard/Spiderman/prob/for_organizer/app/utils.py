import requests
import random
import hashlib
import time

class SessionManager:
    def __init__(self):
        self.sessions = {}

    def generate_token(self, length=32):
        return random.randbytes(length).hex()

    def create_session(self, user_id, expiration = time.time() + 3600):
        token = self.generate_token()
        self.sessions[token] = {'user_id': user_id, 'expires_at': expiration}
        return token

    def validate_session(self, token):
        session = self.sessions.get(token)
        if session and session['expires_at'] > time.time():
            return session['user_id']
        elif session:
            self.sessions.pop(token)
        return None

    def invalidate_session(self, token):
        return self.sessions.pop(token, None)


class User:
    def __init__(self, user_id, password):
        self.user_id = user_id
        self.password = self.hash_password(password)
        self.image_url = ''

    def update(self, image_url):
        self.image_url = image_url
        
    @staticmethod
    def hash_password(password):
        """
            {{super mega hash}}
        """
        return hashlib.sha256(password.encode()).hexdigest()

    def verify_password(self, password):
        return self.hash_password(password) == self.password


users = {}
session_manager = SessionManager()


def register_user(user_id, password):
    if user_id in users:
        return False
    users[user_id] = User(user_id, password)
    return True


def login_user(user_id, password):
    user = users.get(user_id)
    if user and user.verify_password(password):
        token = session_manager.create_session(user_id)
        return token
    return False


def logout_user(token):
    session_manager.invalidate_session(token)
    return True


def get_user_by_token(token):
    user_id = session_manager.validate_session(token)
    if user_id:
        return users.get(user_id)
    return None