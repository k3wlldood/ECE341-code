/************* Project 2 ***************
 * File name:	Project2.c
 *
 * Author:	Richard W. Wall
 * Date:	August 10, 2013    - Tested with MPLab X
 * Notes: 	This project converts the binary encoded buttons to
 *              set one of the four Cerebot MX7cK LEDs.
****************************************/

#include <plib.h>
#include "CerebotMX7cK.h"
#include "Project2.h"

int main()
{
    int mS = 1;
    initialize_system();
    
    while(1)
    {  
        LATBINV = LEDB;
        sw_msDelay(mS);
        hw_msDelay(mS);
    }
    return 1;
}

/* initialize_system Function Description *************************************
 * SYNTAX:          void initialize_system();
 * PARAMETER1:      No Parameters
 * KEYWORDS:        initialize
 * DESCRIPTION:     Sets Registers to default (0) to prevent non zero values 
 *                  from popping up.
 * RETURN VALUE:    None
 * END DESCRIPTION ************************************************************/
void initialize_system(void)
{
    Cerebot_mx7cK_setup(); // Initialize processor board
    PORTSetPinsDigitalOut(IOPORT_B, SM_LEDS);
    LATBCLR = SM_LEDS;
}

/* sw_msDelay (mS) Function Description **********************************
* SYNTAX:       void sw_ms_delay(unsigned int mS);
* DESCRIPTION:  This is a millisecond delay function that will repeat
*               a specified number of times. The constant "COUNTS_PER_MS"
*               must be calibrated for the system frequency.
* KEYWORDS:     delay, ms, milliseconds, software delay
* PARAMETER1:   mS - the total number of milliseconds to delay
* RETURN VALUE: None:
* Notes:        The basic loop counter "COUNTS_PER_MS " is dependent on
*               the CPU frequency. LEDA will toggle at 500 Hz.
*END DESCRIPTION *********************************************************/
void sw_msDelay(unsigned int mS)
{
    int i;
    while(mS --) // SW Stop breakpoint
    {
        for (i=0; i<COUNTS_PER_MS; i++) // 1 ms delay loop
        {
            //do nothing
        }
        LATBINV = LEDA; // Toggle LEDA each ms for instrumentation
    } 
} // SW Stop breakpoint

/* hw_msDelay Function Description ******************************************
* SYNTAX:       void hw_msDelay(unsigned int mS);
* DESCRIPTION:  This is a millisecond delay function uses the core time
*               to set the base millisecond delay period. Delay periods
*               of zero are permitted. LEDA is toggled each millisecond.
* KEYWORDS:     delay, ms, milliseconds, software delay, core timer
* PARAMETER1:   mS - the total number of milliseconds to delay
* RETURN VALUE: None:
* END DESCRIPTION *********************************************************/
void hw_msDelay(unsigned int mS)
{
    unsigned int tWait, tStart;
    tStart=ReadCoreTimer(); // Read core timer count - SW Start breakpoint
    tWait= (CORE_MS_TICK_RATE * mS); // Set time to wait
    while((ReadCoreTimer() - tStart) < tWait); // Wait for the time to pass
    LATBINV = LEDA; // Toggle LED at end of delay period
} // SW Stop breakpoint
	
/* End of Project1.c */
