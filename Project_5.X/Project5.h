#ifndef __PROJECT_5_H__
    #define __PROJECT_5_H__
#endif

/* Definitions */
#define COUNTS_PER_MS 8897
#define CW 1
#define CCW 0
#define FS 1
#define HS 2
#define T1_INTR_RATE 10000

/* Function prototypes */
void initialize_system(void);
void decode_btns(int btns, int *mode, int *dir, int *SM_del);
void stepper_state_machine(unsigned int *Next_State, int mode, int dir);
void Output_to_stepper(unsigned int FSM_State);
