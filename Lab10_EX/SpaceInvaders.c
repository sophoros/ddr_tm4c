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
#include "PLL.h"
#include "ADC.h"
#include "Images.h"
#include "Sound.h"
#include "Timer0.h"
#include "Timer1.h"
#include "IO.h"
#include "ff.h"
#include "diskio.h"
#include "DAC.h"
#include "Timer2.h"


void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(uint32_t count); // time delay in 0.1 seconds

static FATFS g_sFatFs;

void PortF_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x20;
	while((SYSCTL_RCGCGPIO_R & 0x20) == 0){}
	GPIO_PORTF_DIR_R |= 0x02;
	GPIO_PORTF_DEN_R |= 0x02;
}


int main(void){
  PLL_Init(Bus80MHz);       // Bus clock is 80 MHz
	Output_Init();
  ADC_Init();
	//Arr_Init();
	IO_Init();
	DAC_Init();
	//PortF_Init();
	DisableInterrupts();
	
  ST7735_DrawBitmap(0,  80, DDRLOGO, 128,32); // player ship middle bottom
	ST7735_SetCursor(4,10);
	ST7735_OutString("PRESS UP ARROW");
	ST7735_SetCursor(7,11);
	ST7735_OutString("TO START");
	while((GPIO_PORTE_DATA_R & 0x02) == 0){};
	while(1){
		
		EnableInterrupts();
		
		MountFresult = f_mount(&g_sFatFs, "", 0);
		if(MountFresult){
			ST7735_DrawString(0, 0, "f_mount error", ST7735_Color565(0, 0, 255));
			while(1){};
		}
	
		Fresult = f_open(&Handle, inFilename, FA_READ);
		if(Fresult == FR_OK){
			ST7735_DrawString(0, 0, "Opened ", ST7735_Color565(0, 255, 0));
			ST7735_DrawString(7, 0, (char *)inFilename, ST7735_Color565(0, 255, 0));
	
			ST7735_FillScreen(0x0000);
			Delay100ms(10);
			ST7735_DrawBitmap(46, 50, THREE, 40,38); // player ship middle bottom
			Delay100ms(10);
			ST7735_FillScreen(0x0000);
			Delay100ms(10);
			ST7735_DrawBitmap(46, 50, TWO, 40,44); // player ship middle bottom
			Delay100ms(10);
			ST7735_FillScreen(0x0000);
			Delay100ms(10);
			ST7735_DrawBitmap(46, 50, ONE, 40,44); // player ship middle bottom
			Delay100ms(10);
			ST7735_FillScreen(0x0000);
			
			Timer2_Init(2000);
			Timer0_Init(2000000);
			//Timer1_Init(7256);			//11.025 kHz
			Timer1_Init(3628);		//22.050 kHz
			while(1){
				if(ADC_In() > 2500){
					DisableInterrupts();
					ST7735_SetCursor(1,15);
					ST7735_OutString("PAUSED  ");	
				}
				else{
					EnableInterrupts();
				}
			}
		}
		else{
			ST7735_DrawString(0, 0, "Error          (  )", ST7735_Color565(255, 0, 0));
			ST7735_DrawString(6, 0, (char *)inFilename, ST7735_Color565(255, 0, 0));
			ST7735_SetCursor(16, 0);
			ST7735_SetTextColor(ST7735_Color565(255, 0, 0));
			ST7735_OutUDec((uint32_t)Fresult);
		}
	}	

	
#if 0
EnableInterrupts();
	//Mount SDC
	while(1){
		while(/*(GPIO_PORTF_DATA_R & 0x03) == 0*/ 1){
			if(/*(GPIO_PORTF_DATA_R & 0x03) == 0x01*/ 1){
				/*inFilename[0] = 'p';
				inFilename[1] = 's';
				inFilename[2] = 'y';
				inFilename[3] = 'c';
				inFilename[4] = 'h';
				inFilename[5] = 'o';
				inFilename[6] = '.';
				inFilename[7] = 't';
				inFilename[8] = 'x';
				inFilename[9] = 't';*/
				break;
			}
			else if((GPIO_PORTF_DATA_R & 0x03) == 0x02){
				//track = 2;
				break;
			}
			else if((GPIO_PORTF_DATA_R & 0x03) == 0x03){
				//track = 3;
				break;
			}
	}
		
	MountFresult = f_mount(&g_sFatFs, "", 0);
  if(MountFresult){
    ST7735_DrawString(0, 0, "f_mount error", ST7735_Color565(0, 0, 255));
    while(1){};
	}
	
	Fresult = f_open(&Handle, inFilename, FA_READ);
	if(Fresult == FR_OK){
    ST7735_DrawString(0, 0, "Opened ", ST7735_Color565(0, 255, 0));
    ST7735_DrawString(7, 0, (char *)inFilename, ST7735_Color565(0, 255, 0));
		
		Timer2_Init(6000);
		while(1){}
	}
	else{
    ST7735_DrawString(0, 0, "Error          (  )", ST7735_Color565(255, 0, 0));
    ST7735_DrawString(6, 0, (char *)inFilename, ST7735_Color565(255, 0, 0));
    ST7735_SetCursor(16, 0);
    ST7735_SetTextColor(ST7735_Color565(255, 0, 0));
    ST7735_OutUDec((uint32_t)Fresult);
	}
}
#endif
}

void Delay100ms(uint32_t count){uint32_t volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}
