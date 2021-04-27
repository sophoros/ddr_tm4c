#ifndef IO_H
#define IO_H
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "ST7735.h"
#include "Images.c"
#include "Timer0.h"

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
