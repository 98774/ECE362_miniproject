#include "dac.h"
#include "STM32f0xx.h"
#include <math.h>

//===========================================================================
// 2.5 Output Sine Wave
//===========================================================================
#define N 1000
#define RATE 20000
short int wavetable[N];
int step = 0;
int offset = 0;

void init_wavetable(void) {
	for(int i=0; i < N; i++)
		wavetable[i] = 32767 * sin(2 * M_PI * i / N);
}

void set_freq(float f) {
	step = (f * N / 20000) * (1<<16); //assume F_DAC = 20k
}

void setup_dac() {
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN; //enable clock to A
	GPIOA->MODER |= 3<<(2*4); //set PA4 to analog

	RCC->APB1ENR |= RCC_APB1ENR_DACEN;
	DAC->CR |= DAC_CR_TEN1;
	DAC->CR |= DAC_CR_TSEL1;
	DAC->CR |= DAC_CR_EN1;
}
