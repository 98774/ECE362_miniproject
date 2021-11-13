#include "dac.h"
#include "STM32f0xx.h"
#include <math.h>

#include "stm32f0xx.h"
#include <math.h>
#include <stdint.h>
#define SAMPLES 30



/*******************************************************************************
Author: Jonathon Snyder
Date: 11/13/2021
Description: Sets the frequency output for the DAC
*******************************************************************************/
void setfreq(float fre)
{

	TIM7->CR1 &= ~TIM_CR1_CEN; //enable timer

	TIM7->ARR = (48000000 / (fre * SAMPLES)) - 1; //trigger fre * samples / sec

	TIM7->CR1 |= TIM_CR1_CEN; //enable timer
}

/*******************************************************************************
Author: Jonathon Snyder
Date: 11/13/2021
Description: initializes the dac subsystem and enables triggering from tim7
*******************************************************************************/

void init_dac(uint16_t array[]) {
	RCC->APB1ENR |= RCC_APB1ENR_DACEN; //enable clock to dac
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN; //enable GPIO A
	GPIOA->MODER |= GPIO_MODER_MODER4; //Set PA4 to analog

	DAC->CR |= DAC_CR_TSEL1_1; //Set up triggering on timer 7 0x010
	DAC->CR |= DAC_CR_TEN1; //enable trigger
	DAC->CR |= DAC_CR_EN1; //enable channel 1

	for(int x=0; x < SAMPLES; x += 1)
		array[x] = 2048 + 1952 * sin(2 * M_PI * x / SAMPLES);


}
