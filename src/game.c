#include "game.h"
#include "STM32f0xx.h"

void init_buttons(){

    // Turn on clock for the GPIO pins
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    // Set the mode of GPIOC pins to be inputs
    GPIOC->MODER &= ~(GPIO_MODER_MODER4 | GPIO_MODER_MODER5 | GPIO_MODER_MODER6
            | GPIO_MODER_MODER7 | GPIO_MODER_MODER8);

    // Set the GPIOC pins to have pull up resistors
    GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR4 | GPIO_PUPDR_PUPDR5 | GPIO_PUPDR_PUPDR6
            | GPIO_MODER_MODER7 | GPIO_MODER_MODER8);
    GPIOC->PUPDR |= GPIO_PUPDR_PUPDR4_0 | GPIO_PUPDR_PUPDR5_0 |
            GPIO_PUPDR_PUPDR6_0 | GPIO_PUPDR_PUPDR7_0 | GPIO_PUPDR_PUPDR8_0;

    // Set the EXTI interrupts
    EXTI->FTSR |= EXTI_FTSR_TR4 | EXTI_FTSR_TR5 | EXTI_FTSR_TR6 | EXTI_FTSR_TR7
            | EXTI_FTSR_TR8;

    //Unmask interrupt
    NVIC->ISER[0] |= 1 << EXTI4_15_IRQn;
}


void EXTI4_15_IRQHandler(void){


    if (GPIOC->IDR | GPIO_IDR_4){
        //Pause
    }
    if (GPIOC->IDR | GPIO_IDR_5){
        //Right Down
    }
    if (GPIOC->IDR | GPIO_IDR_6){
        //Right Up
    }
    if (GPIOC->IDR | GPIO_IDR_7){
        //Left Down
    }
    if (GPIOC->IDR | GPIO_IDR_8){
        //Left Up
    }

    if (!((GPIOC->IDR | GPIO_IDR_4) || (GPIOC->IDR | GPIO_IDR_5) ||
            (GPIOC->IDR | GPIO_IDR_6) || (GPIOC->IDR | GPIO_IDR_7) ||
            (GPIOC->IDR | GPIO_IDR_8))){

        EXTI->PR &= ~(EXTI_PR_PR4 | EXTI_PR_PR5 | EXTI_PR_PR6 | EXTI_PR_PR7
                | EXTI_PR_PR8);

    }
}
