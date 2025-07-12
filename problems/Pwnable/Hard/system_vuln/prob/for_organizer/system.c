//gcc -o system system.c -lseccomp -no-pie -fomit-frame-pointer
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <seccomp.h>

void gadget(){
    asm("syscall");
    asm("ret");
}

void sandbox_init(){
    scmp_filter_ctx ctx = seccomp_init(SCMP_ACT_ALLOW);
    if (ctx == NULL) {
        perror("seccomp failed! call OSORI!");
        exit(-1);
    }
    seccomp_rule_add(ctx, SCMP_ACT_KILL, SCMP_SYS(socket), 0);    
    seccomp_load(ctx);
    seccomp_release(ctx);
    close(1);
    close(2);
}

int main() {
    setbuf(stdout, 0);    
    puts("[Super Safe Sandbox]");    
    void(*code)() = NULL;    
    code = mmap(NULL, 0x2000, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (code == MAP_FAILED){
        perror("mmap failed!");
        exit(-1);
    }
    printf("code > ");
    read(0, code, 0x2000);    
    sandbox_init();    
    code();  
    puts("You never escape from super safe sandbox");
    return 0;
}
