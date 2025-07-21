import subprocess
import os

CTF_UID = 1000
CTF_GID = 1000

def lower_privileges():
    os.setgid(CTF_GID)
    os.setuid(CTF_UID)

def node_exec(tmp_path):
    stdout, stderr = subprocess.Popen(['node', tmp_path], stdout=subprocess.PIPE, stderr=subprocess.STDOUT, preexec_fn=lower_privileges).communicate()
    return stdout, stderr