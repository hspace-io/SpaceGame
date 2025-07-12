import subprocess
import os

flag = os.getenv("FLAG")

program = input("Enter your program: (e.g. 23 2a 12)\n")
program = bytes.fromhex(program)

if len(program) > 0x100:
    print("[*] Too long")
    exit(0)

result = subprocess.run(["./intel-sde/sde64", "--", "./runner"], input=program, stdout=subprocess.PIPE)

if result.stdout == b"Safe Code\n":
    print("[*] Sandbox Pass")
else:
    print("[*] Sandbox Fail")
    exit(0)

result = subprocess.run(["./runner"], input=program, stdout=subprocess.PIPE)

if result.stdout == b"Malicous Code\n":
    print("[*] Runner Fail")
    print("Flag: %s" % flag)
else:
    print("[*] Runner Pass")