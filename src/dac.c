#include "dac.h"
#include "STM32f0xx.h"


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


void TIM7_IRQHandler(void) {
	TIM7->SR &= ~TIM_SR_UIF; //acknowledge the interrupt
	DAC->SWTRIGR |= DAC_SWTRIGR_SWTRIG1;
	offset += step;
	if((offset >> 16) > N) //check to reset offset
		offset -= N * (1 << 16); //fixed point rep of N
	int newVal = wavetable[offset>>16]; //get sample at offset
	newVal *= volume;
	newVal = newVal >> 16; //shift right
	newVal += 2048; //centered on 0
	//value becomes (wavetable[offset>>16] * 2047) >> 16 + 2048
	DAC->DHR12R1 = newVal;

}
