#ifndef EEPROM_LIB_H
    #define EEPROM_LIB_H

/* Definitions */
#define EEADDR 0x50
#define STARTINGADDR 0x0000
#define I2CFRQ 400000
#define PAGESZ 64
#define NBYTES 1
#define MEMSTART 0xABDC
#define I2C_BUS I2C2

/* Function prototypes */
int eeinit_i2c(unsigned int fck);
void eeprom_write_polling(char eaddr);
void single_byte_write(void);
void multi_byte_write(void);
unsigned int write_eeprom( int paddr, int maddr,
                            char *data, unsigned int nbyte);
unsigned int read_eeprom( int paddr, int maddr,
                            char *data, unsigned int nbyte);

#endif