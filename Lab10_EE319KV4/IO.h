#ifndef IO_H
#define IO_H
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "ST7735.h"
#include "Images.c"
#include "Timer0.h"
#include "Sound.h"
#include "DAC.h"


const uint8_t sinee[64]={32,35,38,41,44,47,49,52,54,56,58,59,61,62,62,63,63,63,62,62,61,
												59,58,56,54,52,49,47,44,41,38,35,32,29,26,23,20,17,15,
												12,10,8,6,5,3,2,2,1,1,1,2,2,3,5,6,8,10,12,15,17,20,23,26,29}; 
	

void DisableInterrupts(void);
void EnableInterrupts(void);

//Initialize PE1-4 & PD2
void IO_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x18;
	while((SYSCTL_RCGCGPIO_R & 0x18) == 0){}
	GPIO_PORTD_DIR_R &= ~0x04;
	GPIO_PORTD_DEN_R |= 0x04;
	GPIO_PORTE_DIR_R &= ~0x1E;
	GPIO_PORTE_DEN_R |= 0x1E;
	GPIO_PORTE_IS_R &= ~0x1E;
	GPIO_PORTE_IBE_R &= ~0x1E;
	GPIO_PORTE_IEV_R |= 0x1E;
	GPIO_PORTE_ICR_R = 0x1E;
	GPIO_PORTE_IM_R |= 0x1E;
	NVIC_PRI1_R = (NVIC_PRI1_R&0xFFFFFF00)|0x80;				//Button interrupt priority 4
	NVIC_EN0_R = 0x00000010;
	EnableInterrupts();
}

void GPIOPortE_Handler(void){
	GPIO_PORTE_ICR_R = 0x1E;
	
	int pin = GPIO_PORTE_DATA_R & 0x1E;
if(pin == 0x02){																		//If Left
		ST7735_DrawBitmap(5, 29, arrowLeftWhite, 25, 25);
		CheckKill(1);
		leftCount = 5;
		ST7735_DrawBitmap(5, 30, arrowLeft, 25, 26);
		Sound_Play(2841);
		
		uint32_t times = 3; 
		
 while (1){
			DAC_Out(sinee[indexx]);
	}
}
	else if(pin == 0x04){															//If Down
		ST7735_DrawBitmap(36, 29, arrowDownWhite, 25, 25);
		CheckKill(2);
		downCount = 5;
		ST7735_DrawBitmap(36, 30, arrowDown, 25, 26);
	}
	else if(pin == 0x08){															//If Up
		ST7735_DrawBitmap(67, 29, arrowUpWhite, 25,25);
		CheckKill(3);
		upCount = 5;
		ST7735_DrawBitmap(67, 30, arrowUp, 25, 26);
	}
	else if(pin == 0x10){															//If Right
		ST7735_DrawBitmap(98, 29, arrowRightWhite, 25,25);
		CheckKill(4);
		rightCount = 5;
		ST7735_DrawBitmap(98, 30, arrowRight, 25, 26);
	}
}

#endif
