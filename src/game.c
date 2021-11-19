#include "game.h"
#include "STM32f0xx.h"

void init_buttons(){

    // Turn on clock for the GPIO pins
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    // Set the mode of GPIOC pins to be inputs
    GPIOC->MODER &= ~(GPIO_MODER_MODER4 |
    		GPIO_MODER_MODER5 |
			GPIO_MODER_MODER6 |
			GPIO_MODER_MODER7 |
			GPIO_MODER_MODER8);

    // Set the GPIOC pins to have pull up resistors
    GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPDR4 |
    		GPIO_PUPDR_PUPDR5 |
			GPIO_PUPDR_PUPDR6 |
			GPIO_PUPDR_PUPDR7 |
			GPIO_PUPDR_PUPDR8);

    GPIOC->PUPDR |= GPIO_PUPDR_PUPDR4_0 |
    		GPIO_PUPDR_PUPDR5_0 |
            GPIO_PUPDR_PUPDR6_0 |
			GPIO_PUPDR_PUPDR7_0 |
			GPIO_PUPDR_PUPDR8_0;


    //unmask the interrupts
    EXTI->IMR |= EXTI_IMR_MR4 |
    		EXTI_IMR_MR5 |
			EXTI_IMR_MR6 |
			EXTI_IMR_MR7 |
			EXTI_IMR_MR8;

    //Set the EXTI interrupts for pins 4-8 to PC
    SYSCFG->EXTICR[1] = SYSCFG_EXTICR2_EXTI4_PC |
    		SYSCFG_EXTICR2_EXTI5_PC |
			SYSCFG_EXTICR2_EXTI6_PC |
			SYSCFG_EXTICR2_EXTI7_PC;

    SYSCFG->EXTICR[2] = SYSCFG_EXTICR3_EXTI8_PC;

    // Set the EXTI interrupts
       EXTI->FTSR |= EXTI_FTSR_TR4 |
       		EXTI_FTSR_TR5 |
   			EXTI_FTSR_TR6 |
   			EXTI_FTSR_TR7 |
   			EXTI_FTSR_TR8;

    //Unmask interrupt
    NVIC->ISER[0] |= 1 << EXTI4_15_IRQn;
}


void EXTI4_15_IRQHandler(void){


    if (~GPIOC->IDR & GPIO_IDR_4){
        //Pause
    	spi_display1("Pause");
    }
		if (~GPIOC->IDR & GPIO_IDR_5){
			//Right Down
			spi_display1("RD");
		}
		if (~GPIOC->IDR & GPIO_IDR_6){
			//Right Up
			spi_display1("RU");
		}
		if (~GPIOC->IDR & GPIO_IDR_7){
			//Left Down
			spi_display1("LD");
		}
		if (~GPIOC->IDR & GPIO_IDR_8){
			//Left Up
			spi_display1("LU");
		}

    if ((GPIOC->IDR & GPIO_IDR_4) && (GPIOC->IDR & GPIO_IDR_5) &&
            (GPIOC->IDR & GPIO_IDR_6) && (GPIOC->IDR & GPIO_IDR_7) &&
            (GPIOC->IDR & GPIO_IDR_8)){

        EXTI->PR &= ~(EXTI_PR_PR4 | EXTI_PR_PR5 | EXTI_PR_PR6 | EXTI_PR_PR7
                | EXTI_PR_PR8);

    }
}
