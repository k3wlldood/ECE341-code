#include <plib.h>
#include "CerebotMX7cK.h"
#include "Project5.h"
#include "delays.h"

static int mode = HS, dir = CW, SM_del = 20;
enum SMStates {S0_5=0xA, S1=0x8, S1_5=0x9, S2=0x1,
               S2_5=0x5, S3=0x4, S3_5=0x6, S4=0x2};
enum SMStates FSM_State = S0_5;

int main()
{
    initialize_system();
    
    while(1);
        
    return 1;
}

void initialize_system(void)
{
    Cerebot_mx7cK_setup(); // Initialize processor board
    PORTSetPinsDigitalIn(IOPORT_G, BTN1|BTN2);
    PORTSetPinsDigitalOut(IOPORT_B, SM_LEDS);
    LATBCLR = SM_LEDS;
    
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_1, T1_INTR_RATE-1);
    mT1SetIntPriority(2); // Group priority range: 1 to 7
    mT1SetIntSubPriority(0); // Subgroup priority range: 0 to 3
    mT1IntEnable(1); // Enable T1 interrupts
    
    unsigned int dummy; // NEEDED!!!!!!!
    mCNOpen(CN_ON,(CN8_ENABLE | CN9_ENABLE), 0);
    mCNSetIntPriority(1);
    mCNSetIntSubPriority(0);
    // the following 3 lines are CRITICAL!!!
    dummy = PORTReadBits(IOPORT_G, BTN1 | BTN2);
    mCNClearIntFlag(); // do after reading into dummy.
    mCNIntEnable(1);
    
    INTEnableSystemMultiVectoredInt();
    INTEnableInterrupts();
}

void __ISR(_CHANGE_NOTICE_VECTOR, IPL1) CNIntHandler(void)
{
    LATBINV = LEDC;
    hw_msDelay(20);
    int btns = mPORTGReadBits(BTN1 | BTN2);
    decode_btns(btns, &mode, &dir, &SM_del);
    mCNClearIntFlag();
    LATBINV = LEDC;
}

void __ISR(_TIMER_1_VECTOR, IPL2) Timer1Handler(void)
{
    int local_SM_del = SM_del;
    while(local_SM_del >= 1)
    {
        while(!mT1GetIntFlag()); // Wait for interrupt flag
        mT1ClearIntFlag(); // Clear the interrupt flag
        LATBINV = LEDA;
        (local_SM_del)--;
    }
    stepper_state_machine(&FSM_State, mode, dir);
    mT1ClearIntFlag();
}

void decode_btns(int btns, int *mode, int *dir, int *SM_del)
{
    switch(btns)
    {
        case (BTN1):
                *mode = FS;
                *dir = CW;
                *SM_del = 40;
            break;
        case (BTN2):
                *mode = HS;
                *dir = CCW;
                *SM_del = 30;
            break;
        case (BTN1|BTN2):
                *mode = FS;
                *dir = CCW;
                *SM_del = 24;
            break;
        default:
                *mode = HS;
                *dir = CW;
                *SM_del = 20;
            break;
    }
}

void stepper_state_machine(unsigned int *Next_State, int mode, int dir)
{
    LATBINV = LEDB;
    switch(*Next_State) 
    {
        case S0_5:
            if(mode==FS && dir==CW) 
            {
                *Next_State = S1_5;
            }
            else if (mode==HS && dir==CW) 
            {
                *Next_State = S1;
            }
            else if (mode==FS && dir==CCW) 
            {
                *Next_State = S3_5;
            }
            else *Next_State = S4;
            break;
        case S1:
            if(mode==FS && dir==CW) 
            {
                *Next_State = S2;
            }
            else if (mode==HS && dir==CW) 
            {
                *Next_State = S1_5;
            }
            else if (mode==FS && dir==CCW) 
            {
                *Next_State = S4;
            }
            else *Next_State = S0_5;
            break;
        case S1_5:
            if(mode==FS && dir==CW) 
            {
                *Next_State = S2_5;
            }
            else if (mode==HS && dir==CW) 
            {
                *Next_State = S2;
            }
            else if (mode==FS && dir==CCW) 
            {
                *Next_State = S0_5;
            }
            else *Next_State = S1;
            break;
        case S2:
            if(mode==FS && dir==CW) 
            {
                *Next_State = S3;
            }
            else if (mode==HS && dir==CW) 
            {
                *Next_State = S2_5;
            }
            else if (mode==FS && dir==CCW) 
            {
                *Next_State = S1;
            }
            else *Next_State = S1_5;
            break;
        case S2_5:
            if(mode==FS && dir==CW) 
            {
                *Next_State = S3_5;
            }
            else if (mode==HS && dir==CW) 
            {
                *Next_State = S3;
            }
            else if (mode==FS && dir==CCW) 
            {
                *Next_State = S1_5;
            }
            else *Next_State = S2;
            break;
        case S3:
            if(mode==FS && dir==CW) 
            {
                *Next_State = S4;
            }
            else if (mode==HS && dir==CW) 
            {
                *Next_State = S3_5;
            }
            else if (mode==FS && dir==CCW) 
            {
                *Next_State = S2;
            }
            else *Next_State = S2_5;
            break;
        case S3_5:
            if(mode==FS && dir==CW) 
            {
                *Next_State = S0_5;
            }
            else if (mode==HS && dir==CW) 
            {
                *Next_State = S4;
            }
            else if (mode==FS && dir==CCW) 
            {
                *Next_State = S2_5;
            }
            else *Next_State = S3;
            break;
        case S4:
            if(mode==FS && dir==CW) 
            {
                *Next_State = S1;
            }
            else if (mode==HS && dir==CW) 
            {
                *Next_State = S0_5;
            }
            else if (mode==FS && dir==CCW) 
            {
                *Next_State = S3;
            }
            else *Next_State = S3_5;
            break;
        default:
            *Next_State = S0_5;
    }
    Output_to_stepper(*Next_State);
}

void Output_to_stepper(unsigned int FSM_State) 
{
    LATB = (FSM_State<<7 & SM_COILS) | (LATB & ~SM_COILS);
}

