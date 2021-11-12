#include "timer.h"
#include "STM32f0xx.h"


void configure_TIM6(){

    TIM6->DIER |= TIM_DIER_UIE;

    return;
}

void configure_TIM7(){
    return;
}
