#include <plib.h>
#include "CerebotMX7cK.h"
#include "LCDlib.h"
#include "delays.h"

int main()
{
    initialize_system();
    
    char string1[] = "Does Dr J prefer PIC32 or FPGA??";
    char string2[] = "Answer: \116\145\151\164\150\145\162\041";
    
    while(1)
    {
        LCD_puts(string1);
        hw_msDelay(5000);
        
        LCD_puts(string2);
        hw_msDelay(5000);
    }
        
    return 1;
}