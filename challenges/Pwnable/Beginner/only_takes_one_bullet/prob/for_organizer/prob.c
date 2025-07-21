#include <stdio.h>
#include <stdint.h>

int main() {
    uintptr_t* address;
    uintptr_t value;
    char monolog[10];
    
    printf("stdout addr : %p\n", stdout);

    puts("Load your bullet");
    scanf("%lx", &value);

    puts("Aim your target");
    scanf("%p", &address);

    puts("What's your cool monolog?");
    read(0, monolog, 9);
    
    puts("Trigger!");
    *address = value;
    
    puts(monolog);
    
    return 0;
}