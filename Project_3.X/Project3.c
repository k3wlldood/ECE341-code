/************* Project 3 ***************
 * File name:	Project3.c
 *
 * Author:	Richard W. Wall
 * Date:	August 10, 2013    - Tested with MPLab X
 * Notes: 	This project converts the binary encoded buttons to
 *              set one of the four Cerebot MX7cK LEDs.
****************************************/

#include <plib.h>
#include "CerebotMX7cK.h"
#include "Project3.h"

enum SMStates {S0_5=0xA, S1=0x8, S1_5=0x9, S2=0x1,
               S2_5=0x5, S3=0x4, S3_5=0x6, S4=0x2};

int main()
{
    int mS = 20;
    initialize_system();
    PORTSetPinsDigitalIn(IOPORT_G, BTN1 | BTN2);
    enum SMStates FSM_State = S0_5;
    enum SMStates Next_State;
    static int mode, dir;
    
    while(1)
    {  
        int btns = read_buttons();
        decode_btns(btns, &mode, &dir);
        FSM_State = FSM_NSL(FSM_State, mode, dir);
        Output_to_stepper(FSM_State);
        LATBINV = LEDB;
        sw_msDelay(mS);
    }
    return 1;
}

void initialize_system(void)
{
    Cerebot_mx7cK_setup(); // Initialize processor board
    PORTSetPinsDigitalOut(IOPORT_B, SM_LEDS);
    LATBCLR = SM_LEDS;
}

int read_buttons(void)
{
    return mPORTGReadBits(BTN1 | BTN2);
}

int decode_btns(int btns, int *mode, int *dir)
{
    switch(btns)
    {
        case (BTN1):
                *mode = HS;
                *dir = CW;
            break;
        case (BTN2):
                *mode = HS;
                *dir = CCW;
            break;
        case (BTN1|BTN2):
                *mode = FS;
                *dir = CCW;
            break;
        default:
                *mode = FS;
                *dir = CW;
            break;
    }
    return *mode, *dir;
}

int FSM_NSL(int FSM_State, int mode, int dir)
{
    enum SMStates Next_State;
    switch(FSM_State) 
    {
        case S0_5:
            if(mode==FS && dir==CW) 
            {
                Next_State = S1_5;
            }
            else if (mode==HS && dir==CW) 
            {
                Next_State = S1;
            }
            else if (mode==FS && dir==CCW) 
            {
                Next_State = S3_5;
            }
            else Next_State = S4;
            break;
        case S1:
            if(mode==FS && dir==CW) 
            {
                Next_State = S2;
            }
            else if (mode==HS && dir==CW) 
            {
                Next_State = S1_5;
            }
            else if (mode==FS && dir==CCW) 
            {
                Next_State = S4;
            }
            else Next_State = S0_5;
            break;
        case S1_5:
            if(mode==FS && dir==CW) 
            {
                Next_State = S2_5;
            }
            else if (mode==HS && dir==CW) 
            {
                Next_State = S2;
            }
            else if (mode==FS && dir==CCW) 
            {
                Next_State = S0_5;
            }
            else Next_State = S1;
            break;
        case S2:
            if(mode==FS && dir==CW) 
            {
                Next_State = S3;
            }
            else if (mode==HS && dir==CW) 
            {
                Next_State = S2_5;
            }
            else if (mode==FS && dir==CCW) 
            {
                Next_State = S1;
            }
            else Next_State = S1_5;
            break;
        case S2_5:
            if(mode==FS && dir==CW) 
            {
                Next_State = S3_5;
            }
            else if (mode==HS && dir==CW) 
            {
                Next_State = S3;
            }
            else if (mode==FS && dir==CCW) 
            {
                Next_State = S1_5;
            }
            else Next_State = S2;
            break;
        case S3:
            if(mode==FS && dir==CW) 
            {
                Next_State = S4;
            }
            else if (mode==HS && dir==CW) 
            {
                Next_State = S3_5;
            }
            else if (mode==FS && dir==CCW) 
            {
                Next_State = S2;
            }
            else Next_State = S2_5;
            break;
        case S3_5:
            if(mode==FS && dir==CW) 
            {
                Next_State = S0_5;
            }
            else if (mode==HS && dir==CW) 
            {
                Next_State = S4;
            }
            else if (mode==FS && dir==CCW) 
            {
                Next_State = S2_5;
            }
            else Next_State = S3;
            break;
        case S4:
            if(mode==FS && dir==CW) 
            {
                Next_State = S1;
            }
            else if (mode==HS && dir==CW) 
            {
                Next_State = S0_5;
            }
            else if (mode==FS && dir==CCW) 
            {
                Next_State = S3;
            }
            else Next_State = S3_5;
            break;
        default:
            Next_State = S0_5;
    }
    return Next_State;
}

void Output_to_stepper(enum SMStates FSM_State) 
{
    LATB = (FSM_State<<7 & SM_COILS) | (LATB & ~SM_COILS);
}

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
	
/* End of Project1.c */
