// SpaceInvaders.c
// Runs on LM4F120/TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the EE319K Lab 10

// Last Modified: 11/20/2018 
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php
/* This example accompanies the books
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2018

   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2018

 Copyright 2018 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
// ******* Possible Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PD2/AIN5
// Slide pot pin 3 connected to +3.3V 
// fire button connected to PE0
// special weapon fire button connected to PE1
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// LED on PB4
// LED on PB5

// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "ST7735.h"
#include "Random.h"
#include "PLL.h"
#include "ADC.h"
#include "Images.h"
#include "Sound.h"
#include "Timer0.h"
#include "Timer1.h"
#include "IO.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(uint32_t count); // time delay in 0.1 seconds

void PortF_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x20;
	while((SYSCTL_RCGCGPIO_R & 0x20) == 0){}
	GPIO_PORTF_DIR_R |= 0x0E;
	GPIO_PORTF_DEN_R |= 0x0E;
}

int main(void){
  PLL_Init(Bus80MHz);       // Bus clock is 80 MHz
  Random_Init(1);
  Output_Init();
	Arr_Init();
	IO_Init();
	ADC_Init();

	DisableInterrupts();
	
	ST7735_FillScreen(0x0000);            // set screen to black
  ST7735_DrawBitmap(0,  80, DDRLOGO, 128,32); // player ship middle bottom
	ST7735_SetCursor(4,10);
	ST7735_SetTextColor(0xFFFF);
	ST7735_OutString("PRESS UP ARROW");
	ST7735_SetCursor(5,11);
	ST7735_OutString("TO CONTINUE");
	while((GPIO_PORTE_DATA_R & 0x2) == 0){};
	ST7735_FillScreen(0x0000);
	Delay100ms(10);
	ST7735_DrawBitmap(45, 50, THREE, 40,38); // player ship middle bottom
	Delay100ms(10);
	ST7735_FillScreen(0x0000);
	Delay100ms(10);
	ST7735_DrawBitmap(45, 50, TWO, 40,44); // player ship middle bottom
  Delay100ms(10);
	ST7735_FillScreen(0x0000);
	Delay100ms(10);
	ST7735_DrawBitmap(45, 50, ONE, 40,44); // player ship middle bottom
	Delay100ms(10);
	ST7735_FillScreen(0x0000);
	
	ST7735_DrawBitmap(5, 30, arrowLeft, 25,26);
	ST7735_DrawBitmap(36, 30, arrowDown, 25,26);
	ST7735_DrawBitmap(67, 30, arrowUp, 25, 26);
	ST7735_DrawBitmap(98, 30, arrowRight, 25,26);
		
	
	
	Timer0_Init(3500000);
	Timer1_Init(87500000);
	Timer2_Init();
	EnableInterrupts();	
	uint32_t ADCdata;
	
	
	while(1){
		ADCdata = ADC_In();
		if(ADCdata > 3800)
		{
			DisableInterrupts();
			ST7735_SetCursor(1,15);
			ST7735_OutString(" PAUSED ");
			
		}
		else
		{
			EnableInterrupts();
		}
	}
}


// You can't use this timer, it is here for starter code only 
// you must use interrupts to perform delays
void Delay100ms(uint32_t count){uint32_t volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}
