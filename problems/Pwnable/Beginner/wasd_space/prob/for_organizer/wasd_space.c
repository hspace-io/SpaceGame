// gcc wasd_space.c -z execstack -fno-stack-protector -o wasd_space
// gcc wasd_space.c -z execstack -no-pie -fno-stack-protector -o wasd_space
// gcc double_jump_rev.c -z execstack -no-pie -fno-stack-protector -o double_jump_rev
// gcc new_jmp.c -z execstack -no-pie -fno-stack-protector -o new_jmp
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h> // rand()함수 포함 라이브러리
#include <time.h> // time()함수 포함 라이브러리
// 대충 shell들은 15길이 주고 buf는 안건들여도 될듯
char First_text[0x20] = "First_text";
// 0x404080
char First_name[15] = "IMFIRST";
// 0x4040A0
char Second_text[0x20] = "Second_text";
// 0x4040C0
char Second_name[15] = "IMSECOND";
// 004040E0
char Third_text[0x20] = "Third_text";
// 00404100
char Third_name[15] = "IMTHIRD";
// 00404120
// i mistake 0x10 10

int my_score = 0;
int First_score = 1500;
int Second_score = 1100;
int Third_score = 900;
int len,t_len;
int menu() {
    puts("JJR : jump jump revolution");
    puts("[1] : Start");
    puts("[2] : Rank");
    puts("[3] : My score");
    puts("[4] : Quit");
    puts("Choice > ");
}
int edit_rank() {
    char buf[30];
    char user_text[30];
    for (len = 0; len < 30; ++len) {
        buf[len] = 0;
    }
    len=0;
    t_len=0;
    puts("=============");
    puts("Edit rank");
    puts("=============");
    if (my_score > Third_score) {
        // puts("your name : ");
        printf("your name : ");
        read(0, buf, 0x30);
        len = strlen(buf)+1;
        checking(buf, len);



        printf("your text : ");
        read(0, user_text, 0x30);
        t_len = strlen(user_text)+1;
        checking(user_text, t_len);

        if (my_score > Second_score) {
            if (my_score > First_score) {
                puts("con~");
                puts("you are first");
                strncpy(First_name, buf, len);
                First_name[len - 1] = 0;

                strncpy(First_text, user_text, t_len);
                First_text[t_len - 1] = 0;

                First_score = my_score;
                return 0;
            }
            puts("con~");
            puts("you are second");
            strncpy(Second_name, buf, len);
            Second_name[len - 1] = 0;

            strncpy(Second_text, user_text, t_len);
            Second_text[t_len - 1] = 0;

            Second_score = my_score;
            return 0;
        }
        puts("cong~");
        puts("you are third");
        strncpy(Third_name, buf, len);
        Third_name[len - 1] = 0;

        strncpy(Third_text, user_text, t_len);
        Third_text[t_len - 1] = 0;

        Third_score = my_score;
        return 0;
    }
    return 0;
}

int start_game() {
    printf("start\n");
    my_score = 0;
    char count = 50;
    char key;
    char dict[10] = "wasd ";
    int random = 0; // 정수형 변수 선언
    while(count) {
        random = rand() % 5; // 난수 생성
        printf("key - \'%c\'\n", dict[random]);
        // printf("%d - \'%c\'\n", my_score, dict[random]);
        // fflush(stdin);
        key = getchar();
        getchar();
        if (dict[random] == key) {
            my_score += 100;
        }
        if(key == '!') {
            break;
        }
        count--;
    }
    puts("==========Game end");
    if (my_score > Third_score) {
        edit_rank();
    }
}
// int admin() {
//     char buf[30];


//     puts("First_name");
//     read(0, buf, 0x33);
//     my_score = strlen(buf)+1;
//     strncpy(First_name, buf, my_score);
//     First_name[my_score - 1] = 0;
//     // strncpy(First_name, buf, 0x33);
    
//     puts("Second_name");
//     read(0, buf, 0x33);
//     my_score = strlen(buf)+1;
//     strncpy(Second_name, buf, my_score);
//     Second_name[my_score - 1] = 0;
//     // strncpy(Second_name, buf, 0x33);
    

//     puts("Third_name");
//     read(0, buf, 0x33);
//     my_score = strlen(buf)+1;
//     strncpy(Third_name, buf, my_score);
//     Third_name[my_score - 1] = 0;
//     // strncpy(Third_name, buf, 0x33);
//     return 0;  

// }
int ranking() {
    puts("===============rank==============");
    printf("1. ");
    printf("%s : ", First_name);
    puts(First_text);
    printf("2. ");
    printf("%s : ", Second_name);
    puts(Second_text);
    printf("3. ");
    printf("%s : ", Third_name);
    puts(Third_text);
    puts("===============rank==============");
}
int checking(char *check_p, int l) {
    for (int i = 0; i < l; ++i) {
        // printf("%02x\n",check_p[i]);
        // if(*(check_p+i) == 0xe9) {
        if(check_p[i] == '\xc3' || check_p[i]=='\xc9') {
            puts("No");
            exit(0);
        }
    }
    return 0;
}
int main() {
    char c;
    mprotect(0x0404000, 0x1000, 0x7);
    srand(time(NULL)); // 매번 다른 시드값 생성
    while(1) {
        menu();
        fflush(stdin);
        // c = getchar();
        c = getchar();
        getchar();
        switch(c) {
        case '1':
            start_game();
            break;
        case '2':
            ranking();
            break;
        case '3':
            printf("\nMy score : %d\n",my_score);
            puts("");
            break;
        case '4':
            puts("Good bye");
            return 0;
            break;
        case 'e':
            edit_rank();
            break;
        case ';':
            puts("bye");
            return 1;
        // case '=':
        //     admin();
        //     return 1;
        default:
            puts("Invalid");
            break;
        }
    }
    return 0;
}


// in 64bit
// jmp esp;

// pop esp;

// add esp 8 스택정리

// sub esp 스택할당


// AAAAAAAA
// AAAAAAAA
// SFP_SFP_ for stack pivot?



// leave
// ret




// #include <stdio.h>

// int main() {
//     int n;
//     scanf("%d", &n);

//     int visit[26] = { 0, };
//     char s[101];
//     int ret = 0;
//     int k, pass;
//     for (int i = 0; i < n; ++i) {
//         scanf("%s", &s);
//         for (int j = 0; j < 26; ++j) {
//          // 초기화
//             visit[j] = 0;
//         }
//         k = 1;
//         pass = 1;
//         while (s[k]) {
//             if (visit[s[k] - 'a']) {
//                 pass = 0;
//                 break;
//             }
//             if (s[k] != s[k - 1]) {
//                 ++visit[s[k -  1] - 'a'];
//             }
//             ++k;
//         }

//         ret += pass;
//     }

//     printf("%d\n", ret);
//     return 0;
// }

// ret
// buf1
// buf2
// shellcode





// 0x404140 bss


// shellcode + NOP + SFP + jmp
// shellcode + NOP + SFP + rop

// 과학사 :)
// 과학사  - 꼭듣자람쥐

// 과기법
// 과학사
// 과학과 철학
