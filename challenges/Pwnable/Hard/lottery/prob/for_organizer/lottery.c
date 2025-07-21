#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

void initialize();
void myPrint(char* str);
void printInt(unsigned int num);
int readInt();
void printMenu();
void getLuckyNum();
void readString(char* dst, size_t size);
void wish();
void draw();

unsigned int seed;
char state[12];
size_t* mem;
char tmpBuf[0x20];

int main()
{
    initialize();
    while(1)
    {
        printMenu();
        int sel = readInt();
        if (sel == 1)
        {
            wish();
        }
        else if (sel == 2)
        {
            draw();
        }
        else if (sel == 3)
        {
            getLuckyNum();
        }    
        else
            break;
    }
    myPrint("bye~\n");
}

void initialize()
{
    setbuf(stdout, 0);
    setbuf(stdin, 0);
    mem = (void*)mmap(0, 0x1000, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED)
    {
        myPrint("Wow.. :(\n");
        _exit(-1);
    }
    alarm(60);
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0)
    {
        myPrint("Opps.. :(\n");
        _exit(-1);
    }
    if (read(fd, &seed, 4) != 4)
    {
        myPrint("Oh.. :(\n");
        _exit(-1);
    }    
    rand();
    *(int*)state = 0;
    *(unsigned int*)(&state[4]) = seed;
    setstate(state);
    myPrint("Welcome to HS-lottery store!\n");
    close(fd);
}

void myPrint(char* str)
{
    int len = 0;
    char* ptr = str;
    while (*ptr)
        ptr++;
    len = ptr - str;
    write(1, str, len);
}

int readInt()
{
    char c[2];
    read(0, c, 2);
    return c[0] - '0'; 
}

void printInt(unsigned int num) 
{
    if (num == 0)
    {
        myPrint("0");
        return;
    }
    unsigned int original = num;
    size_t reversedNum = 0;
    while (num > 0) {
        unsigned int digit = num % 10;
        reversedNum = reversedNum * 10 + digit;
        num /= 10;
    }    
    while (reversedNum > 0) {
        unsigned int digit = reversedNum % 10;
        char tmp[2];
        tmp[1] = 0;
        tmp[0] = '0' + digit;
        myPrint(tmp);
        reversedNum /= 10;
    }
    if (original % 10 == 0)
        myPrint("0");
}

void printMenu()
{
    myPrint("1. Write a lottery Number\n"); //oob aaw     
    myPrint("2. Lottery draws.\n");
    myPrint("3. Get your lucky number.\n"); //libc leak 
    myPrint("4. Exit.\n");
    myPrint("> ");
}

void readString(char* dst, size_t size)
{
    if (size > sizeof(tmpBuf))
        return;
    for(int i = 0; i < size; i++)
        tmpBuf[i] = 0;
    read(0, tmpBuf, size);
    strcpy(dst, tmpBuf);
}

void wish()
{
    char buf[8] = {0,};
    long index;
    myPrint("Lottery index : ");
    index = readInt();
    if (index < 0 || index >= 0x200)
    {
        myPrint("Bad index :(\n");
        return;
    }
    myPrint("Lottery number : ");
    readString(buf, sizeof(buf)); 
    for (int i = 0; i < sizeof(buf); i++)
    {
        if (buf[i] == '\n')
            buf[i] = 0;
    }
    strcpy((char* )&mem[index], buf);
}

void draw()
{
    char comment[16];
    unsigned int ans = rand();
    for(int i = 0; i < 0x200; i++)
    {
        if (mem[i] == ans)
        {            
            myPrint("Congratulation.\nYou are super rich now!\n");
            myPrint("Any comment please : ");
            readString(comment, sizeof(comment));
            return;
        }
    }
    myPrint("That's too bad :(\n");
}

void getLuckyNum()
{
    myPrint("Your lucky number is ");
    printInt(rand());
    myPrint("\n");
}