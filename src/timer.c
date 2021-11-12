#include "timer.h"
#include "STM32f0xx.h"


// TIM6 will be used as the refresh rate of the LEDs
void init_TIM6(){

    //Turn on clock for TIM6
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

    //Turn on the interrupt trigger for timer
    TIM6->DIER |= TIM_DIER_UIE;

    //Set prescaler and auto-reload value to achieve 30 Hz
    TIM6->PSC = 1600 - 1;
    TIM6->ARR = 1000 - 1;

    //Turn on the timer
    TIM6->CR1 |= TIM_CR1_CEN;

    //Unmask the interrupt
    NVIC->ISER[0] |= 1 << TIM6_DAC_IRQn;

    return;
}
/*******************************************************************************
Author: Jonathon Snyder
Date: 11/11/2021
Description: initializes timer 7 and enables the interrupt.
	The DAC subsytem uses this timer to drive the audio output.
*******************************************************************************/
void init_TIM7(void) {
	int psc = 8000000 / RATE - 1;
	RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
	TIM7->ARR = 6-1;
	TIM7->PSC = psc;
	TIM7->CR1 |= TIM_CR1_CEN;
	//NVIC->ISER |= NVIC_ISER_TIM7_IRQHandler;
	//need to use EXTI to unmask
	TIM7->DIER |= TIM_DIER_UIE;
	NVIC->ISER[0] |= (1 << 18); //TIM7 Interrupt

}

/*******************************************************************************
Author: Jonathon Snyder
Date: 11/11/2021
Description: initializes timer 7 and enables the interrupt.
    The DAC subsytem uses this timer to drive the audio output.
*******************************************************************************/
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
