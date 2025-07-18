#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

char *table[128] = {
    "운동하고","산책하고","책읽고","영화보고","노래부르고","춤추고","잠자고","샤워하고","세수하고","양치하고","물마시고","커피마시고","술마시고","옷입고","옷갈아입고","머리감고","머리말리고","일하고","회의하고","글쓰고","그림그리고","사진찍고","노트필기하고","쇼핑하고","여행하고","캠핑하고","수영하고","달리기하고","헬스하고","자전거타고","버스타고","지하철타고","택시타고","비행기타고","기차타고","배타고","걷고","뛰고","넘어지고","일어나고","앉고","눕고","쉬고","웃고","울고","화내고","미안하고","고맙고","감사하고","사랑하고","좋아하고","싫어하고","생각하고","계획하고","상상하고","기억하고","잊고","기다리고","만나고","헤어지고","웃으며보고","대화하고","물어보고","대답하고","들어가고","나가고","열고","닫고","줍고","버리고","꺼내고","넣고","고치고","만들고","조립하고","부수고","켜고","끄고","충전하고","다운로드하고","업로드하고","저장하고","삭제하고","검색하고","설치하고","업데이트하고","로그인하고","로그아웃하고","결제하고","주문하고","포장하고","잔소리하고","공감하고","반응하고","질문하고","확인하고","끝내고","출근하고","퇴근하고","외출하고","집에가고","모이고","흩어지고","정리하고","고르고","살고","태어나고","성장하고","배우고","졸업하고","취업하고","퇴사하고","쉬고","늘고","줄고","넘어지고","일어나고","우산쓰고","지켜보고","다가가고","멈추고","시작하고","끝나고","놀고","즐기고","참여하고","이사하고","적응하고"
};
char buffer[0x1000];

uint64_t lcg1_state;
uint64_t lcg2_state;

const uint32_t k[64] = {
0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee ,
0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501 ,
0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be ,
0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821 ,
0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa ,
0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8 ,
0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed ,
0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a ,
0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c ,
0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70 ,
0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05 ,
0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665 ,
0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039 ,
0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1 ,
0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1 ,
0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };
 
// r specifies the per-round shift amounts
const uint32_t r[] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
                      5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
                      4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
                      6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};
 
// leftrotate function definition
#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))
 
void to_bytes(uint32_t val, uint8_t *bytes)
{
    bytes[0] = (uint8_t) val;
    bytes[1] = (uint8_t) (val >> 8);
    bytes[2] = (uint8_t) (val >> 16);
    bytes[3] = (uint8_t) (val >> 24);
}
 
uint32_t to_int32(const uint8_t *bytes)
{
    return (uint32_t) bytes[0]
        | ((uint32_t) bytes[1] << 8)
        | ((uint32_t) bytes[2] << 16)
        | ((uint32_t) bytes[3] << 24);
}
 
void md5(const uint8_t *initial_msg, size_t initial_len, uint8_t *digest) {
 
    // These vars will contain the hash
    uint32_t h0, h1, h2, h3;
 
    // Message (to prepare)
    uint8_t *msg = NULL;
 
    size_t new_len, offset;
    uint32_t w[16];
    uint32_t a, b, c, d, i, f, g, temp;
 
    // Initialize variables - simple count in nibbles:
    h0 = 0x67452301;
    h1 = 0xefcdab89;
    h2 = 0x98badcfe;
    h3 = 0x10325476;
 
    //Pre-processing:
    //append "1" bit to message    
    //append "0" bits until message length in bits ≡ 448 (mod 512)
    //append length mod (2^64) to message
 
    for (new_len = initial_len + 1; new_len % (512/8) != 448/8; new_len++)
        ;
 
    msg = (uint8_t*)malloc(new_len + 8);
    memcpy(msg, initial_msg, initial_len);
    msg[initial_len] = 0x80; // append the "1" bit; most significant bit is "first"
    for (offset = initial_len + 1; offset < new_len; offset++)
        msg[offset] = 0; // append "0" bits
 
    // append the len in bits at the end of the buffer.
    to_bytes(initial_len*8, msg + new_len);
    // initial_len>>29 == initial_len*8>>32, but avoids overflow.
    to_bytes(initial_len>>29, msg + new_len + 4);
 
    // Process the message in successive 512-bit chunks:
    //for each 512-bit chunk of message:
    for(offset=0; offset<new_len; offset += (512/8)) {
 
        // break chunk into sixteen 32-bit words w[j], 0 ≤ j ≤ 15
        for (i = 0; i < 16; i++)
            w[i] = to_int32(msg + offset + i*4);
 
        // Initialize hash value for this chunk:
        a = h0;
        b = h1;
        c = h2;
        d = h3;
 
        // Main loop:
        for(i = 0; i<64; i++) {
 
            if (i < 16) {
                f = (b & c) | ((~b) & d);
                g = i;
            } else if (i < 32) {
                f = (d & b) | ((~d) & c);
                g = (5*i + 1) % 16;
            } else if (i < 48) {
                f = b ^ c ^ d;
                g = (3*i + 5) % 16;          
            } else {
                f = c ^ (b | (~d));
                g = (7*i) % 16;
            }
 
            temp = d;
            d = c;
            c = b;
            b = b + LEFTROTATE((a + f + k[i] + w[g]), r[i]);
            a = temp;
 
        }
 
        // Add this chunk's hash to result so far:
        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
 
    }
 
    // cleanup
    free(msg);
 
    //var char digest[16] := h0 append h1 append h2 append h3 //(Output is in little-endian)
    to_bytes(h0, digest);
    to_bytes(h1, digest + 4);
    to_bytes(h2, digest + 8);
    to_bytes(h3, digest + 12);
}

uint64_t urandom(void)
{
    uint64_t ret;
    int fd;

    ret = 0;
    if ((fd = open("/dev/urandom", O_RDONLY)) < 0) {
        exit(1);
    }
    read(fd, &ret, 8);
    close(fd);

    return ret;
}

uint64_t mulmod_128(uint64_t a, uint64_t b, uint64_t m) {
    __uint128_t result = (__uint128_t)a * b;
    return (uint64_t)(result % m);
}

uint64_t lcg1(void)
{
    return lcg1_state = (mulmod_128(lcg1_state, 5824465958561025943, 8608563731122359827) + 7979475314568588667) % 8608563731122359827;
}

uint64_t lcg2(void)
{
    return lcg2_state = (mulmod_128(lcg2_state, 5791510840215506503, 8194488386109005551) + 7731928730996589209) % 8194488386109005551;
}

// AES SBox
static const uint8_t sbox[256] = {
    0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16
};

// Rcon lookup table
static const uint8_t Rcon[11] = {
    0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36
};

// AES key expansion function
static void KeyExpansion(uint8_t* key, uint8_t* expandedKey) {
    int i;
    uint8_t temp[4], k;

    // First round key is the key itself
    memcpy(expandedKey, key, 16);

    // Generate the remaining round keys
    for(i = 4; i < 44; i++) {
        memcpy(temp, expandedKey + (i-1) * 4, 4);
        if(i % 4 == 0) {
            // Rotate word
            k = temp[0];
            temp[0] = sbox[temp[1]];
            temp[1] = sbox[temp[2]];
            temp[2] = sbox[temp[3]];
            temp[3] = sbox[k];
            
            // XOR with Rcon
            temp[0] ^= Rcon[i/4];
        }
        for(int j = 0; j < 4; j++) {
            expandedKey[i * 4 + j] = expandedKey[(i-4) * 4 + j] ^ temp[j];
        }
    }
}

// Mix single column
static uint8_t gmul(uint8_t a, uint8_t b) {
    uint8_t p = 0;
    uint8_t hi_bit_set;
    for(int counter = 0; counter < 8; counter++) {
        if((b & 1) != 0) {
            p ^= a;
        }
        hi_bit_set = (a & 0x80);
        a <<= 1;
        if(hi_bit_set != 0) {
            a ^= 0x1b;
        }
        b >>= 1;
    }
    return p;
}

// Mix columns transformation
static void MixColumns(uint8_t* state) {
    uint8_t temp[4];
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            temp[j] = state[i * 4 + j];
        }
        state[i * 4 + 0] = gmul(0x02, temp[0]) ^ gmul(0x03, temp[1]) ^ temp[2] ^ temp[3];
        state[i * 4 + 1] = temp[0] ^ gmul(0x02, temp[1]) ^ gmul(0x03, temp[2]) ^ temp[3];
        state[i * 4 + 2] = temp[0] ^ temp[1] ^ gmul(0x02, temp[2]) ^ gmul(0x03, temp[3]);
        state[i * 4 + 3] = gmul(0x03, temp[0]) ^ temp[1] ^ temp[2] ^ gmul(0x02, temp[3]);
    }
}

// Shift rows transformation
static void ShiftRows(uint8_t* state) {
    uint8_t temp;
    
    // Row 1
    temp = state[1];
    state[1] = state[5];
    state[5] = state[9];
    state[9] = state[13];
    state[13] = temp;
    
    // Row 2
    temp = state[2];
    state[2] = state[10];
    state[10] = temp;
    temp = state[6];
    state[6] = state[14];
    state[14] = temp;
    
    // Row 3
    temp = state[15];
    state[15] = state[11];
    state[11] = state[7];
    state[7] = state[3];
    state[3] = temp;
}

// Add round key transformation
static void AddRoundKey(uint8_t* state, uint8_t* roundKey) {
    for(int i = 0; i < 16; i++) {
        state[i] ^= roundKey[i];
    }
}

// Main AES encryption function
void AES_encrypt(uint8_t* input, uint8_t* key, uint8_t* output) {
    uint8_t state[16];
    uint8_t expandedKey[176];
    
    // Initial state
    memcpy(state, input, 16);
    
    // Key expansion
    KeyExpansion(key, expandedKey);
    
    // Initial round
    AddRoundKey(state, key);
    
    // Main rounds
    for(int round = 1; round < 10; round++) {
        // SubBytes
        for(int i = 0; i < 16; i++) {
            state[i] = sbox[state[i]];
        }
        
        ShiftRows(state);
        MixColumns(state);
        AddRoundKey(state, expandedKey + round * 16);
    }
    
    // Final round
    for(int i = 0; i < 16; i++) {
        state[i] = sbox[state[i]];
    }
    ShiftRows(state);
    AddRoundKey(state, expandedKey + 160);
    
    // Copy result
    memcpy(output, state, 16);
}

void base128_encode(const uint8_t* input, size_t input_len) {
    size_t output_len = 0;
    uint32_t buffer = 0;
    int bits_in_buffer = 0;

    for (size_t i = 0; i < input_len; i++) {
        // 버퍼에 8비트 추가
        buffer = (buffer << 8) | input[i];
        bits_in_buffer += 8;

        // 7비트씩 처리
        while (bits_in_buffer >= 7) {
            bits_in_buffer -= 7;
            uint8_t index = (buffer >> bits_in_buffer) & 0x7F;
            printf("%s", table[index]);
        }
    }

    // 남은 비트 처리
    if (bits_in_buffer > 0) {
        uint8_t index = (buffer << (7 - bits_in_buffer)) & 0x7F;
        printf("%s", table[index]);
    }
}


void __attribute__((constructor)) setup(void)
{
    uint64_t seed[2];
    int i;
    uint8_t x, y;
    char *tmp;

    x = urandom();
    // printf("SEED %hhx\n", x);
    md5(&x, 1, (uint8_t *)seed);
    
    lcg1_state = seed[1];
    lcg2_state = seed[0];
    // printf("%016lx\n", lcg1_state);
    // printf("%016lx\n", lcg2_state);

    for (i = 0; i < 10000; ++i) {
        lcg1();
        lcg2();
    }

    for (i = 0; i < 10; ++i) {
        x = lcg1() & 0x7f;
        y = lcg1() & 0x7f;
        // printf("%02hhx %02hhx\n", x, y);
        tmp = table[x];
        table[x] = table[y];
        table[y] = tmp;
    }
}

void __attribute__((destructor)) out(void)
{
    uint8_t key[16];
    uint64_t tmp;
    int i, len;
    char *output;


    len = strlen(buffer);
    if (len % 16) {
        len += 16 - len % 16;
    }
    output = malloc(len);

    for (i = 0; i < len; i += 16) {
        tmp = lcg2();
        memcpy(&key[0], &tmp, 8);
        tmp = lcg2();
        memcpy(&key[8], &tmp, 8);

        // for (int j = 0; j < 16; ++j)
        //     printf("%02hhx", key[j]);
        // putchar(10);

        AES_encrypt(buffer + i, key, output + i);
    }

    // for (i = 0; i < 128; ++i) {
    //     printf("%s ", table[i]);
    // }
    // putchar(10);

    base128_encode(output, len);

    puts("행복하기");
}

int main()
{
    scanf("%1000s", buffer);
}