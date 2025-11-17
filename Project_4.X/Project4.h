/************* Project 4 ***************
 *
 * File name:	Project4.h
 * Author:		Richard W. Wall (C)opyright 2011
 * Rev Date:	Aug. 1, 2012
 * Rev Date:	Aug, 28, 2013
 *
****************************************/

#ifndef __PROJECT_4_H__
    #define __PROJECT_4_H__
#endif

/* Definitions */
#define COUNTS_PER_MS 8897
#define CW 1
#define CCW 0
#define FS 1
#define HS 2
#define T1_PRESCALE 1
#define TOGGLES_PER_SEC 1000
#define T1_TICK (FPB/T1_PRESCALE/TOGGLES_PER_SEC)

/* Function prototypes */
void initialize_system(void);
int read_buttons(void);
void decode_btns(int btns, int *mode, int *dir, int *SM_del);

/* End of Projec4.h */