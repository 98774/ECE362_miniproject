#include "timer.h"
#include "draw.h"
#include "lcd.h"
#include "STM32f0xx.h"

#define TIME 60
int rows = 0;
int gameRunning;
int gameOver;
int timSpeed = 500;

/*******************************************************************************
Author: Bryce Sasser
Date: 11/11/2021
Description: initializes tim6 to trigger an interrupt at 60HZ
*******************************************************************************/
void init_TIM6(){

	rows = 0;
    //Turn on clock for TIM6
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;

    TIM6->CR1 &= ~TIM_CR1_CEN;
    //Turn on the interrupt trigger for timer
    TIM6->DIER |= TIM_DIER_UIE;

    //Set prescaler and auto-reload value to achieve 30 Hz
    TIM6->PSC = timSpeed-1;
    TIM6->ARR = 48000000 / TIME - 1;

    //Turn on the timer
    TIM6->CR1 |= TIM_CR1_CEN;

    //Unmask the interrupt
    NVIC->ISER[0] |= 1 << TIM6_DAC_IRQn;

    return;
}

void TIM6_DAC_IRQHandler(void){
    TIM6->SR &= ~TIM_SR_UIF;


    if(rows == 320){
    	TIM6->CR1 &= ~TIM_CR1_CEN;
		gameOver = 1;
    } else {
    	rows++;
    }

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
