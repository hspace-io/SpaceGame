from pwn import*

pl = b''
#mov idx imm
#0x10
def mov_imm(dst, src):
    global pl
    pl += p32(10)
    pl += p32(1)
    pl += p32(dst)
    pl += p32(src)

#store imm idx
#0xc
def store(dst, src):
    global pl
    pl += p32(11)
    pl += p32(dst)
    pl += p32(src)

#mov idx idx 
#0x28
def mov_reg(dst, src):
    global pl
    mov_imm(3, dst)
    mov_imm(4, src)
    pl += p32(10)
    pl += p32(0)

def interrupt(num):
    global pl
    mov_imm(0, num)
    pl += p32(1)

def load(dst):
    global pl
    mov_imm(3, dst)
    pl += p32(9)
    pl += p32(0)


def sub():
    global pl
    pl += p32(3)

def jmp():
    global pl
    pl += p32(8)

def add():
    global pl
    pl += p32(2)

def bls():
    global pl
    pl += p32(7)

def divv():
    global pl 
    pl += p32(5)

def mul():
    global pl 
    pl += p32(4)

def padded_hex(value):
    hex_str = hex(value)[2:] 
    return hex_str.zfill(2) 
'''
err_msg = "Please input code!\n"
for i, ch in enumerate(err_msg):
    mov_imm(1, ord(ch))
    store(i, 1)
mov_imm(1, 0)
mov_imm(2, len(err_msg))
interrupt(1)
interrupt(60)
for i in range(len(pl)):
    print("\\x" + padded_hex(pl[i]), end ='')
'''
msg = "FLAG > "
for i, ch in enumerate(msg):
    mov_imm(1, ord(ch))
    store(i, 1)
mov_imm(1, 0)
mov_imm(2, len(msg))
interrupt(1)
mov_imm(1, 0x10)
mov_imm(2, 0x100)
interrupt(0) 
mov_imm(7, 0x10)
#r7 = data idx, r5 = i, r6 = data[data_idx], 
#index 75
mov_reg(4, 7)
load(6)

mov_reg(1, 7)
mov_imm(2, 1)
add()
mov_reg(7, 0)

mov_reg(1, 5)
mov_imm(2, 4)
divv()
mov_reg(8, 1)

mov_reg(1, 6)
mov_reg(2, 8)
bls()

mov_reg(1, 0)
mov_reg(2, 5)
add()
mov_reg(10, 0) # r10 = (flag[i] << (i % 4)) + i

mov_reg(1, 5)
mov_imm(2, 4)
mul()
mov_reg(1, 0)
mov_imm(2, 0x111)
add()  
mov_reg(4, 0) #r4 = 0x111 + i * 4
load(8) 

mov_reg(1, 8)
mov_imm(2, 0x100)
mul()
mov_reg(8, 0)
mov_reg(1, 5)
mov_imm(2, 4)
mul()
mov_reg(1, 0)
mov_imm(2, 0x110)
add()  
mov_reg(4, 0) #r4 = 0x110 + i * 4
load(9) 
mov_reg(1, 8)
mov_reg(2, 9)
add()

mov_reg(1, 0)
mov_reg(2, 10) 
sub()

mov_imm(0, 5) # go back
jmp()
interrupt(60)
#idx ++ 
mov_reg(1, 5)
mov_imm(2, 1)
add()
mov_reg(5, 0)
mov_imm(1, 23)
mov_reg(2, 5)
sub() #if r5 == 23 -> jmp 1000
mov_imm(0, 14) # win
jmp()
#go back 
mov_imm(1, 0)
mov_imm(2, 0)
sub()
mov_imm(0, negate(336 + 10)) # wback
jmp()
msg = "You win!\n"
for i, ch in enumerate(msg):
    print(len(pl))
    mov_imm(1, ord(ch))
    store(i, 1)
mov_imm(1, 0)
mov_imm(2, len(msg))
interrupt(1)
interrupt(60)

file = open('./code.txt', 'wb')
file.write(pl)
file.close()

