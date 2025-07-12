#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>

void run_shellcode(unsigned char *shellcode) {
    void *exec_mem = mmap(0, 0x1000, PROT_READ | PROT_WRITE | PROT_EXEC, 
                          MAP_ANON | MAP_PRIVATE, -1, 0);

    if (exec_mem == MAP_FAILED) {
        perror("mmap");
        return;
    }

    memcpy(exec_mem, shellcode, 0x100);

    void (*func)() = exec_mem;
    
    func();

}

int main() {
    unsigned char shellcode[0x100];
    read(0, shellcode, sizeof(shellcode));
    run_shellcode(shellcode);

    return 0;
}
