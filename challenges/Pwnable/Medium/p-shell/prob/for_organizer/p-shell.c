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
