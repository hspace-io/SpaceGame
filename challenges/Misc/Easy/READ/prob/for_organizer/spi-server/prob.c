// gcc -o prob prob.c
#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#define CS 0
#define DO 1
#define HOLD 2
#define WP 3
#define VSS 4
#define VCC1 5
#define VCC2 6
#define RP 7
#define CLK 8
#define DI 9

int pin[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void proc_init ();
void chipsetStatus();
void serialList();
void setPin();
void menu();
void firmware();
int readData();

void init()
{
  setvbuf(stdin, 0, 2, 0);
  setvbuf(stdout, 0, 2, 0);
  setvbuf(stderr, 0, 2, 0);
}

int main(){
    init();
    printf("\n        Welcome to Hardware CTF\n");
    while(1){
        chipsetStatus();
        serialList();
        menu();
    }
}

void chipsetStatus(){
    int choice = 0;
    printf("\n");
    printf("        ____________________\n");
    printf("     __｜                  ｜__\n");
    printf("  %02d｜1｜                  ｜6｜%02d\n", pin[CS], pin[VCC1]);
    printf("     ￣｜                  ｜￣\n");
    printf("     __｜                  ｜__\n");
    printf("  %02d｜2｜                  ｜7｜%02d\n", pin[DO], pin[VCC2]);
    printf("     ￣｜                  ｜￣\n");
    printf("     __｜                  ｜__\n");
    printf("  %02d｜3｜    PL25SL128C    ｜8｜%02d\n", pin[HOLD], pin[RP]);
    printf("     ￣｜                  ｜￣\n");
    printf("     __｜                  ｜__\n");
    printf("  %02d｜4｜                  ｜9｜%02d\n", pin[WP], pin[CLK]);
    printf("     ￣｜                  ｜￣\n");
    printf("     __｜                  ｜__\n");
    printf("  %02d｜5｜                  ｜10｜%02d\n", pin[VSS], pin[DI]);
    printf("     ￣｜                  ｜￣\n");
    printf("        ￣￣￣￣￣￣￣￣￣￣\n");
    printf("\n");
}

void menu(){
    int choice;
    printf(" [*] Menu\n");
    printf(" 1. Set SerialPort\n");
    printf(" 2. Firmware Extract\n");
    printf(" > ");
    scanf("%d", &choice);
    switch(choice){
        case 1:
            setPin();
            break;
        case 2:
            firmware();
            break;
        default:
            break;
    }
}

void setPin(){
    int sopIndex, serialNumber;
    printf(" [*] Input SOP 8PIN INDEX\n > ");
    scanf("%d", &sopIndex);
    printf(" [*] Input Serial Number\n > ");
    scanf("%d", &serialNumber);
    pin[sopIndex-1] = serialNumber;
    printf("==============================\n");
    printf(" Port Set - %d : %d\n", sopIndex, serialNumber);
    printf("==============================\n");
}

void serialList(){
    printf(" [*] Serial Port List\n");
    printf(" PIN   NAME              PIN   NAME\n");
    printf(" ―――――――――――――――――――――――――――――――――――――――――――――――\n");
    printf(" (01) 3.3V DC Power      (02) 3.3V DC Power\n");
    printf(" (03) 5V DC Power        (04) 5V DC Power\n");
    printf(" (05) GPIO03(SDA1)       (06) GPIO04(SDL1)\n");
    printf(" (07) Ground             (08) GPIO06(TXD0, UART)\n");
    printf(" (09) GPIO07(GPCLK0)     (10) GPIO08(RXD0, UART)\n");
    printf(" (11) GPIO09(SPI0_CLK)   (12) GPIO10(SPI10_MISO)\n");
    printf(" (13) GPIO11(PWM0)       (14) GPIO12(SPI10_MOSI)\n");
    printf(" (15) GPIO13(SPI_CE0_N)  (16) GPIO14(SDA0)      \n");
    printf(" (17) GPIO15(SCL0) \n");
    printf("\n");
}

void firmware(){
    int op;
    char re[2];
    printf(" [*] Input Opcode(Hex)\n > ");
    scanf("%x", &op);
    if(!(pin[CS]==15 && pin[DO]==12 && pin[VSS]==7 && (pin[VCC1]==1 || pin[VCC1]==2) && ((pin[VCC2]==3 && pin[RP]==4) || (pin[VCC2]==4 && pin[RP]==3)) && pin[CLK]==11 && pin[DI]==14)){
        printf("No EEPROM/flash device found.\n");
        printf("Note: flashrom can never write if the flash chip isn't found automatically.\n");
        exit(0);
    }
    switch(op){
        case 0x03:
            readData();
            break;
        case 0x9f:
            printf("=========== Read Identification ===========\n");
            printf(" ['0x4D', '0x30', '0x15'] \n");
            printf("===========================================\n");
            break;
        case 0x90:
            printf("=========== Manufacturer/Device ID ===========\n");
            printf(" ['0x00', '0x00', '0x00', '0x4D', '0x42'] \n");
            printf("==============================================\n");
            break;
        case 0xb9:
            printf("=========== Read Identification ===========\n");
            printf(" Deep Power-down  \n");
            printf(" ['0x00', '0x00', '0x00', '0x4D', '0x42'] \n");
            printf("===========================================\n");
            break;
        case 0xab:
            printf("=== Release from Deep Power-down ===\n");
            printf("============ Device ID =============\n");
            printf(" ['0x00', '0x00', '0x00', '0x42'] \n");
            printf("====================================\n");
            break;
        default:
            printf("==============================\n");
            printf(" opcode is not supported  :( \n");
            printf(" Please enter Read Data opcode.\n");
            printf("==============================\n");
            break;
    }
    printf(" [*] Continue? (y/N)\n > ");
    read(0, re, 2);
    if (re[0]!='Y' && re[0]!='y'){
        printf(" [*] exit...\n");
        exit(0);
    }
}


int readData() {
    FILE *file;
    char *buffer;
    long file_size;
    // char ch[2];

    // printf(" [*] Are you want to extract firmware? (y/N)\n > ");
    // read(0, ch, 2);

    // if (ch[0]!='Y' && ch[0]!='y'){
    //     printf(" [*] exit...\n");
    //     exit(0);
    // }

    printf(" [*] Wait a few minutes");
    for (int i = 0; i < 8; i++) {
        sleep(1);
        printf(".");
    }
    printf("\n [*] Firmware Extract Complete!");
    printf("\n =========== Firmware Download Link ===========\n");
    printf("  war.hspace.io:21345/.7efd073b/HS-PL21345v3.bin");
    printf("\n ==============================================\n");

    return 0;
}


