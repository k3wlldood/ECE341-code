/************* Project 3 ***************
 *
 * File name:	Project3.h
 * Author:		Richard W. Wall (C)opyright 2011
 * Rev Date:	Aug. 1, 2012
 * Rev Date:	Aug, 28, 2013
 *
****************************************/

#ifndef __PROJECT_3_H__
    #define __PROJECT_3_H__
#endif

/* Definitions */
#define COUNTS_PER_MS 8897
#define CW 1
#define CCW 0
#define FS 1
#define HS 2

/* Function prototypes */
void initialize_system(void);
int read_buttons(void);
int decode_btns(int btns, int *mode, int *dir);
void sw_msDelay(unsigned int mS);


/* End of Projec2.h */