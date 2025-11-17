#include <plib.h>
#include "CerebotMX7cK.h"
#include "eeprom_lib.h"


char BusyI2C2(void) { // non-blocking version of IdleI2C2 in PLIB
    return ( I2C2CONbits.SEN || I2C2CONbits.PEN || I2C2CONbits.RSEN || 
            I2C2CONbits.RCEN || I2C2CONbits.ACKEN || I2C2STATbits.TRSTAT );
}

int eeinit_i2c(unsigned int fck) 
{
    OpenI2C2(I2C_EN, fck);
    return 0; //busyI2C2();
}

void eeprom_write_polling(char eaddr) 
{
    char ctrlByte = (eaddr << 1) | 0;
    
    IdleI2C2();
    StartI2C2(); //start the I2C Peripheral Bus
    IdleI2C2();
    
    while(MasterWriteI2C2(ctrlByte)) // MaterWriteI2C2 returns 0 on periph ACK
    {
        IdleI2C2();
        RestartI2C2();
        IdleI2C2();
    }
    
    IdleI2C2();
    StopI2C2();
    IdleI2C2();
}

unsigned int write_eeprom( int paddr, int maddr, 
                            char *data, unsigned int nbyte) 
{
    char werr = 0;
    char addrH = (maddr & 0xFF00) >> 8;
    char addrL = maddr & 0x00FF;
    
    IdleI2C2();
    StartI2C2();
    IdleI2C2();
    werr |= MasterWriteI2C2(((paddr << 1) | 0));
    werr |= MasterWriteI2C2( addrH );
    werr |= MasterWriteI2C2( addrL );
    while(nbyte--){
        werr |= MasterWriteI2C2( *data );
        data++; // increment data pointer
        maddr++; // increment address
        IdleI2C2();
        if ((maddr % PAGESZ) == 0){
            // at page bound, wait and poll for write complete
            IdleI2C2();
            StopI2C2();
            IdleI2C2();

            eeprom_write_polling(paddr);
            IdleI2C2();
            StartI2C2(); // restart the i2c bus so you can continue writing
            IdleI2C2();
            
            // resend the control byte and start of next page
            addrH = (maddr & 0xFF00) >> 8;
            addrL = maddr & 0x00FF;
            
            werr |= MasterWriteI2C2(((paddr << 1) | 0));
            werr |= MasterWriteI2C2( addrH );
            werr |= MasterWriteI2C2( addrL );
            IdleI2C2();
        }
    }
    
    IdleI2C2();
    StopI2C2();
    IdleI2C2();
}

unsigned int read_eeprom( int paddr, int maddr,
                            char *data, unsigned int nbyte) 
{
    char werr = 0;
    char addrH = (maddr & 0xFF00) >> 8;
    char addrL = maddr & 0x00FF;
    IdleI2C2();
    StartI2C2();
    IdleI2C2();
    werr |= MasterWriteI2C2(((paddr << 1) | 0)); // send ctrl byte write mode
    werr |= MasterWriteI2C2( addrH ); // write address
    werr |= MasterWriteI2C2( addrL );
    IdleI2C2();
    RestartI2C2();
    IdleI2C2();
    werr |= MasterWriteI2C2(((paddr << 1) | 1)); // send ctrl byte in read mode.
    IdleI2C2();
    while(nbyte--){
        (*data) = MasterReadI2C2(); // note parentheses around pointer
        IdleI2C2(); // release lines
        if(nbyte){
            AckI2C2(); // ack if there is still bytes to go
            IdleI2C2(); // release lines
            data++; // increment the pointer reference
        }
    }
    
    NotAckI2C2();
    IdleI2C2();
    StopI2C2();
    IdleI2C2();
}
