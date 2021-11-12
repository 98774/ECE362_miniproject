#include "game.h"
#include "STM32f0xx.h"

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
