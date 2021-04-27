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
#include "random.h"

uint8_t UpArr[180];
uint8_t DownArr[180];
uint8_t LeftArr[180];
uint8_t RightArr[180];
uint16_t upCount = 0;
uint16_t downCount = 0;
uint16_t leftCount = 0;
uint16_t rightCount = 0;
int score;
uint8_t deploy;
int prog;
int track;
char hopes[206] = {0,0,0,0,0,0,0,0,0,0,'l',0,0,'r',0,0,'u',0,0,'d',0,0,'l',0,0,'r',0,0,'u',0,0,'l',0,0,'r',0,0,'l','l','d',0,0,'u','r','l',0,'d',0,'l','d','u',0,0,0,'u','r',0,0,'d','l',0,0,'r',0,0,'u',0,0,'d',0,0,'l',0,0,'r',0,0,'u',0,0,'l',0,0,'r',0,0,'l',0,0,'d',0,0,'u','d','r',0,'d',0,'l','d','u',0,0,0,'u','r',0,
0,'d','l',0,0,'r',0,0,'u',0,0,'d',0,0,'u','r',0,'r',0,0,'u',0,0,'l',0,0,'r',0,0,'u','r','r','r',0,'u','r','l',0,'d',0,'l','d','u',0,'u','u','u','r',0,0,'d','l',0,0,'r',0,0,'u',0,0,'d',0,0,'l',0,0,'r',0,'u',0,0,'l',0,0,'r','l',0,'d',0,'l','d','u',0,0,0,'u','r',0,0,'d','l',0,0,'r',0,0,'u',};

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

void Timer0_Init(/*int trackNum,*/ uint32_t period){
  SYSCTL_RCGCTIMER_R |= 0x01;   // 0) activate TIMER0
  TIMER0_CTL_R = 0x00000000;    // 1) disable TIMER0A during setup
  TIMER0_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER0_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER0_TAILR_R = period-1;    // 4) reload value
  TIMER0_TAPR_R = 0;            // 5) bus clock resolution
  TIMER0_ICR_R = 0x00000001;    // 6) clear TIMER0A timeout flag
  TIMER0_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0xE0000000; // 8) priority 7
// interrupts enabled in the main program after all devices initialized
// vector number 35, interrupt number 19
  NVIC_EN0_R = 1<<19;           // 9) enable IRQ 19 in NVIC
  TIMER0_CTL_R = 0x00000001;    // 10) enable TIMER0A
	score = 0;
	prog = 0;
	deploy = 25;
	//track = trackNum;
}

void Timer0A_Handler(void){
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER0A timeout
	
	if(deploy == 0){
		deploy = 25;
		//if(track == 1){
			writeArr(hopes[prog]);
		/*}
		else{
			int rando = Random32()%5;
			if(rando == 1){writeArr('l');}
			else if(rando == 2){writeArr('u');}
			else if(rando == 3){writeArr('d');}
			else if(rando == 4){writeArr('r');}
		}*/
		prog++;
	}
	else{
		deploy--;
	}
	
	
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

void CheckKillL(void){
	for(int y = 0; y < 11; y++){
		if(LeftArr[y+24] == 1){
			LeftArr[y+24] = 0;
			score++;
		}
	}
	ST7735_DrawFastHLine(5, 30, 25, 0x0000);			//Kill Zone Residue Cleaner
	ST7735_DrawFastHLine(5, 31, 25, 0x0000);
	ST7735_DrawFastHLine(5, 32, 25, 0x0000);
	ST7735_DrawFastHLine(5, 4, 25, 0x0000);
	ST7735_DrawFastHLine(5, 3, 25, 0x0000);
	ST7735_DrawFastHLine(5, 2, 25, 0x0000);
}

void CheckKillD(void){
	for(int y = 0; y < 11; y++){
		if(DownArr[y+24] == 1){
			DownArr[y+24] = 0;
			score++;
		}
	}
	ST7735_DrawFastHLine(36, 30, 25, 0x0000);			//Kill Zone Residue Cleaner
	ST7735_DrawFastHLine(36, 31, 25, 0x0000);
	ST7735_DrawFastHLine(36, 32, 25, 0x0000);
	ST7735_DrawFastHLine(36, 4, 25, 0x0000);
	ST7735_DrawFastHLine(36, 3, 25, 0x0000);
	ST7735_DrawFastHLine(36, 2, 25, 0x0000);
}

void CheckKillU(void){
	for(int y = 0; y < 11; y++){
		if(UpArr[y+24] == 1){
			UpArr[y+24] = 0;
			score++;
		}
	}
	ST7735_DrawFastHLine(67, 30, 25, 0x0000);			//Kill Zone Residue Cleaner
	ST7735_DrawFastHLine(67, 31, 25, 0x0000);
	ST7735_DrawFastHLine(67, 32, 25, 0x0000);
	ST7735_DrawFastHLine(67, 4, 25, 0x0000);
	ST7735_DrawFastHLine(67, 3, 25, 0x0000);
	ST7735_DrawFastHLine(67, 2, 25, 0x0000);
}

void CheckKillR(void){
	for(int y = 0; y < 11; y++){
		if(RightArr[y+24] == 1){
			RightArr[y+24] = 0;
			score++;
		}
	}
	ST7735_DrawFastHLine(98, 30, 25, 0x0000);			//Kill Zone Residue Cleaner
	ST7735_DrawFastHLine(98, 31, 25, 0x0000);
	ST7735_DrawFastHLine(98, 32, 25, 0x0000);
	ST7735_DrawFastHLine(98, 4, 25, 0x0000);
	ST7735_DrawFastHLine(98, 3, 25, 0x0000);
	ST7735_DrawFastHLine(98, 2, 25, 0x0000);
}
