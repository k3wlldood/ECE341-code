#include <plib.h>
#include "CerebotMX7cK.h"
#include "delays.h"

void sw_msDelay(unsigned int mS)
{
    int i;
    while(mS --) // SW Stop breakpoint
    {
        for (i=0; i<COUNTS_PER_MS; i++) // 1 ms delay loop
        {
            //do nothing
        }
    } 
} // SW Stop breakpoint

void hw_msDelay(unsigned int mS)
{
    unsigned int tWait, tStart;
    tStart=ReadCoreTimer(); // Read core timer count - SW Start breakpoint
    tWait= (CORE_MS_TICK_RATE * mS); // Set time to wait
    while((ReadCoreTimer() - tStart) < tWait); // Wait for the time to pass
} // SW Stop breakpoint

