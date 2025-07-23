import os
import hashlib

BSIZE = 26
def pad(s):
    padlen = BSIZE-len(s)%BSIZE
    return s+bytes([padlen])*padlen

L = 286
message = os.urandom(L).hex()

while True:
    inp = input(">>> ")
    if inp.strip() == "Yo Bro I got the answer":
        break
    output = ""
    s = set(inp)
    for c in message:
        if c in s:
            pass
        else:
            output += c
    output = pad(output.encode())
    out_len = len(output)
    nonce = os.urandom(32)
    out = ""
    for i in range(0,out_len,BSIZE):
        h = hashlib.sha256(output[i:i+BSIZE]+nonce).hexdigest()
        out += h
    print(f"Nonce = {nonce.hex()}")
    print(f"Output = {out}")

answer = input("Answer >>> ")
if answer == message:
    with open("flag.txt","r") as f:
        print(f.read())
