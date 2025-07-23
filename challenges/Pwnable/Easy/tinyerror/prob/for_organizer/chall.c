// gcc -o ./chall -fno-stack-protector -no-pie ./chall.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char input[200];

void get_shell(){
    execve("/bin/sh", 0, 0);
}

int interpret_hex()
{
    char buf[100];
    if(strlen(input) > 112){
        puts("Input too long!");
        exit(-1);
    }
    memcpy(buf, input, 113);
    return strtol(buf, NULL, 16);
}

int interpret_dec()
{
    char buf[100];
    if(strlen(input) > 112){
        puts("Input too long!");
        exit(-1);
    }
    memcpy(buf, input, 113);
    return atoi(buf);
}

int main()
{
    char buf[100];
    setvbuf(stdout, 0, 2, 0);
    setvbuf(stdin, 0, 2, 0);

    puts("Provide your input: ");
    read(0, input, 200);
    
    int dec = interpret_dec();
    printf("Interpreted as decimal: %d\n", dec);

    int hex = interpret_hex();
    printf("Interpreted as hex: %d\n", hex);
}