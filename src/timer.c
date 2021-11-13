#include "timer.h"
#include "STM32f0xx.h"
#define SAMPLES 30 //stores the number of samples per period

/*******************************************************************************
Author: Bryce Sasser
Date: 11/11/2021
Description: initializes tim6 to trigger an interrupt at 30 Hz;
            used as frame rate of the game
*******************************************************************************/
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

void TIM6_DAC_IRQHandler(void){
    TIM6->SR &= ~TIM_SR_UIF;
    return;
}

/*******************************************************************************
Author: Bryce Sasser
Date: 11/11/2021
Description: initializes tim2 to trigger an interrupt at 2 Hz, which is
                the speed of the game
*******************************************************************************/
void init_TIM2(){
    //Turn on clock for TIM2
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    //Turn on the interrupt trigger for timer
    TIM2->DIER |= TIM_DIER_UIE;

    //Set prescaler and auto-reload value to achieve 2 Hz
    TIM2->PSC = 24000 - 1;
    TIM2->ARR = 1000 - 1;

    //Turn on the timer
    TIM2->CR1 |= TIM_CR1_CEN;

    //Unmask the interrupt
    NVIC->ISER[0] |= 1 << TIM2_IRQn;

    return;
}

void TIM2_IRQHandler(void){
    TIM6->SR &= ~TIM_SR_UIF;
    return;
}

/*******************************************************************************
Author: Jonathon Snyder
Date: 11/11/2021
Description: initializes timer 7 and enables the DMA request.
	The DAC subsytem uses this timer to drive the audio output.
*******************************************************************************/

void init_tim7_dac(void) {
	//Step 3: Configure timer
	RCC->APB1ENR |= RCC_APB1ENR_TIM7EN; //enable RCC clock to tim 7

	TIM7->ARR = (48000000 / (SAMPLES)) - 1; //trigger fre * samples / sec
	TIM7->PSC = 0;
	TIM7->CR2 &= ~TIM_CR2_MMS;
	TIM7->CR2 |=  TIM_CR2_MMS_1; //enable update trigger on timer edge
	TIM7->DIER |= TIM_DIER_UDE; //enable dma requests
	TIM7->CR1 |= TIM_CR1_CEN; //enable timer


}

/*******************************************************************************
Author: Jonathon Snyder
Date: 11/11/2021
Description: initializes timer 7 and enables the interrupt.
    The DAC subsytem uses this timer to drive the audio output.
*******************************************************************************/
/*
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
*/

