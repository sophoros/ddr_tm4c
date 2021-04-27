// Sound.c
// Runs on any computer
// Sound assets based off the original Space Invaders 
// Import these constants into your SpaceInvaders.c for sounds!
// Jonathan Valvano
// November 17, 2014
#include <stdint.h>
#include "Sound.h"
#include "../inc/tm4c123gh6pm.h"
#include "DAC.h"


void Sound_Init(void){
	
};
void Sound_Play(uint32_t count){
// write this
		TIMER2_TAILR_R = count-1;
};
void Sound_Shoot(void){
// write this
};
