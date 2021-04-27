#ifndef __TIMER0INTS_H__
#define __TIMER0INTS_H__

// ***************** Timer0_Init ****************
// Activate Timer0 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none
void Timer0_Init(uint32_t period);
//void Arr_Init(void);
void CheckKillL(void);
void CheckKillD(void);
void CheckKillU(void);
void CheckKillR(void);
extern uint16_t upCount;
extern uint16_t downCount;
extern uint16_t leftCount;
extern uint16_t rightCount;
extern int score;

#endif
