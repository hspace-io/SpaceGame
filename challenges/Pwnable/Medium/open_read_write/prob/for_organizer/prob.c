#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <linux/seccomp.h>
#include <linux/filter.h>
#include <linux/audit.h>
#include <sys/syscall.h>
#include <stddef.h>

char init_reg[] = {
    "\x48\x31\xc0"
    "\x48\x31\xdb"
    "\x48\x31\xc9"
    "\x48\x31\xd2"
    "\x48\x31\xed"
    "\x48\x31\xe4"
    "\x48\x31\xff"
    "\x48\x31\xf6"
    "\x4d\x31\xc0"
    "\x4d\x31\xdb"
    "\x4d\x31\xc9"
    "\x4d\x31\xd2"
    "\x4d\x31\xed"
    "\x4d\x31\xe4"
    "\x4d\x31\xff"
    "\x4d\x31\xf6"
};

typedef struct {
    long start;
    long end;
    int len;
    char* perms;
} _maps;

typedef void (*func_ptr_t)(void);

_maps TO_BE_REMOVED[1000];
char remove_maps[0x1000];
void *MAPS[1000];
unsigned int RWX_IDX;
unsigned int RW_IDX;
long CNT;

void setup();
void gen_map();
void install_syscall_filter();
void go();

int main()
{
    setup();
    go();
}

void gen_map()
{
    int i;
    int prot;
    int rand_fd;
    void *ptr;

    rand_fd = open("/dev/urandom", 0);

    if ( rand_fd == -1 ) {
        puts("[-] ....");
        exit(-1);
    }

    read(rand_fd, &RWX_IDX, 4);
    read(rand_fd, &RW_IDX, 4);

    RWX_IDX %= 1000;
    RW_IDX %= 1000;

    if ( RWX_IDX == RW_IDX ) RW_IDX = (RW_IDX + 0x1337) % 1000;

    i = 0;
    while ( i < 1000 )
    {
        prot = 0;
        read(rand_fd, &ptr, 8);
        if ( i == RWX_IDX ) prot = PROT_WRITE | PROT_READ | PROT_EXEC;
        if ( i == RW_IDX ) {
            prot |= PROT_READ | PROT_WRITE;
            *(long*)&ptr = *(long*)&ptr % 0xFFFFFF000 + 0x200000000000;
        }

        *(long*)&ptr &= 0x7FFFFFFFF000;

        ptr = mmap(ptr, 0x4000, prot, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

        if ( *(long*)&ptr == MAP_FAILED) continue;
            MAPS[i] = *(long*)&ptr;
        ++i;
    }
    sleep(1);
}

void setup()
{
    setvbuf(stdin, NULL, 2, 0);
    setvbuf(stdout, NULL, 2, 0);
    setvbuf(stderr, NULL, 2, 0);
    puts("[+] generating...");
    gen_map();
    puts("[*] done!");
}

void install_syscall_filter() {
    // struct sock_filter filter[] = {
    //     BPF_STMT(BPF_LD | BPF_W | BPF_ABS, (offsetof(struct seccomp_data, arch))),
    //     BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, AUDIT_ARCH_X86_64, 0, 12),
    //     BPF_STMT(BPF_LD | BPF_W | BPF_ABS, (offsetof(struct seccomp_data, nr))),
    //     BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_getdents, 11, 0),
    //     BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_read, 10, 0),
    //     BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_write, 9, 0),
    //     BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_exit, 8, 0),
    //     BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_mprotect, 0, 2),
    //     BPF_STMT(BPF_LD | BPF_W | BPF_ABS,
    //              offsetof(struct seccomp_data, args[2])),
    //     BPF_JUMP(BPF_JMP | BPF_JSET | BPF_K, PROT_WRITE, 0, 5),
    //     BPF_STMT(BPF_LD | BPF_W | BPF_ABS, (offsetof(struct seccomp_data, nr))),
    //     BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_openat, 0, 2),
    //     BPF_STMT(BPF_LD | BPF_W | BPF_ABS,
    //              offsetof(struct seccomp_data, args[2])),
    //     BPF_JUMP(BPF_JMP | BPF_JSET | BPF_K, O_CREAT, 0, 1),
    //     BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_KILL),
    //     BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_ALLOW),
    // };

    struct sock_filter filter[] = {
        BPF_STMT(BPF_LD | BPF_W | BPF_ABS, (offsetof(struct seccomp_data, arch))),
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, AUDIT_ARCH_X86_64, 0, 17),
        BPF_STMT(BPF_LD | BPF_W | BPF_ABS, (offsetof(struct seccomp_data, nr))),
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_getdents, 16, 0),
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_write, 15, 0),
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_exit, 14, 0),
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_mprotect, 0, 2),
        BPF_STMT(BPF_LD | BPF_W | BPF_ABS,
                 offsetof(struct seccomp_data, args[2])),
        BPF_JUMP(BPF_JMP | BPF_JSET | BPF_K, PROT_WRITE, 10, 11),
        BPF_STMT(BPF_LD | BPF_W | BPF_ABS, (offsetof(struct seccomp_data, nr))),
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_openat, 0, 2),
        BPF_STMT(BPF_LD | BPF_W | BPF_ABS,
                 offsetof(struct seccomp_data, args[2])),
        BPF_JUMP(BPF_JMP | BPF_JSET | BPF_K, O_CREAT, 6, 7),
        BPF_STMT(BPF_LD | BPF_W | BPF_ABS, (offsetof(struct seccomp_data, nr))),
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, SYS_read, 0, 4),
        BPF_STMT(BPF_LD | BPF_W | BPF_ABS,
                 offsetof(struct seccomp_data, args[0])),
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, 0, 2, 0),
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, 1, 1, 0),
        BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, 2, 0, 1),
        BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_KILL),
        BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_ALLOW),
    };

    struct sock_fprog prog = {
        .len = (unsigned short)(sizeof(filter) / sizeof(filter[0])),
        .filter = filter,
    };

    if (prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0)) {
        perror("prctl(PR_SET_NO_NEW_PRIVS)");
        exit(1);
    }

    if (prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &prog)) {
        perror("prctl(PR_SET_SECCOMP)");
        exit(1);
    }
}

void go()
{
    memcpy(MAPS[RWX_IDX], init_reg, sizeof(init_reg));
    printf("[+] ???: ");
    read(0, (MAPS[RWX_IDX] + sizeof(init_reg) - 1), 0x400);
    close(0);
    install_syscall_filter();
    mprotect(MAPS[RWX_IDX], 0x4000, PROT_EXEC);
    MAPS[RW_IDX] = NULL;
    ((func_ptr_t)MAPS[RWX_IDX])();
    puts("[+] I found the escaping impossible :(");
    exit(-1);
}
