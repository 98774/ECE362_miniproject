#include "timer.h"
#include "STM32f0xx.h"


// TIM6 will be used as the refresh rate of the LEDs
void configure_TIM6(){

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

//TIM7 will be used as the speed of the game
void configure_TIM7(){
    return;
}
