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
    write(1, "Meow Meow: ", 11);
    read(0, input, 10);
    c = atoi(input);
    uint64_t result = c ^ b;

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

void ls() {
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
    printf("(^ ・x ・^): %s\n", buf);

    return 0;
}
