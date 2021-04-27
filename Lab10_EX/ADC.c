// put implementations for functions, explain how it works
// put your names here, date
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

void ADC_Init(void){
 // --UUU-- Complete this(copy from Lab8)
	  int j;
   	SYSCTL_RCGCGPIO_R |= 0x8;
	  while((SYSCTL_PRGPIO_R&0x8) == 0){};
		GPIO_PORTD_DIR_R &= ~0x4;
		GPIO_PORTD_AFSEL_R |= 0x4;
		GPIO_PORTD_DEN_R &= ~0x4;      
    GPIO_PORTD_AMSEL_R |= 0x4;
		SYSCTL_RCGCADC_R |= 0x1;
		for(j = 0; j < 1000000; j++){}
		ADC0_PC_R |= 0x1;
		ADC0_SSPRI_R = 0x0123;
		ADC0_ACTSS_R &= ~0x0008;
		ADC0_EMUX_R &= ~0xF000; 
    ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0)+5;	
		ADC0_SSCTL3_R = 0x0006;
		ADC0_IM_R &= ~0x0008;
		ADC0_ACTSS_R |= 0x0008;
		ADC0_SAC_R = 0xD;
}

//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
uint32_t ADC_In(void){  
	uint32_t result;
	ADC0_PSSI_R = 0x0008;
	while((ADC0_RIS_R & 0x0008) == 0){};
	result = ADC0_SSFIFO3_R & 0xFFF;
	ADC0_ISC_R = 0x0008; 
  return result;   // --UUU-- Complete this(copy from Lab8)
}
