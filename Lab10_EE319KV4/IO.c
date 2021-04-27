#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

void IO_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x0C;
	while((SYSCTL_RCGCGPIO_R & 0x0C) == 0){}
	GPIO_PORTC_DIR_R &= ~0x0F;
	GPIO_PORTC_DEN_R |= 0x0F;
	GPIO_PORTD_DIR_R &= ~0x04;
	GPIO_PORTD_DEN_R |= 0x04;
}
