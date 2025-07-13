#include <stdio.h>
#include <setjmp.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

sigjmp_buf init_state;
jmp_buf state[12];
unsigned int *sys_num;
unsigned int r[15];
unsigned int* pc;
const int ans[] = {0x68, 0xe7, 0x1c2, 0x30b, 0x67, 0xcf, 0x1f2, 0x19f, 0x60, 0x8f, 0x19e, 0x193, 0x3d, 0x73, 0x146, 0x1c7, 0x6f, 0xe5, 0x166, 0x37b, 0x84, 0x57, 0x20a};
unsigned int code[0x1000];
const char* err_code = "\x0a\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x50\x00\x00\x00\x0b\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x0a\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x6c\x00\x00\x00\x0b\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x0a\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x65\x00\x00\x00\x0b\x00\x00\x00\x02\x00\x00\x00\x01\x00\x00\x00\x0a\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x61\x00\x00\x00\x0b\x00\x00\x00\x03\x00\x00\x00\x01\x00\x00\x00\x0a\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x73\x00\x00\x00\x0b\x00\x00\x00\x04\x00\x00\x00\x01\x00\x00\x00\x0a\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x65\x00\x00\x00\x0b\x00\x00\x00\x05\x00\x00\x00\x01\x00\x00\x00\x0a\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x20\x00\x00\x00\x0b\x00\x00\x00\x06\x00\x00\x00\x01\x00\x00\x00\x0a\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x69\x00\x00\x00\x0b\x00\x00\x00\x07\x00\x00\x00\x01\x00\x00\x00\x0a\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x6e\x00\x00\x00\x0b\x00\x00\x00\x08\x00\x00\x00\x01\x00\x00\x00\x0a\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x70\x00\x00\x00\x0b\x00\x00\x00\x09\x00\x00\x00\x01\x00\x00\x00\x0a\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x75\x00\x00\x00\x0b\x00\x00\x00\x0a\x00\x00\x00\x01\x00\x00\x00\x0a\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x74\x00\x00\x00\x0b\x00\x00\x00\x0b\x00\x00\x00\x01\x00\x00\x00\x0a\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x20\x00\x00\x00\x0b\x00\x00\x00\x0c\x00\x00\x00\x01\x00\x00\x00\x0a\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x63\x00\x00\x00\x0b\x00\x00\x00\x0d\x00\x00\x00\x01\x00\x00\x00\x0a\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x6f\x00\x00\x00\x0b\x00\x00\x00\x0e\x00\x00\x00\x01\x00\x00\x00\x0a\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x64\x00\x00\x00\x0b\x00\x00\x00\x0f\x00\x00\x00\x01\x00\x00\x00\x0a\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x65\x00\x00\x00\x0b\x00\x00\x00\x10\x00\x00\x00\x01\x00\x00\x00\x0a\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x21\x00\x00\x00\x0b\x00\x00\x00\x11\x00\x00\x00\x01\x00\x00\x00\x0a\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x0a\x00\x00\x00\x0b\x00\x00\x00\x12\x00\x00\x00\x01\x00\x00\x00\x0a\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x0a\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x00\x13\x00\x00\x00\x0a\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x0a\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x3c\x00\x00\x00\x01\x00\x00\x00";
unsigned char data[0x100];
unsigned int* ptr;
char zf = 0;
#define INT 1
#define ADD 2
#define SUB 3
#define MUL 4
#define DIV 5
#define BRS 6
#define BLS 7
#define JMP 8
#define LOAD 9
#define MOV 10
#define STR 11

void handler(int sig)
{
    switch ((int)sys_num)
    {
    case 0: //sys_read
        read(0, &data[r[1]], r[2]);        
        break;
    
    case 1: //sys_write        
        write(1, &data[r[1]], r[2]);
        break;
    case 60: //sys_exit
        exit(r[0]);
        break;
    }
    //IMPORTANT!!
    //siglongjmp 를 사용해서 handler 에서 탈출한 것으로 판단함
    siglongjmp(init_state, 1);
}

void interrupt()
{
    int result = setjmp(state[1]);
    if (result == 1)
    {
        #ifdef DEBUG
        printf("int %x\n", r[0]);
        #endif
        sys_num = r[0];
        *sys_num = 0;
    }
}

void add()
{
    if (setjmp(state[2]))
    {
        #ifdef DEBUG
        printf("add %x, %x\n", r[1], r[2]);
        #endif
        r[0] = r[1] + r[2];
        longjmp(init_state, 1);
    }
}

void sub()
{
    if (setjmp(state[3]))
    {
        #ifdef DEBUG
        printf("sub %x, %x\n", r[1], r[2]);
        #endif
        r[0] = r[1] - r[2];
        if (r[0] == 0)
            zf = 1;
        else
            zf = 0;
        longjmp(init_state, 1);
    }
}

void mul()
{
    if (setjmp(state[4]))
    {
        #ifdef DEBUG
        printf("mul %x, %x\n", r[1], r[2]);
        #endif
        r[0] = r[1] * r[2];
        longjmp(init_state, 1);
    }    
}

void divv()
{
    if (setjmp(state[5]))
    {
        #ifdef DEBUG
        printf("div %x, %x\n", r[1], r[2]);
        #endif
        r[0] = r[1] / r[2];
        r[1] = r[1] % r[2];
        longjmp(init_state, 1);
    }
}

void brs()
{
    if (setjmp(state[6]))
    {
        #ifdef DEBUG
        printf("brs %x, %x\n", r[1], r[2]);
        #endif
        r[0] = r[1] >> r[2];
        longjmp(init_state, 1);
    }
}

void bls()
{
    if (setjmp(state[7]))
    {
        #ifdef DEBUG
        printf("bls %x, %x\n", r[1], r[2]);
        #endif
        r[0] = r[1] << r[2];
        longjmp(init_state, 1);
    }
}

void jmp()
{
    if (setjmp(state[8]))
    {
        
        if (zf) 
        {
            #ifdef DEBUG
            printf("jmp + %d\n", r[0]);
            #endif
            pc += (int)r[0];
        }
        else
        {
            #ifdef DEBUG
            printf("skip jmp\n");
            #endif
        }
            
        longjmp(init_state, 1);
    }
}

void load()
{
    if (setjmp(state[9]))
    {
        if (*pc) //load from code
        {
            #ifdef DEBUG
            printf("load r[%x], %x\n", r[1], *pc);
            #endif
            r[r[1]] = *pc;
        }
        else // load from data section
        {
            #ifdef DEBUG
            printf("load r[%x], %x\n", r[3], data[r[4]]);
            #endif
            r[r[3]] = data[r[4]];
        }
        pc++;
        longjmp(init_state, 1);
    }
}

void mov()
{
    if (setjmp(state[10]))
    {
        if (*pc) //mov from code
        {
            pc++;
            unsigned int dst = *pc++;
            unsigned int src = *pc;
            #ifdef DEBUG
            printf("mov r[%x], %x\n", dst, src);
            #endif
            r[dst] = src;
        }
        else
        {
            #ifdef DEBUG
            printf("mov r[%x], r[%x]\n", r[3], r[4]);
            #endif
            r[r[3]] = r[r[4]];
        }        
        pc++;
        longjmp(init_state, 1);
    }
}

void store()
{
    if (setjmp(state[11]))
    {
        unsigned int dst = *pc++;
        unsigned int src = r[*pc];
        #ifdef DEBUG
        printf("store data[%x], %x\n", dst, src);
        #endif
        data[dst] = src;
        pc++;
        longjmp(init_state, 1);
    }
}


__attribute__((constructor)) 
void init()
{    
    struct sigaction sa;
    sa.sa_handler  = handler;
    sa.sa_flags = 0;
    sigemptyset(&(sa.sa_mask));
    sigaddset(&(sa.sa_mask), SIGSEGV);
    sigaction(SIGSEGV, &sa, NULL);
    interrupt();
    add();
    sub();
    mul();
    divv();
    bls();
    brs();
    jmp();
    load();
    store();
    mov();
    memcpy(data + 0x110, ans, sizeof(ans));
    
}

int main(int argc, char** argv)
{
    setbuf(stdout, 0);
    int fd = open("code.txt", 0);
    read(fd, code, sizeof(code));
    close(fd);
    if (fd > 0)
    {
        pc = code;
        sigsetjmp(init_state, 1);
        #ifdef DEBUG
        printf("pc : %d\n", pc - code);
        #endif
        longjmp(state[*pc++], 1);
    }
    else
    {
        pc = (unsigned int*)err_code;
        sigsetjmp(init_state, 1);
        longjmp(state[*pc++], 1);
    }

}

#ifdef DEBUG
void check_original(char* flag)
{
    int result[] = {0x68, 0xe7, 0x1c2, 0x30b, 0x67, 0xcf, 0x1f2, 0x19f, 0x60, 0x8f, 0x19e, 0x193, 0x3d, 0x73, 0x146, 0x1c7, 0x6f, 0xe5, 0x166, 0x37b, 0x84, 0x57, 0x20a};
    for (int i = 0; i < 24; i++)
    {
        int tmp = (flag[i] << (i % 4));
        tmp += i;
        if (result[i] != tmp)
            return;
    }
    puts("You win");
}
#endif
