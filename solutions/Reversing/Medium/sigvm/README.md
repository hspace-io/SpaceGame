# sigvm

# Usage

# Concept

- vm, signal handling, longjmp

# Writeup

## 배경지식

이 문제의 핵심은 아니지만 문제에서 제공한 VM을 보면 메인루프가 없는 것을 볼 수 있다. 

```cpp
void __fastcall __noreturn main(int a1, char **a2, char **a3)
{
  unsigned int *v3; // rax
  unsigned int *v4; // rax
  int fd; // [rsp+1Ch] [rbp-4h]

  setbuf(stdout, 0LL);
  fd = open("code.txt", 0);
  read(fd, &unk_404B80, 0x4000uLL);
  close(fd);
  if ( fd > 0 )
  {
    qword_404B60 = (__int64)&unk_404B80;
    __sigsetjmp(env, 1);
    v3 = (unsigned int *)qword_404B60;
    qword_404B60 += 4LL;
    longjmp((struct __jmp_buf_tag *)(200LL * *v3 + 0x4041A0), 1);
  }
  qword_404B60 = (__int64)off_404098;
  __sigsetjmp(env, 1);
  v4 = (unsigned int *)qword_404B60;
  qword_404B60 += 4LL;
  longjmp((struct __jmp_buf_tag *)(200LL * *v4 + 0x4041A0), 1);
}
```

`longjmp` 와 `setjmp` 함수에 대해서 알아야 되는데, `setjmp` 는 버퍼에 현재 컨텍스트를 저장, `longjmp` 는 해당 컨텍스트로 점프이다. 그리고 `longjmp` 의 두번째 인자가 `setjmp` 의 인자값이 된다. `setjmp` 를 통해서 단순 컨텍스트를 저장할 때는 0이 리턴되고, `longjmp` 를 통해서 컨텍스트가 이동했을 때는 `longjmp` 로 전달한 인자값을 리턴하는 구조이다. 

이를 이용하면 아래와 같이 초기 컨텍스트 저장에는 아무런 행동을 하지 않는 코드를 작성할 수 있다. vm operation 중  bit left 함수인데, 처음에는 `setjmp` 의 반환값이 0이기 때문에 그냥 함수가 끝나지만 `main` 함수에서 `longjmp` 를 통해서 이 함수의 컨텍스트로 돌아올 때는 `setjmp` 가 1을 반환한다. 따라서 vm operation 을 수행할 수 있고, 계산 이후에는 다시 코드를 fetch 하는 곳으로 돌아간다. 

```cpp
int sub_401416()
{
  int result; // eax

  result = _setjmp(stru_404718);
  if ( result )
  {
    status = dword_404B24 << n;
    longjmp(env, 1);
  }
  return result;
}
```

`main` 함수에서 코드를 fetch 하는 부분에는 `sigsetjmp` 함수가 쓰였다. 

문제의 vm 은 hypercall 처리를 위해서 `SIGSEGV` 시그널을 이용하는데, 해당 시그널 핸들러에서 시그널을 처리하고 다시 코드를 fetch 하는 부분으로 돌아갈 때 시그널마스크도 복원시켜야 하기 때문이다. (만약 `longjmp` , `setjmp` 를 이용하면 계속 핸들러 안에 있는 것으로 간주되어 핸들러가 동작하지 않는다!)

```cpp
void __noreturn sub_4011F6()
{
  if ( (_DWORD)qword_404B00 != 60 )
  {
    if ( (int)qword_404B00 <= 60 )
    {
      if ( (_DWORD)qword_404B00 )
      {
        if ( (_DWORD)qword_404B00 == 1 )
          write(1, (const void *)((unsigned int)dword_404B24 + 4230016LL), (unsigned int)n);
      }
      else
      {
        read(0, (void *)((unsigned int)dword_404B24 + 4230016LL), (unsigned int)n);
      }
    }
    siglongjmp(env, 1);
  }
  exit(status);
}
```

시스템콜은 `read`, `write` , `exit` 를 지원한다. 

## 풀이

생소한 함수들이 사용됐지만 사실 문제를 푸는데는 그렇게 큰 영향을 끼치지는 않는다. 

해석을 위해문제 코드를 분석하는 분석기를 만들어보자. 

```python
from pwn import*

f = open('code.txt', 'rb')
code = f.read()

get_data = lambda i: u32(code[i * 4 : (i + 1)* 4])

i = 0
while i < len(code) // 4:
    op = get_data(i)
    if op == 1:
        print(f'int r0')
    elif op == 2:
        print(f'add r1, r2')
    elif op == 3:
        print(f'sub r1, r2')
    elif op == 4:
        print(f'mul r1, r2')
    elif op == 5:
        print(f'div r1, r2')
    elif op == 6:
        print(f'r1 >> r2')
    elif op == 7:
        print(f'r1 << r2')
    elif op == 8:
        print(f'jmp + r0')
    elif op == 9:
        i += 1
        flag = get_data(i)
        if flag > 0:
            print(f'load r[r1], {flag}')
        else:
            print(f'load r[r3], data[r[4]]')
        i += 1
    elif op == 10:
        i += 1
        flag = get_data(i)
        if flag == 1: # mov imm
            i += 1
            dst = get_data(i)
            i += 1
            src = get_data(i)
            print(f'mov r[{dst}], {src}')
        else:
            print(f'mov r[r3], r[r4]')
    elif op == 11:
        i += 1
        dst = get_data(i)
        i+= 1
        src= get_data(i)
        print(f'store data[{dst}], r[{src}]')
    i += 1        

            

    
```

vm은 +, *, / , -, %, <<, >>, jmp, mov, store, load 명령을 지원하는데, 해당 연산자들에 해당하는 코드를 해석하는 것은 별로 어렵지 않으니 본 풀이에 포함하지는 않았다. 문제에서 제공하는 code.txt 파일의 크기에 비해서 매우 작은 코드가 나왔다. 부분별로 분석해보자

아래 코드는 data 영역에 특정 상수를 저장시키고 intterupt 를 호출한다. vm 호출규약에 따르면 `write(1, “FLAG >”, 7)`  로 번역된다. 

```python
mov r[1], 70
store data[0], r[1]
mov r[1], 76
store data[1], r[1]
mov r[1], 65
store data[2], r[1]
mov r[1], 71
store data[3], r[1]
mov r[1], 32
store data[4], r[1]
mov r[1], 62
store data[5], r[1]
mov r[1], 32
store data[6], r[1]
mov r[1], 0
mov r[2], 7
mov r[0], 1
int r0
```

아래 코드는 동일하게 `read(0, &data[0x10], 0x100)` 으로 해석된다. 

```python
mov r[1], 16
mov r[2], 256
mov r[0], 0
int r0
```

핵심 부분이다. mov 인스트럭션을 위해서 r3, r4 레지스터가 계속 쓰이고 있기 때문에 이를 정리해서 보면 된다. 

분석을 하면 flag[i] << ( i % 4 ) + i 값이 data 영역에 존재하는 정답과 맞는지 확인을 하는 내용임을 알 수 있다. data 영역에 존재하는 정답은 92바이트로 vm 이 4바이트씩 처리를 하는 것을 고려하면 플래그는 23바이트이다. 

```python
mov r[7], 16
mov r[3], 4
mov r[4], 7
mov r[r3], r[r4]
mov r[3], 6
load r[r3], data[r[4]]
mov r[3], 1
mov r[4], 7
mov r[r3], r[r4]
mov r[2], 1
add r1, r2
mov r[3], 7
mov r[4], 0
mov r[r3], r[r4]
mov r[3], 1
mov r[4], 5
mov r[r3], r[r4]
mov r[2], 4
div r1, r2
mov r[3], 8
mov r[4], 1
mov r[r3], r[r4]
mov r[3], 1
mov r[4], 6
mov r[r3], r[r4]
mov r[3], 2
mov r[4], 8
mov r[r3], r[r4]
r1 << r2
mov r[3], 1
mov r[4], 0
mov r[r3], r[r4]
mov r[3], 2
mov r[4], 5
mov r[r3], r[r4]
add r1, r2
mov r[3], 10
mov r[4], 0
mov r[r3], r[r4]
mov r[3], 1
mov r[4], 5
mov r[r3], r[r4]
mov r[2], 4
mul r1, r2
mov r[3], 1
mov r[4], 0
mov r[r3], r[r4]
mov r[2], 273
add r1, r2
mov r[3], 4
mov r[4], 0
mov r[r3], r[r4]
mov r[3], 8
load r[r3], data[r[4]]
mov r[3], 1
mov r[4], 8
mov r[r3], r[r4]
mov r[2], 256
mul r1, r2
mov r[3], 8
mov r[4], 0
mov r[r3], r[r4]
mov r[3], 1
mov r[4], 5
mov r[r3], r[r4]
mov r[2], 4
mul r1, r2
mov r[3], 1
mov r[4], 0
mov r[r3], r[r4]
mov r[2], 272
add r1, r2
mov r[3], 4
mov r[4], 0
mov r[r3], r[r4]
mov r[3], 9
load r[r3], data[r[4]]
mov r[3], 1
mov r[4], 8
mov r[r3], r[r4]
mov r[3], 2
mov r[4], 9
mov r[r3], r[r4]
add r1, r2
mov r[3], 1
mov r[4], 0
mov r[r3], r[r4]
mov r[3], 2
mov r[4], 10
mov r[r3], r[r4]
sub r1, r2
mov r[0], 5
jmp + r0
mov r[0], 60
int r0
mov r[3], 1
mov r[4], 5
mov r[r3], r[r4]
mov r[2], 1
add r1, r2
mov r[3], 5
mov r[4], 0
mov r[r3], r[r4]
mov r[1], 23
mov r[3], 2
mov r[4], 5
mov r[r3], r[r4]
sub r1, r2
mov r[0], 14
jmp + r0
```

지금까지의 내용을 기반으로 플래그를 bruteforcing 하는 코드를 작성할 수 있다. 

```python
result = [0x68, 0xe7, 0x1c2, 0x30b, 0x67, 0xcf, 0x1f2, 0x19f, 0x60, 0x8f, 0x19e, 0x193, 0x3d, 0x73, 0x146, 0x1c7, 0x6f, 0xe5, 0x166, 0x37b, 0x84, 0x57, 0x20a]
flag = ['h']
for i in range(0, 23):
    for j in range(0x20, 0x80):
        tmp = (j << (i % 4)) + i
        if (tmp) == result[i]:
            print(chr(j), end = '')
```

`hspace{3XCe113N7_jUmp!}`