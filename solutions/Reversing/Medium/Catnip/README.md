# catnip

# Usage
- 

# Concept
- 변형된 TEA 알고리즘

# Writeup


#### catnip.c
```C
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#define DELTA 0x13379999
uint32_t unk_4010[4] = { 0xfedcbabc, 0xcafebabe, 0x13379999, 0x4d3c2b1a };

void sub_1269(uint32_t v[2], uint32_t k[4]) {
    uint32_t v0 = v[0], v1 = v[1], sum = 0;
    uint32_t k0 = k[0], k1 = k[1], k2 = k[2], k3 = k[3];

    for (int i = 0; i < 24; i++) {
        sum += DELTA;
        v0 += ((v1 >> 6) + k0) ^ (v1 + sum) ^ ((v1 << 3) + k1);
        v1 += ((v0 >> 6) + k2) ^ (v0 + sum) ^ ((v0 << 3) + k3);
    }

    v[0] = v0; v[1] = v1;
}

int sub_1370(uint32_t enc[], uint32_t enc_input[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        if (enc[i] != enc_input[i]) {
            return 0;
        }
    }
    return 1;
}
void sub_1400(uint32_t v[2], uint32_t k[4], int count) {
    if (count > 0) {
        sub_1269(v, k);
        sub_1400(v, k, count - 1);
    }
}

int main() {
    uint32_t enc[12];
    enc[0] = 0x30273baa;
    enc[1] = 0x3ad7d031;
    enc[2] = 0xa60dd865;
    enc[3] = 0xf4cf1585;
    enc[4] = 0x07eba521;
    enc[5] = 0x9c105bb6;
    enc[6] = 0xb461d5d2;
    enc[7] = 0x4bcd388a;
    enc[8] = 0x84289038;
    enc[9] = 0xb1ea6f23;
    enc[10] = 0x01f6349e;
    enc[11] = 0xdf36a91a;


    char input[40];
    printf("input: ");
    scanf("%40s", input);
    input[strcspn(input, "\n")] = '\0';

    uint32_t* encrypted_input = (uint32_t*)malloc(sizeof(uint32_t) * 12);
    memset(encrypted_input, 0, sizeof(uint32_t) * 12);
    memcpy(encrypted_input, input, strlen(input));

    for (size_t i = 0; i < 12; i += 2) {
        sub_1400(&encrypted_input[i], unk_4010, 24);
    }

    if (sub_1370(enc, encrypted_input, 12)) {
        printf("That's right! Catnip is something you eat like this.\n");
    }
    else {
        printf("Spit! Ew! This is too bitter!\n");
    }

    free(encrypted_input);

    return 0;
}
```
TEA를 기반으로 제작하였으며 표준 TEA와는 차이점이 존재합니다.

1. 표준 TEA는 반복문으로 동작하지만, 수정한 TEA는 재귀적으로 동작합니다.

2. TEA의 핵심상수(DELTA) 값은 기존의 0x9E3779B9에서 추론을 어렵게하기 위해 0x13379999로 수정하였습니다.

3. 표준 TEA는 왼쪽으로 4비트, 오른쪽으로 5비트 이동하며, 수정된 TEA는 오른쪽으로 6비트, 왼쪽으로 3비트 이동합니다.

4. 난독화 되어있는 변수들의 실제 변수명은 다음과 같습니다. 


unk_4010 = key,
sub_1400 = recursive_encrypt,
sub_1269 = encrypt,
sub_1370 = verify


이러한 점을 바탕으로 encrypt를 하는 sub_1269(uint32_t v[2],uint32_t v[4])로 32비트 값을 2개씩 암호화 합니다.

이를 재귀적으로 24번 반복합니다.

이후 sub_1370에서 이미 암호화되어있는 enc[12]배열과 사용자의 입력값을 24번 암호화한 encrypted_input을 비교하여 flag와 일치하면 'That's right! Catnip is something you eat like this.' 를 출력하는 문제입니다.


#### ex.c
```C
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define DELTA 0x13379999

void decrypt(uint32_t v[2], uint32_t k[4]) {
    uint32_t v0 = v[0], v1 = v[1];
    uint32_t k0 = k[0], k1 = k[1], k2 = k[2], k3 = k[3];
    uint32_t sum = DELTA * 24;

    for (int i = 0; i < 24; i++) {
        v1 -= ((v0 >> 6) + k2) ^ (v0 + sum) ^ ((v0 << 3) + k3);
        v0 -= ((v1 >> 6) + k0) ^ (v1 + sum) ^ ((v1 << 3) + k1);
        sum -= DELTA;
    }
    v[0] = v0; v[1] = v1;
}

void recursive_decrypt(uint32_t v[2], uint32_t k[4], int count) {
    if (count > 0) {
        recursive_decrypt(v, k, count - 1);
        decrypt(v, k);
    }
}

int main() {
    uint32_t key[4] = { 0xfedcbabc, 0xcafebabe, 0x13379999, 0x4d3c2b1a };
    uint32_t enc[12] = {
        0x30273baa, 0x3ad7d031, 0xa60dd865, 0xf4cf1585,
        0x07eba521, 0x9c105bb6, 0xb461d5d2, 0x4bcd388a,
        0x84289038, 0xb1ea6f23, 0x01f6349e, 0xdf36a91a
    };
    
    uint32_t dec[12];
    memcpy(dec, enc, sizeof(enc));
    
    for (size_t i = 0; i < 12; i += 2) {
        recursive_decrypt(&dec[i], key, 24);
    }
    
    printf("Decrypted hex values:\n");
    for (int i = 0; i < 12; i++) {
        printf("%08x ", dec[i]);
    }
    printf("\n\nDecrypted string:\n");
    char* str = (char*)dec;
    for (size_t i = 0; i < sizeof(dec); i++) {
        if (str[i] >= 32 && str[i] <= 126) { 
            printf("%c", str[i]);
        }
    }
    printf("\n");
    
    return 0;
}
```

1. decrypt() 함수에서 sum은 24번 암호화를 거쳤으니 sum = DELTA * 24로 설정합니다.
2. 복호화는 encrypt의 역순으로 진행하는 점을 유의하여 작성합니다.
3. recursive_decrypt() 함수에서는 재귀함수를 먼저 호출한 뒤에 복호화를 진행합니다.
4. main 함수에서 반복문으로 recurisve_decrypt를 할 때 decrypt(uint32_t v[2],)이기 때문에 2개씩 호출한다는 점을 유의하며 i+=2를 사용합니다.

암호화는 sum += DELTA이지만, 복호화는 sum -= DELTA라는 점을 유의하며 작성합니다.