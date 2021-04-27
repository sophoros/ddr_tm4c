// Timer0.c
// Runs on LM4F120/TM4C123
// Use TIMER0 in 32-bit periodic mode to request interrupts at a periodic rate
// Daniel Valvano
// Last Modified: 3/6/2015 
// You can use this timer only if you learn how it works

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "ST7735.h"
#include "Images.c"

uint8_t UpArr[180];
uint8_t DownArr[180];
uint8_t LeftArr[180];
uint8_t RightArr[180];
int upCount = 0;
int downCount = 0;
int leftCount = 0;
int rightCount = 0;
int score;

// ***************** Timer0_Init ****************
// Activate TIMER0 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none

void Arr_Init(void){
	for(int i = 0; i < 180; i++){
		UpArr[i] = DownArr[i] = RightArr[i] = LeftArr[i] = 0;
	}
}

void writeArr(char dir){
	if(dir == 0){}
	else if(dir == 'l'){
		LeftArr[179] = 1;
	}
	else if(dir == 'd'){
		DownArr[179] = 1;
	}
	else if(dir == 'u'){
		UpArr[179] = 1;
	}
	else if(dir == 'r'){
		RightArr[179] = 1;
	}
}

void Timer0_Init(uint32_t period){
  SYSCTL_RCGCTIMER_R |= 0x01;   // 0) activate TIMER0
  TIMER0_CTL_R = 0x00000000;    // 1) disable TIMER0A during setup
  TIMER0_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER0_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER0_TAILR_R = period-1;    // 4) reload value
  TIMER0_TAPR_R = 0;            // 5) bus clock resolution
  TIMER0_ICR_R = 0x00000001;    // 6) clear TIMER0A timeout flag
  TIMER0_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 35, interrupt number 19
  NVIC_EN0_R = 1<<19;           // 9) enable IRQ 19 in NVIC
  TIMER0_CTL_R = 0x00000001;    // 10) enable TIMER0A
	score = 0;
	
}

void Timer0A_Handler(void){
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER0A timeout
	
	if(leftCount > 0){
		ST7735_DrawBitmap(5, 29, arrowLeftWhite, 25, 26);
		leftCount--;
	}
	else{
		ST7735_DrawBitmap(5, 30, arrowLeft, 25, 26);
	}
	if(downCount > 0){
		ST7735_DrawBitmap(36, 29, arrowDownWhite, 25, 26);
		downCount--;
	}
	else{
		ST7735_DrawBitmap(36, 30, arrowDown, 25, 26);
	}
	if(upCount > 0){
		ST7735_DrawBitmap(67, 29, arrowUpWhite, 25, 26);
		upCount--;
	}
	else{
		ST7735_DrawBitmap(67, 30, arrowUp, 25, 26);
	}
	if(rightCount > 0){
		ST7735_DrawBitmap(98, 29, arrowRightWhite, 25, 26);
		rightCount--;
	}
	else{
		ST7735_DrawBitmap(98, 30, arrowRight, 25, 26);
	}
	
  for(int y = 0; y < 180; y++){
		if(LeftArr[y]){
			ST7735_DrawBitmap(5, y, arrowLeftRain, 25, 26);
			if(y != 0){
				LeftArr[y-1] = 1;
			}
			LeftArr[y] = 0;
		}
		if(DownArr[y]){
			ST7735_DrawBitmap(36, y, arrowDownRain, 25, 26);
			if(y != 0){
				DownArr[y-1] = 1;
			}
			DownArr[y] = 0;
		}
		if(UpArr[y]){
			ST7735_DrawBitmap(67, y, arrowUpRain, 25, 26);
			if(y != 0){
				UpArr[y-1] = 1;
			}
			UpArr[y] = 0;
		}
		if(RightArr[y]){
			ST7735_DrawBitmap(98, y, arrowRightRain, 25, 26);
			if(y != 0){
				RightArr[y-1] = 1;
			}
			RightArr[y] = 0;
		}
	}
	
	ST7735_DrawFastHLine(0, 147, 128, 0xFFFF);
	ST7735_SetCursor(1,15);
	ST7735_OutString("SCORE:");
	ST7735_OutUDec(score);
}

/*Checks if arrow is in Kill Zone(3 pixel tolerance):
		1-Left
		2-Down
		3-Up
		4-Right*/
void CheckKill(uint8_t dir){
	if(dir == 1){
		for(int y = 0; y < 7; y++){
			if(LeftArr[y+26] == 1){
				LeftArr[y+26] = 0;
				score++;
			}
		}
	}
	else if(dir == 2){
		for(int y = 0; y < 7; y++){
			if(DownArr[y+26] == 1){
				DownArr[y+26] = 0;
				score++;
			}
		}
	}
	else if(dir == 3){
		for(int y = 0; y < 7; y++){
			if(UpArr[y+26] == 1){
				UpArr[y+26] = 0;
				score++;
			}
		}
	}
	else if(dir == 4){
		for(int y = 0; y < 7; y++){
			if(RightArr[y+26] == 1){
				RightArr[y+26] = 0;
				score++;
			}
		}
	}
	ST7735_DrawFastHLine(5, 30, 120, 0x0000);			//Kill Zone Residue Cleaner
	ST7735_DrawFastHLine(5, 31, 120, 0x0000);
	ST7735_DrawFastHLine(5, 32, 120, 0x0000);
	ST7735_DrawFastHLine(5, 4, 120, 0x0000);
	ST7735_DrawFastHLine(5, 3, 120, 0x0000);
	ST7735_DrawFastHLine(5, 2, 120, 0x0000);
}
	
