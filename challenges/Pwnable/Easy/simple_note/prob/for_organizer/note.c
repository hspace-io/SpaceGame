//gcc -o note note.c -Wl,-z,relro
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int read_int();
void printMenu();

int totalCount = 0;
char memo[0x10][8];

int main(int argc, char** argv)
{
    setbuf(stdin, 0);
    setbuf(stdout, 0);
    while(totalCount < 2)
    {
        printMenu();
        printf("> ");
        int select = read_int();
        if (select == 1)
        {
            printf("Select index : ");
            int idx = read_int();
            printf("Write your diary : ");
            read(0, &memo[idx], 8);
    
        }
        else if (select == 2)
        {
            printf("Select index : ");
            int idx = read_int();
            printf(memo[idx]);
        }
        else
        {
            break;
        }
        totalCount++;
    }
    return 0;
}

void printMenu()
{
    puts("1. Write diary");
    puts("2. read diary");
    puts("3. exit");
}

int read_int()
{
    char num[4];
    read(0, &num, 4);
    return atoi(num);
}