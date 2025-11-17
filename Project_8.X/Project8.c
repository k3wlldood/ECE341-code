#include <plib.h>
#include "CerebotMX7cK.h"
#include "delays.h"
#include "eeprom_lib.h"
#include "LCDlib.h"

void project_init(void) 
{
    Cerebot_mx7cK_setup();
    eeinit_i2c(I2CFRQ);
    
    mPORTBSetPinsDigitalOut(SM_LEDS);
    mPORTGSetPinsDigitalOut(BRD_LEDS);
    
    LATBCLR = SM_LEDS;
    LATGCLR = BRD_LEDS;
}

int main(void)
{
    project_init();
    initialize_LCD();
    
    //single_byte_write();
    multi_byte_write();
}

void multi_byte_write(void) 
{
    char data[NBYTES];
    char data1[NBYTES];
    unsigned int i,j,k,w_remaining_bytes,r_remaining_bytes;
    for (i=0; i<NBYTES; i++)
        data[i] = i % 0xFF;
    
    char paddr = EEADDR;
    char maddr = 0x0000;
    w_remaining_bytes = write_eeprom(paddr, maddr, (char*) &data, NBYTES);
    eeprom_write_polling(paddr);
    r_remaining_bytes = read_eeprom(paddr, maddr, (char*) &data1, NBYTES);
    
    //data1[33] = 2; //to test for failure
    
//    int test_pass = 0;
//    for (i=0; i<NBYTES; i++)
//        if (data[i]!=data1[i]){
//            test_pass = i;
//            break;
//        }
    int test_pass = 1;
    for (i=0; i<NBYTES; i++)
        if (data[i]!=data1[i]){
            test_pass = 0;
        }
    
    if(test_pass) {
        LATBSET = 0xAAA & SM_LEDS;
        LCD_puts("Test Success");
    }
    else {
        LATBSET = (0xFF >> 2) & SM_LEDS;
//        char failstr[31];
//        sprintf(&failstr,"Test Failure %d",i);
//        LCD_puts(failstr);
        LCD_puts("Test Failure");
    }
    
    int n, C;
    
    while(1) 
    {
        LATBINV = SM_LEDS;
        for (n = 0; n < 150; n++)
            for (C = 0; C < 8888; C++);
    }
}

void single_byte_write(void) 
{
    char peripheralAddr = EEADDR;
    char write_error = 0;
    
    char data[10];
    int datasz, index;
    char i2cbyte = 0xAA, i2cbyte_read; // Test value
    
    project_init();
    
    data[0] = peripheralAddr << 1 | 0; //ctrl byte (paddr | !W)
    data[1] = (STARTINGADDR & 0xFF00) >> 8; //upper addr
    data[2] = STARTINGADDR & 0x00FF; //lower addr
    data[3] = i2cbyte; //byte of data to write
    
    datasz = 4;
    index = 0;
    
    IdleI2C2();
    StartI2C2();
    IdleI2C2();
    
    while(datasz--)
        write_error |= MasterWriteI2C2(data[index++]);
    
    IdleI2C2();
    StopI2C2();
    IdleI2C2();
    // End of write sequence
    eeprom_write_polling(EEADDR);
    for(datasz = 0; datasz<50; datasz++)
        for(index = 0; index < 8888; index++);
    
    // Start of Read Sequence
    data[0] = peripheralAddr << 1 | 0; //ctrl byte (paddr | !W)
    data[1] = (STARTINGADDR & 0xFF00) >> 8; //upper addr
    data[2] = STARTINGADDR & 0x00FF; //lower addr
    
    datasz = 3;
    index = 0;
    
    IdleI2C2();
    StartI2C2();
    IdleI2C2();
    
    while(datasz--)
        MasterWriteI2C2(data[index++]);
    
    RestartI2C2();
    //ctrl byte (paddr | r)
    MasterWriteI2C2(peripheralAddr << 1 | 1);
    IdleI2C2();
    i2cbyte_read = MasterReadI2C2();
    NotAckI2C2();
    IdleI2C2();
    StopI2C2();
    IdleI2C2();
    // End of Read Sequence
    
    if(i2cbyte == i2cbyte_read)
        LATBSET = LEDA | LEDC | LEDE | LEDG;
    else
        LATGSET = BRD_LEDS;
    
    int l,k;
    while(1) 
    {
        LATBINV = SM_LEDS;
        for(l = 0; 1<150; l++)
            for(k = 0; k < 8888; k++);
    }
        
    return 1; 
}