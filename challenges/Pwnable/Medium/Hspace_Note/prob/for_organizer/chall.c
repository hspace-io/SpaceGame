#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char* pages[20];

void add()
{
    int idx, lines;
    char buf[0x100];
    printf("Page: ");
    scanf("%d", &idx);
    if (idx < 0 || idx >= 20){
        puts("Invalid index");
        return;
    }
    printf("Lines: ");
    scanf("%d", &lines);
    char* temp = (char*)malloc(lines*0x10ULL);
    if (temp == NULL){
        puts("malloc failed");
        return;
    }
    pages[idx] = temp;
    printf("Note: ");
    read(0, pages[idx], lines*0x10ULL);
}

void edit()
{
    int idx;
    printf("Page: ");
    scanf("%d", &idx);
    if (idx < 0 || idx >= 20){
        puts("Invalid index");
        return;
    }
    if (pages[idx] == NULL){
        puts("Invalid index");
        return;
    }
    char* cur = pages[idx];
    printf("Line: ");
    scanf("%d", &idx);
    cur += 0x10ULL*idx;
    printf("Note: ");
    read(0, cur, 0x10ULL);
}

void show()
{
    int idx;
    printf("Page: ");
    scanf("%d", &idx);
    if (idx < 0 || idx >=20){
        puts("Invalid index");
        return;
    }
    if (pages[idx] == NULL){
        puts("Invalid index");
        return;
    }
    char* cur = pages[idx];
    printf("Line: ");
    scanf("%d", &idx);
    cur += 0x10ULL*idx;
    printf("%s", cur);
}

void menu()
{
    puts("1. Add");
    puts("2. Edit");
    puts("3. Show");
    puts("4. Exit");
}

void init()
{
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
}

int main()
{
    init();

    while (1){
        menu();
        int choice;
        printf(">> ");
        scanf("%d", &choice);
        switch (choice){
            case 1:
                add();
                break;
            case 2:
                edit();
                break;
            case 3:
                show();
                break;
            case 4:
                exit(0);
                break;
            default:
                puts("Invalid choice");
                break;
        }
    }
}