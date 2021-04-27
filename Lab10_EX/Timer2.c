#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "diskio.h"
#include "ff.h"

FIL Handle,Handle2;
FRESULT MountFresult;
FRESULT Fresult;
char inFilename[] = "hopesL2.txt";
UINT successfulreads;
//char c[4];	//data read buffer
unsigned int carry;

uint8_t fifo[256];
char buffer0[512];
char buffer1[512];
int bufferFull0;
int bufferFull1;
int currentBuffer;
int bufferProg;
int success;
uint8_t data;
int readI;
int writeI;

int startTime;
int stopTime;
int elapsedTime;

uint8_t buff_3Digit(void);

// ***************** TIMER2_Init ****************
// Activate TIMER2 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none
void Timer2_Init(uint32_t period){
	
	readI = 0;
	writeI = 0;
	success = 1;
	
	Fresult = f_read(&Handle, &buffer0, 512, &successfulreads);		//Preload Buffer Duo
	bufferFull0 = 1;
	Fresult = f_read(&Handle, &buffer1, 512, &successfulreads);
	bufferFull1 = 1;
	
	currentBuffer = 0;
	bufferProg = 0;
	
	for(int i = 0; i < 255; i++){																	//Preload Fifo
		if(((writeI+1)%256 != readI) && buff_3Digit()){
			fifo[writeI] = data;
			writeI = (writeI+1)%256;
		}
	}
	
  SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate TIMER2
  TIMER2_CTL_R = 0x00000000;    // 1) disable TIMER2A during setup
  TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER2_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER2_TAILR_R = period-1;    // 4) reload value
  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R = 0x00000001;    // 6) clear TIMER2A timeout flag
  TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 37, interrupt number 21
  NVIC_EN0_R = 1<<23;           // 9) enable IRQ 21 in NVIC
  TIMER2_CTL_R = 0x00000001;    // 10) enable TIMER2A
}

void Timer2A_Handler(void){
  TIMER2_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER1A timeout
	
	if(bufferFull0 == bufferFull1 == 1){}
	else if(bufferFull0 == 0){
		//startTime = TIMER2_TAV_R;
		Fresult = f_read(&Handle, &buffer0, 512, &successfulreads);
		//stopTime = TIMER2_TAV_R;
		//elapsedTime = startTime - stopTime;
		if((Fresult != FR_OK) || (successfulreads < 512)){
			Fresult = f_close(&Handle);
		}
		bufferFull0 = 1;
	}
	else if(bufferFull1 == 0){
		Fresult = f_read(&Handle, &buffer1, 512, &successfulreads);
		if((Fresult != FR_OK) || (successfulreads < 512)){
			Fresult = f_close(&Handle);
		}
		bufferFull1 = 1;
	}
	
	if(((writeI+1)%256 != readI) && buff_3Digit()){
		fifo[writeI] = data;
		writeI = (writeI+1)%256;
	}
}

uint8_t fifo_Read(uint8_t *datapt){ 
	if(readI != writeI){
		*datapt = fifo[readI];
		readI = (readI+1)%256;
		return 1;
	}
  return 0;
}

uint8_t buff_3Digit(void){
	if(success == 1){
		data = 0;
	}
	if(currentBuffer == 0){
		while(buffer0[bufferProg] != ','){
			data = data*10 + (buffer0[bufferProg] - '0');
			bufferProg++;
			if(bufferProg == 512){				//Case: Buffer ends in a number
				bufferFull0 = 0;
				currentBuffer = 1;
				bufferProg = 0;
				success = 0;
				return 0;
			}
		}
		bufferProg++;
		if(bufferProg == 512){					//Case: Buffer ends in a comma
			bufferFull0 = 0;
			currentBuffer = 1;
			bufferProg = 0;
		}
		success = 1;
		return 1;
	}
	else{
		while(buffer1[bufferProg] != ','){
			data = data*10 + (buffer1[bufferProg] - '0');
			bufferProg++;
			if(bufferProg == 512){
				bufferFull1 = 0;
				currentBuffer = 0;
				bufferProg = 0;
				success = 0;
				return 0;
			}
		}
		bufferProg++;
		if(bufferProg == 512){
			bufferFull1 = 0;
			currentBuffer = 0;
			bufferProg = 0;
		}
		success = 1;
		return 1;
	}
}
