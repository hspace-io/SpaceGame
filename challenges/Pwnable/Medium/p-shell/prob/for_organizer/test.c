#include <stdio.h>
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
    for (int i = 2; i <= 255; i++) {
        if (is_prime(i)) {
            printf("%#x ", i);
        }
    }
    printf("\n");
    return 0;
}
