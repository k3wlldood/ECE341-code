#ifndef __LCDLIB_H__
    #define __LCDLIB_H__

/* Definitions */
#define LCD_DATA (BIT_0|BIT_1|BIT_2|BIT_3|BIT_4|BIT_5|BIT_6|BIT_7) //Port E
#define LCD_EN BIT_5  //Port D
#define LCD_RS BIT_15 //Port B
#define LCD_RW BIT_4  //Port D

/* Function prototypes */
void initialize_system(void);
char readLCD(int addr);
void writeLCD(int addr, char c);
int busyLCD(void);
void LCD_puts(char *char_string);
void LCD_putc(char c);

#endif

