// [challenge name] : Surprising Painful Infrastructure
// [author] : m0nd2y
// [team] : physical lab
// [date] : 27/OCT/2024

#include "chall.h"

void start_init() {
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
}

int main()
{
    int input;

    start_init();
    SPI_Mode mode = setmode();

    AES_Config aes = {{0}};
    SPI_Master_Config master = {{0},{0},{0},{0},{0},{0}};
    SPI_Slave_Config slaveA = {{0},{0},{0},{0}}, slaveB = {{0},{0},{0},{0}}, slaveC = {{0},{0},{0},{0}};

    init(mode, &aes, &master, &slaveA, &slaveB, &slaveC);
    while(1) {
        print_main_menu();
        scanf("%d", &input);
        switch (input) {
            case 1:
                status(mode, &master);
                break;
            case 2:
                execute(&aes, &master, &slaveA, &slaveB, &slaveC);
                break;
            case 3:
                config(&master);
                break;
            case 4:
                getflag(&slaveA, &slaveB, &slaveC);
                break;
            case 5:
                printf("good bye~");
                return 0;
                break;
            default :
                printf("wrong input :(\n");
                return 0;
                break;
        }
    }
    return 0;
}
