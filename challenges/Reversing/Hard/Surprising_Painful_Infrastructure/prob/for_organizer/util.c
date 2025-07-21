#include "chall.h"

int ger_random_zero_or_one() {
    return rand();
}

int setmode() {
    int CPOL = 0, CPHA = 0;
    srand(time(NULL));

    CPOL = ger_random_zero_or_one() % 2;
    CPHA = ger_random_zero_or_one() % 2;

    if (CPOL == 0 && CPHA == 0) {
        return 1;
    }
    else if (CPOL == 0 && CPHA == 1) {
        return 2;
    }
    else if (CPOL == 1 && CPHA == 0) {
        return 3;
    }
    else if (CPOL == 1 && CPHA == 1) {
        return 4;
    }
    return 0;
}

void generate_random_bytes(uint8_t *buffer, size_t size) {
    for (size_t i = 0; i < size; i++) {
        buffer[i] = rand() % 256;
    }
}

void string_to_bits(const char *str, uint8_t *buffer, size_t size) {
    size_t i = 0;
    while (*str && i < size) {
        buffer[i] = 0;
        for (int bit = 0; bit < 8 && *str; bit++) {
            if (*str == '1') {
                buffer[i] |= (1 << (7 - bit));
            }
            str++;
        }
        i++;
    }
    for (; i < size; i++) {
        buffer[i] = rand() % 256;
    }
}

void config(SPI_Master_Config *master) {
    int select;
    char input[129];
    printf("[~/Main/config]\n");
    printf("1. MOSI\n");
    printf("2. MISO\n");
    printf("3. SCK\n");
    printf("4. CS1\n");
    printf("5. CS2\n");
    printf("6. CS3\n");
    scanf("%d", &select);
    switch(select) {
        case 1 :
            printf("Enter new value (binary string): \n");
            scanf("%128s", input);
            input[strcspn(input, "\n")] = 0;
            string_to_bits(input, master->MOSI, AES_KEY_SIZE_256/2);
            break;
        case 2 :
            printf("[+] You can'y change this line :(\n");
            break;
        case 3 :
            printf("Enter new value (binary string): \n");
            scanf("%128s", input);
            input[strcspn(input, "\n")] = 0;
            string_to_bits(input, master->SCK, AES_KEY_SIZE_256/2);
            break;
        case 4 :
            printf("Enter new value (binary string): \n");
            scanf("%128s", input);
            input[strcspn(input, "\n")] = 0;
            string_to_bits(input, master->CS1, AES_KEY_SIZE_256/2);
            break;
        case 5 :
            printf("Enter new value (binary string): \n");
            scanf("%128s", input);
            input[strcspn(input, "\n")] = 0;
            string_to_bits(input, master->CS2, AES_KEY_SIZE_256/2);
            break;
        case 6 :
            printf("Enter new value (binary string): \n");
            scanf("%128s", input);
            input[strcspn(input, "\n")] = 0;
            string_to_bits(input, master->CS3, AES_KEY_SIZE_256/2);
            break;
        default :
            printf("no hack ~.~");
            exit(1);
            break;
    }
}

void print_main_menu()
{
    printf("[~/Main]\n");
    printf("1. status\n");
    printf("2. execute\n");
    printf("3. config\n");
    printf("4. getflag\n");
    printf("5. exit\n");
}

void init(int mode, AES_Config *aes, SPI_Master_Config *master, SPI_Slave_Config *slaveA, SPI_Slave_Config *slaveB, SPI_Slave_Config *slaveC) {
    generate_random_bytes(aes->key, AES_KEY_SIZE_256);

    generate_random_bytes(master->MOSI, AES_KEY_SIZE_256/2);
    generate_random_bytes(master->MISO, AES_KEY_SIZE_256/2);
    generate_random_bytes(master->SCK, AES_KEY_SIZE_256/2);
    generate_random_bytes(master->CS1, AES_KEY_SIZE_256/2);
    generate_random_bytes(master->CS2, AES_KEY_SIZE_256/2);
    generate_random_bytes(master->CS3, AES_KEY_SIZE_256/2);

    generate_random_bytes(slaveA->MOSI, AES_KEY_SIZE_256/2);
    memcpy(slaveA->MISO, aes->key, AES_KEY_SIZE_256/2);
    generate_random_bytes(slaveA->CS, AES_KEY_SIZE_256/2);

    generate_random_bytes(slaveB->MOSI, AES_KEY_SIZE_256/2);
    memcpy(slaveB->MISO, aes->key + AES_KEY_SIZE_256/4, AES_KEY_SIZE_256/2);
    generate_random_bytes(slaveB->CS, AES_KEY_SIZE_256/2);

    generate_random_bytes(slaveC->MOSI, AES_KEY_SIZE_256/2);
    memcpy(slaveC->MISO, aes->key + AES_KEY_SIZE_256/2, AES_KEY_SIZE_256/2);
    generate_random_bytes(slaveC->CS, AES_KEY_SIZE_256/2);

    switch(mode) {
        case 1 :
            memset(slaveA->SCK, 0x55, AES_KEY_SIZE_256/2);
            memset(slaveB->SCK, 0x55, AES_KEY_SIZE_256/2);
            memset(slaveC->SCK, 0x55, AES_KEY_SIZE_256/2);
            break;
        case 2 :
            memset(slaveA->SCK, 0x55, AES_KEY_SIZE_256/2);
            memset(slaveB->SCK, 0x55, AES_KEY_SIZE_256/2);
            memset(slaveC->SCK, 0x55, AES_KEY_SIZE_256/2);
            break;
        case 3 :
            memset(slaveA->SCK, 0xAA, AES_KEY_SIZE_256/2);
            memset(slaveB->SCK, 0xAA, AES_KEY_SIZE_256/2);
            memset(slaveC->SCK, 0xAA, AES_KEY_SIZE_256/2);
            break;
        case 4 :
            memset(slaveA->SCK, 0xAA, AES_KEY_SIZE_256/2);
            memset(slaveB->SCK, 0xAA, AES_KEY_SIZE_256/2);
            memset(slaveC->SCK, 0xAA, AES_KEY_SIZE_256/2);
            break;
        default :
            printf("no hack ~.~");
            exit(1);
            break;
    }
}

void status(int mode, SPI_Master_Config *master) {
    printf("[~/Main/status]\n");
    printf("[+] Mode : %d\n", mode);
    printf("[+] Master\n");

    printf("- Master MOSI: ");
    for (int i = 0; i < AES_KEY_SIZE_256/2; i++) {
        printf("%02X", master->MOSI[i]);
    }
    printf("\n");

    printf("- Master MISO: ");
    for (int i = 0; i < AES_KEY_SIZE_256/2; i++) {
        printf("%02X", master->MISO[i]);
    }
    printf("\n");

    printf("- Master SCK: ");
    for (int i = 0; i < AES_KEY_SIZE_256/2; i++) {
        printf("%02X", master->SCK[i]);
    }
    printf("\n");

    printf("- Master CS1: ");
    for (int i = 0; i < AES_KEY_SIZE_256/2; i++) {
        printf("%02X", master->CS1[i]);
    }
    printf("\n");

    printf("- Master CS2: ");
    for (int i = 0; i < AES_KEY_SIZE_256/2; i++) {
        printf("%02X", master->CS2[i]);
    }
    printf("\n");

    printf("- Master CS3: ");
    for (int i = 0; i < AES_KEY_SIZE_256/2; i++) {
        printf("%02X", master->CS3[i]);
    }
    printf("\n");
}

static int should_transfer(int current_clock, int slave_clock) {
    if (current_clock == slave_clock) {
        return 1;
    }
    return 0;
}

void print_byte_bits(uint8_t byte) {
    for (int bit_index = 7; bit_index >= 0; bit_index--) {
        printf("%d", (byte & (1 << bit_index)) >> bit_index);
    }
    printf(" ");
}

void execute(AES_Config *aes, SPI_Master_Config *master, SPI_Slave_Config *slaveA, SPI_Slave_Config *slaveB, SPI_Slave_Config *slaveC) {
    int flag[3] = { 0, };
    for (size_t byte_index = 0; byte_index < AES_KEY_SIZE_256/2; byte_index++) {
        for (int bit_index = 7; bit_index >= 0; bit_index--) {
            int current_clock = (master->SCK[byte_index] & (1 << bit_index)) >> bit_index;
            int slave_clock = (slaveA->SCK[byte_index] & (1 << bit_index)) >> bit_index;
            if (should_transfer(current_clock, slave_clock)) {                
                uint8_t master_bit = (master->MOSI[byte_index] & (1 << bit_index)) >> bit_index;
                if (master->CS1[byte_index] & (1 << bit_index)) {
                    flag[0] = 1;
                    uint8_t slaveA_bit = (slaveA->MISO[byte_index] & (1 << bit_index)) >> bit_index;
                    master->MISO[byte_index] &= ~(1 << bit_index);
                    master->MISO[byte_index] |= slaveA_bit << bit_index;
                    slaveA->MOSI[byte_index] &= ~(1 << bit_index);
                    slaveA->MOSI[byte_index] |= master_bit << bit_index;
                }
                if (master->CS2[byte_index] & (1 << bit_index)) {
                    flag[1] = 1;
                    uint8_t slaveB_bit = (slaveB->MISO[byte_index] & (1 << bit_index)) >> bit_index;
                    master->MISO[byte_index] &= ~(1 << bit_index);
                    master->MISO[byte_index] |= slaveB_bit << bit_index;
                    slaveB->MOSI[byte_index] &= ~(1 << bit_index);
                    slaveB->MOSI[byte_index] |= master_bit << bit_index;
                }
                if (master->CS3[byte_index] & (1 << bit_index)) {
                    flag[2] = 1;
                    uint8_t slaveC_bit = (slaveC->MISO[byte_index] & (1 << bit_index)) >> bit_index;
                    master->MISO[byte_index] &= ~(1 << bit_index);
                    master->MISO[byte_index] |= slaveC_bit << bit_index;
                    slaveC->MOSI[byte_index] &= ~(1 << bit_index);
                    slaveC->MOSI[byte_index] |= master_bit << bit_index;
                }
            }
        }
    }

    if (flag[0]) {
        struct AES_ctx ctx;
        AES_init_ctx(&ctx, aes->key);
        AES_ECB_decrypt(&ctx, slaveA->MOSI);
        memcpy(slaveA->MISO, slaveA->MOSI, AES_KEY_SIZE_256/2);
    }
    if (flag[1]) {
        struct AES_ctx ctx;
        AES_init_ctx(&ctx, aes->key);
        AES_ECB_decrypt(&ctx, slaveB->MOSI);
        memcpy(slaveB->MISO, slaveB->MOSI, AES_KEY_SIZE_256/2);
    }
    if (flag[2]) {
        struct AES_ctx ctx;
        AES_init_ctx(&ctx, aes->key);
        AES_ECB_decrypt(&ctx, slaveC->MOSI);
        memcpy(slaveC->MISO, slaveC->MOSI, AES_KEY_SIZE_256/2);
    }
}

void getflag(SPI_Slave_Config *slaveA, SPI_Slave_Config *slaveB, SPI_Slave_Config *slaveC)
{
    int flag = 1;
    printf("[~/Main/getflag]\n");

    for (size_t i = 0; i < AES_KEY_SIZE_256/2; ++i) {
        if ((slaveA->MISO[i] ^ slaveB->MISO[i]) != slaveC->MISO[i]) {
            flag = 0;
            printf("[%d]wrong!!!!\n", i);
        }
    }
    if (slaveA->MISO[0] + slaveB->MISO[0] - slaveC->MISO[0] != 0x24) {
        flag = 0;
    }
    if (slaveA->MISO[1] - slaveB->MISO[2] + slaveC->MISO[3] != 0x87) {
        flag = 0;
    }
    if (slaveA->MISO[10] ^ slaveB->MISO[5] ^ slaveC->MISO[3] != 0x87) {
        flag = 0;
    }
    if ((slaveA->MISO[6] ^ slaveB->MISO[6] ^ slaveC->MISO[6]) != 0) {
        flag = 0;
    }
    if (flag)
    {
        FILE *file;
        char buffer[256];
        file = fopen("/flag.txt", "r");
        if (file == NULL) {
            printf("please call admin\n");
        }
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            printf("%s", buffer);
        }
        printf("\n");
        fclose(file);
    }
    else {
        printf("wrong :(\n");
    }
}
