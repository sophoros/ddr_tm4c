#ifndef __TIMER2INTS_H__ // do not include more than once
#define __TIMER2INTS_H__
#include "ff.h"

// ***************** Timer2_Init ****************
// Activate Timer1 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none
void Timer2_Init(uint32_t period);
uint8_t fifo_Read(uint8_t *datapt);
extern FIL Handle,Handle2;
extern FRESULT MountFresult;
extern FRESULT Fresult;
extern char inFilename[];

#endif
