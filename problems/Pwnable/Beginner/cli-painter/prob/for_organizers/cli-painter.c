#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define WIDTH 25
#define HEIGHT 25
#define MAP_SIZE ((WIDTH * HEIGHT + 7) / 8)  

struct termios orig_termios;

void init() {
}

void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
    struct termios raw;
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disableRawMode);

    raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

11111111 00000000 11111111 00000000

void printGrid(unsigned char *map, int x, int y) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int index = i * WIDTH + j;
            if ((i==y && j==x)) {
                printf("◎");
            } else if (map[index / 8] & (1 << (index % 8))) {
                printf("▓");  
            } else {
                printf("░");  
            }
        }
        printf("\n");
    }
}


void game() {
    unsigned char map[MAP_SIZE] = {
        28, 0, 112, 56, 0, 224, 112, 0, 192, 225, 0, 128, 195, 253, 127, 135, 255, 255, 15, 255, 255, 63, 62, 0, 124, 62, 0, 240, 60, 0, 224, 120, 0, 192, 241, 255, 255, 227, 255, 255, 135, 255, 255, 15, 63, 0, 28, 254, 255, 59, 220, 255, 127, 56, 255, 255, 113, 0, 224, 227, 0, 128, 199, 1, 128, 239, 255, 255, 207, 255, 255, 159, 255, 255, 59, 28, 0, 112, 0
    };

    int x = 0;
    int y = 0;
    char ch;

    enableRawMode();
    printGrid(map, x, y);
    int brk = 0;
    while (1) {
        ch = getchar();
        int index = y * WIDTH + x;

        switch (ch) {
            case 'w': y--; break;
            case 's': y++; break;
            case 'a': x--; break;
            case 'd': x++; break;
            case ' ':  
                map[index / 8] ^= (1 << (index % 8));
                break;
            case 'q':  
                disableRawMode();
                brk = 1;
                break;
            default:
                continue;
        }
        printf("\033[H\033[J"); 
        printGrid(map, x, y);
        if(brk) break;
    }
}

void win() {
    system("/bin/sh");
} 

int main(void) {
    init();
    game();
}

// rbp - 0x70
// rbp+8

0x78