# P-Shell

# Usage
- 공란

# Concept
- prime number 값들 필터링 Shellcoding

# Writeup

소수 바이트 값들은 필터링 되는데 이를 우회해서 쉘코딩을 작성하면 된다.
필터링 되는 수는 다음과 같다.
- 0x2 0x3 0x5 0x7 0xb 0xd 0x11 0x13 0x17 0x1d 0x1f 0x25 0x29 0x2b 0x2f 0x35 0x3b 0x3d 0x43 0x47 0x49 0x4f 0x53 0x59 0x61 0x65 0x67 0x6b 0x6d 0x71 0x7f 0x83 0x89 0x8b 0x95 0x97 0x9d 0xa3 0xa7 0xad 0xb3 0xb5 0xbf 0xc1 0xc5 0xc7 0xd3 0xdf 0xe3 0xe5 0xe9 0xef 0xf1 0xfb
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <math.h>

int is_prime(int n) {
    if (n < 2)
        return 0;
    int limit = (int)sqrt(n);
    for (int i = 2; i <= limit; i++) {
        if (n % i == 0)
            return 0;
    }
    return 1;
}

int main() {
    unsigned char input[4096];
    ssize_t input_len;

    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);

    printf("Input : ");

    input_len = read(STDIN_FILENO, input, sizeof(input));
    if (input_len <= 0) {
        perror("read error");
        exit(EXIT_FAILURE);
    }

    unsigned char *filtered = malloc(input_len);
    if (!filtered) {
        perror("malloc error");
        exit(EXIT_FAILURE);
    }
    int filtered_len = 0;
    for (int i = 0; i < input_len; i++) {
        if (!is_prime(input[i])) {
            filtered[filtered_len++] = input[i];
        }
    }

    if (filtered_len == 0) {
        printf("Error\n");
        free(filtered);
        exit(EXIT_FAILURE);
    }

    void *exec_mem = mmap(NULL, filtered_len, 
                          PROT_READ | PROT_WRITE | PROT_EXEC, 
                          MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (exec_mem == MAP_FAILED) {
        perror("mmap error");
        free(filtered);
        exit(EXIT_FAILURE);
    }

    memcpy(exec_mem, filtered, filtered_len);

    void (*shell_func)() = exec_mem;
    shell_func();

    munmap(exec_mem, filtered_len);
    free(filtered);
    return 0;
}
```