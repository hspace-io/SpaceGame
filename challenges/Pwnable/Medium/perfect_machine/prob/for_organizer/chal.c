//gcc -o chal chal.c -g -masm=intel 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dlfcn.h>
#include <seccomp.h>
#include <errno.h>
#include <sys/syscall.h>
#include <asm/prctl.h>

#define GREEN "\033[32m"
#define RESET "\033[0m"
#define RED   "\033[31m" 
#define YELLOW "\033[33m"
clock_t start_time;
__attribute__((aligned(0x1000))) size_t secret_memory[0x200];
int sandbox(void* addr);

void set_tf(int val)
{
    volatile unsigned long flags;
    __asm__ __volatile__ 
    (
        "pushf;"
        "pop %0;"
        :"=r"(flags)
    );
    if (val)
        flags |= 0x100;
    else
        flags &= ~0x100;
    __asm__ __volatile__ 
    (
        "push %0;"
        "popf;"
        :: "r"(flags)
    );
}

int check_read_access(void *addr) {        
    int page_size = getpagesize();                                                            
    void *aligned = (void *)((uintptr_t)addr & ~(page_size - 1));                           
    if (msync(aligned, page_size, MS_ASYNC) == -1 && errno == ENOMEM) {        
        return 0;
    }
    return 1;
}

const char* get_register_name(int reg_num) {
    switch(reg_num) {
        case 0:  return "r8";
        case 1:  return "r9";
        case 2:  return "r10";
        case 3:  return "r11";
        case 4:  return "r12";
        case 5:  return "r13";
        case 6:  return "r14";
        case 7:  return "r15";
        case 8:  return "rdi";
        case 9:  return "rsi";
        case 10: return "rbp";
        case 11: return "rbx";
        case 12: return "rdx";
        case 13: return "rax";
        case 14: return "rcx";
        case 15: return "rsp";
        case 16: return "rip";
        case 17: return "efl";
        case 18: return "csgsfs";
        case 19: return "err";
        case 20: return "trapno";
        case 21: return "oldmask";
        case 22: return "cr2";
        default: return "unknown";
    }
}

void handler(int sig, siginfo_t *info, ucontext_t *ucontext)   
{            
    ucontext_t *uc = ucontext;  
    size_t rip_addr = (size_t)uc->uc_mcontext.gregs[16];
    mprotect(secret_memory, 0x1000, PROT_WRITE);         
    if (secret_memory[2]){
        mprotect(secret_memory, 0x1000, PROT_NONE);   
        return;
    }
    if (rip_addr >= secret_memory[0x100] && rip_addr <= secret_memory[0x100] + 0x5000)
    {   
        if (secret_memory[1] == 0) //sandbox init
        {     
            secret_memory[1] = 1;          
        }
        if (sig != SIGTRAP)
        {        
            printf("%s[%lf]%s caught signal %d\n", RED, ((double)clock() - start_time) / 1000, RESET, sig);
            printf("%s[%lf]%s sandbox ends with an error. but it is safe!\n", GREEN, ((double)clock() - start_time) / 1000, RESET);
            _exit(1);
        }                     
        unsigned short* op = (unsigned short*)uc->uc_mcontext.gregs[16];    
        if (*op == 0x50f) {
            printf("%s[%lf]%s syscall is ignored in sandbox\n", YELLOW, ((double)clock() - start_time) / 1000, RESET);  
            if (uc->uc_mcontext.gregs[13] >= 3) {
                printf("%s[%lf]%s bad system call (%lld)\n", RED, ((double)clock() - start_time) / 1000, RESET, uc->uc_mcontext.gregs[13]);             
                printf("%s[%lf]%s you busted!\n", RED, ((double)clock() - start_time) / 1000, RESET);               
                _exit(1);
            }        
            uc->uc_mcontext.gregs[16] += 2;                
        }
        else if (*op ==0x80cd) {
            printf("%s[%lf]%s opcode == \"%s\"\n", RED, ((double)clock() - start_time) / 1000, RESET, "int 0x80");
            printf("%s[%lf]%s you busted!\n", RED, ((double)clock() - start_time) / 1000, RESET);
            _exit(1);                            
        }
        for (register int i = 0; i < 16; i++)
        {        
            char* reg_var = (char*)uc->uc_mcontext.gregs[i];                     
            if (check_read_access(reg_var)) {                           
                if (strstr(reg_var, "flag.txt") || strstr(reg_var, "proc")){                \
                    printf("%s[%lf]%s register %s == \"%s\"\n", RED, ((double)clock() - start_time) / 1000, RESET, get_register_name(i), reg_var);                            
                    printf("%s[%lf]%s you busted!\n", RED, ((double)clock() - start_time) / 1000, RESET);
                    _exit(1);
                }            
            }
        }

        size_t eflags = uc->uc_mcontext.gregs[17];
        if ((eflags | 0x100) != eflags) {
            printf("%s[%lf]%s you busted!\n", RED, ((double)clock() - start_time) / 1000, RESET);
            _exit(1);
        }
        eflags |= 0x100; 
        uc->uc_mcontext.gregs[17] = eflags;
    }
    else if (secret_memory[1])
    {
        if (rip_addr < 0x500000000000 || rip_addr >= 0x700000000000)
        {
            printf("%s[%lf]%s sandbox escaping detected\n", RED, ((double)clock() - start_time) / 1000, RESET);                            
            printf("%s[%lf]%s you busted!\n", RED, ((double)clock() - start_time) / 1000, RESET);
            _exit(1);
        }        
        secret_memory[1] = 0; //sandbox end
    }         
    mprotect(secret_memory, 0x1000, PROT_NONE);     
}

void init()
{    
    int fd = open("/dev/urandom", O_RDWR);
    int seed;
    read(fd, &seed, 4);
    srand(seed);
    close(fd);
    seed = 0;
    setbuf(stdout, 0);    
    start_time = clock();    
    printf("%s[%lf]%s  ╔═╗╔═╗╦═╗╔═╗╔═╗╔═╗╔╦╗  ╔╦╗╔═╗╔═╗╦ ╦╦╔╗╔╔═╗ \n", GREEN, ((double)clock() - start_time) / 1000, RESET);
    printf("%s[%lf]%s  ╠═╝║╣ ╠╦╝╠╣ ║╣ ║   ║   ║║║╠═╣║  ╠═╣║║║║║╣  \n", GREEN, ((double)clock() - start_time) / 1000, RESET);
    printf("%s[%lf]%s  ╩  ╚═╝╩╚═╚  ╚═╝╚═╝ ╩   ╩ ╩╩ ╩╚═╝╩ ╩╩╝╚╝╚═╝ \n", GREEN, ((double)clock() - start_time) / 1000, RESET);       
    puts("My system is too old to use seccomp filter! So I made this machine.\nCan you beat my perfect machine?");
}

int sandbox(void* addr) {        
    printf("%s[%lf]%s sandbox start\n",  GREEN,  ((double)clock() - start_time) / 1000, RESET);    
    struct sigaction sa;
    sa.sa_sigaction = (void (*)(int,  siginfo_t *, void *))handler;
    sa.sa_flags = SA_SIGINFO;
    void (*func)();
    if (sigaction(SIGSEGV, &sa, NULL) == -1) {
        perror("sigaction SIGSEGV");        
    }
    if (sigaction(SIGILL, &sa, NULL) == -1) {
        perror("sigaction SIGILL");
    }
    if (sigaction(SIGTRAP, &sa, NULL) == -1) {
        perror("sigaction SIGTRAP");
    }      
    printf("%s[%lf]%s status check machine enabled\n",  GREEN,  ((double)clock() - start_time) / 1000, RESET);
    *secret_memory = rand() % 0x10000; 
    secret_memory[10] = (size_t)addr;
    printf("%s[%lf]%s single step enabled\n",  GREEN,  ((double)clock() - start_time) / 1000, RESET);        
    printf("%s[%lf]%s test it %ld times\n",  GREEN,  ((double)clock() - start_time) / 1000, RESET, *secret_memory);    
    for (int i = 0; i < *secret_memory; i++)
    {        
        func = (void (*)())secret_memory[10];                   
        mprotect(secret_memory, 0x1000, PROT_NONE);
        set_tf(1);  
        asm("xor rax, rax \n\t"
        "xor rbx, rbx \n\t"
        "xor rcx, rcx \n\t"
        "xor rdx, rdx \n\t"
        "xor rdi, rdi \n\t"
        "xor rsi, rsi \n\t"
        "xor r8, r8 \n\t"
        "xor r9, r9 \n\t"
        "xor r10, r10 \n\t"
        "xor r11, r11\n\t"
        "xor r12, r12\n\t"
        "xor r13, r13\n\t"
        "xor r14, r14\n\t"
        "xor r15, r15\n\t");                
        func();                                           
        set_tf(0);
        mprotect(secret_memory, 0x1000, PROT_READ | PROT_WRITE);           
    }
    printf("%s[%lf]%s it seems to be safe!\n",  GREEN,  ((double)clock() - start_time) / 1000, RESET);     
    set_tf(0);
    mprotect(secret_memory, 0x1000, PROT_READ | PROT_WRITE);          
    func = (void (*)())secret_memory[10];      
    secret_memory[2] = 1;
    mprotect(secret_memory, 0x1000, PROT_NONE);           
    set_tf(1);
    asm("xor rax, rax \n\t"
    "xor rbx, rbx \n\t"
    "xor rcx, rcx \n\t"
    "xor rdx, rdx \n\t"
    "xor rdi, rdi \n\t"
    "xor rsi, rsi \n\t"
    "xor r8, r8 \n\t"
    "xor r9, r9 \n\t"
    "xor r10, r10 \n\t"
    "xor r11, r11\n\t"
    "xor r12, r12\n\t"
    "xor r13, r13\n\t"
    "xor r14, r14\n\t"
    "xor r15, r15\n\t");    
    func();     
    return 1;
}

int main() {
    init();
    void *addr = mmap(NULL, 0x5000, 
                           PROT_READ | PROT_WRITE | PROT_EXEC,
                           MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);   
    secret_memory[0x100] = (size_t)addr;                                                     
    printf("code > ");    
    read(0, addr, 0x5000);    
    mprotect(addr, 0x5000, PROT_READ | PROT_EXEC);
    sandbox(addr);
}