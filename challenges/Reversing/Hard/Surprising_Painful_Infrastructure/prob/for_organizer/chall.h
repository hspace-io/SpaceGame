#ifndef CHALL_H
#define CHALL_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>

#define AES_KEY_SIZE_256 32
#define ECB 1
#define AES256 1
#define AES_BLOCKLEN 16
#define AES_KEYLEN 32
#define AES_keyExpSize 240

typedef struct {
    uint8_t key[AES_KEY_SIZE_256];
} AES_Config;

typedef struct {
    uint8_t MOSI[AES_KEY_SIZE_256/2];   // LINE1
    uint8_t MISO[AES_KEY_SIZE_256/2];	// LINE2
    uint8_t SCK[AES_KEY_SIZE_256/2];	// LINE3
    uint8_t CS1[AES_KEY_SIZE_256/2];	// LINE4
    uint8_t CS2[AES_KEY_SIZE_256/2];	// LINE5
    uint8_t CS3[AES_KEY_SIZE_256/2];	// LINE6

} SPI_Master_Config;

typedef struct {
    uint8_t MOSI[AES_KEY_SIZE_256/2];   // LINE1
    uint8_t MISO[AES_KEY_SIZE_256/2];	// LINE2
    uint8_t SCK[AES_KEY_SIZE_256/2];	// LINE3
    uint8_t CS[AES_KEY_SIZE_256/2];		// LINE4~6 (A : 4, B : 5, C : 6)
} SPI_Slave_Config;

typedef enum {
    MODE_1 = 1, // CPOL = 0, CPHA = 0
    MODE_2,     // CPOL = 0, CPHA = 1
    MODE_3,     // CPOL = 1, CPHA = 0
    MODE_4      // CPOL = 1, CPHA = 1
} SPI_Mode;

struct AES_ctx
{
  uint8_t RoundKey[AES_keyExpSize];
};

//util
int random_zero_or_one();
int setmode();
void status(int mode, SPI_Master_Config *master);
void init(int mode, AES_Config *aes, SPI_Master_Config *master, SPI_Slave_Config *slaveA, SPI_Slave_Config *slaveB, SPI_Slave_Config *slaveC);
void print_main_menu();
void string_to_bits(const char *str, uint8_t *buffer, size_t size);
void config(SPI_Master_Config *master);
void execute(AES_Config *aes, SPI_Master_Config *master, SPI_Slave_Config *slaveA, SPI_Slave_Config *slaveB, SPI_Slave_Config *slaveC);
void print_byte_bits(uint8_t byte);
void getflag(SPI_Slave_Config *slaveA, SPI_Slave_Config *slaveB, SPI_Slave_Config *slaveC);

//aes
void generate_random_bytes(uint8_t *buffer, size_t size);
void copy_key_part(uint8_t *dest, const uint8_t *src, size_t start, size_t end);
void AES_init_ctx(struct AES_ctx* ctx, const uint8_t* key);
void AES_ECB_encrypt(const struct AES_ctx* ctx, uint8_t* buf);
void AES_ECB_decrypt(const struct AES_ctx* ctx, uint8_t* buf);


//log(have to delete)
void print_aes_config(const AES_Config* aes);
void print_spi_master_config(const SPI_Master_Config* master);
void print_spi_slave_config(const SPI_Slave_Config* slave, char id);
#endif