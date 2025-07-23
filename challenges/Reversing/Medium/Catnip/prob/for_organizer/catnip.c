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

