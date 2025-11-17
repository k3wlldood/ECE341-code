#include <plib.h>
#include "CerebotMX7cK.h"
#include "LCDlib.h"
//#include "delays.h"

void msDelay(unsigned int mS)
{
    static int i;
    while(mS --) // SW Stop breakpoint
        for (i=0; i<8888; i++); // 1 ms delay loop
} 

void initialize_LCD(void)
{
    //PMP Initialization
    int cfg1 = PMP_ON|PMP_READ_WRITE_EN|PMP_READ_POL_HI|PMP_WRITE_POL_HI;
    int cfg2 = PMP_DATA_BUS_8 | PMP_MODE_MASTER1 | PMP_WAIT_BEG_4 | PMP_WAIT_MID_15 | PMP_WAIT_END_4;
    int cfg3 = PMP_PEN_0; // only PMA0 enabled
    int cfg4 = PMP_INT_OFF; // no interrupts used
    mPMPOpen(cfg1, cfg2, cfg3, cfg4);
    
    //LCD Initialization
    //msDelay(40);
    writeLCD(0, 0x38);
    //msDelay(40);
    writeLCD(0, 0x0f);
    //msDelay(40);
    writeLCD(0, 0x01);
    //msDelay(40);

    // Refuses to run with delays, for some reason
}

char readLCD(int addr)
{
    PMPSetAddress(addr); // Set LCD RS control
    mPMPMasterReadByte(); // initiate dummy read sequence
    return mPMPMasterReadByte();// read actual data
}

void writeLCD(int addr, char c)
{
    while(busyLCD()); // Wait for LCD to be ready
    PMPSetAddress(addr); // Set LCD RS control
    PMPMasterWrite(c); // initiate write sequence
}

int busyLCD(void)
{
    return readLCD(0) & 0x80;
}

void LCD_puts(char *char_string)
{
    while(*char_string) // Look for end of string NULL character
    {
    LCD_putc(*char_string); // Write character to LCD
    char_string++; // Increment string pointer
    }
}

void LCD_putc(char c)
{ 
    static cursor_pos, row_pos;
    
    while(busyLCD()); 
    cursor_pos = readLCD(0);
    row_pos = cursor_pos & 0x1f;
    switch(c)
    {
        case('\r'):
            writeLCD(0, 0x80 | (cursor_pos & 0x40));
            break;
        case('\n'):
            writeLCD(0, 0x80 | ((~(cursor_pos & 0x40) & 0x40) | (cursor_pos & ~0x40)));
            break;
        default:
            if (row_pos >= 0x10) 
            {
                LCD_putc('\n');
                LCD_putc('\r');
            }
            writeLCD(1,c);
            break;       
    }
    msDelay(7);
}