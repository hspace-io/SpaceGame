#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

///home/arkea/hspcae/bug_hunt/ctf

//char flag[46] = {0x76, 0x39, 0x54, 0x78, 0x26, 0x79, 0x2a, 0x65, 0x38, 0x27, 0x72, 0x77, 0x58, 0x67, 0x78, 0x6a, 0x35, 0x26, 0x45, 0x73, 0x64, 0x64, 0x71, 0x2e, 0x62, 0x32, 0x61, 0x59, 0x64, 0x71, 0x26, 0x24, 0x27, 0x74, 0x24, 0x28, 0x6b, 0x43, 0x4e, 0x36, 0x3c, 0x54, 0x49, 0x29, 0x40};
char flag[46] ={0,};
char fake_flag[46] = {0xc, 0x1e, 0x8, 0x1d, 0xc, 0x2e, 0x14, 0x7c, 0x1, 0x47, 0x14, 0x11, 0xb, 0x2d, 0x5b, 0x6, 0x1d, 0x1f, 0x47, 0x5d, 0x41, 0x37, 0xa, 0x52, 0x47, 0x57, 0x53, 0x17, 0x1, 0x1a, 0x5c, 0x44, 0x5c, 0x47, 0x50, 0x67, 0x43, 0x12, 0x16, 0x2b, 0x16, 0x17, 0x6, 0x44, 0x5c};
char check[35] = {0x3f, 0x41, 0x0, 0x0, 0x36, 0x1a, 0x8, 0x0, 0x7, 0xe, 0x42, 0x1a, 0x3a, 0x1a, 0x1d, 0x1b, 0x59, 0x1d, 0x7, 0x57, 0x38, 0x32, 0x59, 0x12, 0x7, 0x47, 0x17, 0x2d, 0xb, 0x19, 0x5a, 0x7, 0x1a, 0x51};
char xor_table[15] = {0x68, 0x75, 0x6e, 0x74, 0x69, 0x6e, 0x67, 0x5f, 0x6d, 0x61, 0x73, 0x74, 0x65, 0x72};
char ar_pw[35] = {0x6f, 0x70, 0x65, 0x6e, 0x73, 0x73, 0x6c, 0xa, 0x61, 0x72, 0x69, 0x61, 0x2d, 0x31, 0x32, 0x38, 0x2d, 0x63, 0x62, 0x63, 0xa, 0x70, 0x77, 0x3a, 0x20, 0x77, 0x65, 0x6c, 0x63, 0x6f, 0x6d, 0x65, 0xa};

void out()
{
    FILE * ari;
    ari = fopen(".aria", "w+");
    fwrite(ar_pw, sizeof(char), 33, ari);
    fclose(ari);
    exit(0);
}

void calc()
{
    int j = 0;
    for(int i = 0; i < 34; i++){
        fake_flag[i] ^= xor_table[j];
        j++;
        if(j == 14) j= 0;
    }
}

//enc, dec
void sub_rout()
{
    FILE * aria;

    aria = fopen("aria", "r");

    fread(flag, sizeof(char), 45, aria);

    fclose(aria);
}

//swap flag
void swap()
{
    int temp;
    char input[20];

    for(int i = 0; i < 45; i++){

        temp = flag[(i + 1)%45];
        flag[(i+1)%45] = flag[i];
        flag[i] = temp;

        temp = flag[(i + 13)%45];
        flag[(i+13)%45] = flag[i];
        flag[i] = temp;

        flag[i] = temp;
    }

}


void func1()
{
    swap();

    printf("process 2 end\n");
}

//aes decrypt
void func2()
{
    sub_rout();

    printf("process 1 end\n");
}

int main()
{
    char input[100] ={0, };
    char key[20] = {0, };
    int j = 0, cmp = -1;

    printf("Do you want flag? [Y/N] ");
    scanf("%s", input);

    if(strncmp("bug_hunt", input, 8) == 0) goto label1;

    if(strncmp("Y", input, 1) == 0 || strncmp("y", input, 1) == 0 || strncmp("N", input, 1) == 0 || strncmp("n", input, 1) == 0){
        if(strncmp(input, "Y", 1) == 0 || strncmp(input, "y", 1) == 0){
            calc();
            printf("huntingmaster{%s}\n", fake_flag);
            sleep(1);
            system("clear");
            exit(0);
        }

        else if(strncmp(input, "N", 1) == 0 || strncmp(input, "n", 1) == 0){
            printf("bye\n");
            exit(0);
        }
    }

    else exit(0);


    label1:

    printf("Input: ");
    scanf("%s", key);

    if(strncmp("aria", key, 5) == 0){
        out();
    }

    for(int i = 0; i < strlen(key); i++){
        key[i] ^= xor_table[j];
        j++;
        if(j == 14) j == 0;
    }

    cmp = strncmp(check, key, 34);

    if(cmp != 0){
        for(int i = 0; i < 45; i++){
            fake_flag[i] ^= xor_table[j];
            j++;
            if(j == 14) j == 0;
        }

        printf("%s\n", fake_flag);
        exit(1);
    }



    system("clear");
    printf("Process 1 start\n");
    sleep(4);
    func1();


    sleep(1);

    system("clear");
    printf("process 2 start\n");
    sleep(2);
    func2();


    system("clear");
    

    printf("Welcome!\n");
    printf("huntingmaster{%s}\n", flag);


    return 0;
}