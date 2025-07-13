from flask import Flask, render_template, request
import subprocess
import re
import time

app = Flask(__name__)
blacklist = set('flag/')
command_executed = False
last_execution_time = 0

def is_valid_command(command):
    if any(char in blacklist for char in command):
        return False
    return True

def execute_command(command):
    try:
        result = subprocess.run(command, shell=True, capture_output=True, text=True)
        output = result.stdout.strip()
        error = result.stderr.strip()
        if output:
            return output
        if error:
            return error
    except Exception as e:
        return str(e)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/execute_command', methods=['POST'])
def execute_command_route():
    global command_executed, last_execution_time
    current_time = time.time()
    if command_executed and (current_time - last_execution_time) < 30:
        time_left = 30 - (current_time - last_execution_time)
        return f"You've already executed a command! Please wait {int(time_left)} seconds before trying again."
    command = request.form['command']
    if not is_valid_command(command):
        return "try harder!"
    result = execute_command(command)
    command_executed = True
    last_execution_time = current_time
    return result

if __name__ == '__main__':
    app.run(debug=True, port=5678)
