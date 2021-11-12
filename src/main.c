/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f0xx.h"
			

//========================================================
//MAIN FUNCTIONS
//========================================================

#define MAIN
#if defined(MAIN)
#include <stdio.h>

#include "dac.h"
#include "spi.h"
#include "game.h"
#include "timer.h"

/*******************************************************************************
Author: Jonathon Snyder
Date: 11/11/2021
Description: initializes the gpio pins to their respective functions

Pins:
SPI1:
	--PA15: SPI1_NSS
 	--PB3: SPI_SCK
 	--PB4: SPI1_MISO
 	--PB5: SPI1_MOSI

SPI2:
 	--PB12: SPI2_NSS
 	--PB13: SPI2_SCK
 	--PB14: SPI2_MISO
 	--PB15: SPI2_MISO

 DAC:
     --PA4: DAC

Controller GPIO (Need 1 interrupt per button so 6 unique pins):
	PC5-6: C1
	PC7-8: C2
	PC4 will be pause button
*******************************************************************************/

/*******************************************************************************
Author: Bryce Sasser
Date: 11/11/2021
Description: initialize all the pins for the for gpio use
        the buttons will be active low and trigger interrupt on falling edge
*******************************************************************************/
void init_gpio(){

    // Turn on clock for the GPIO pins
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOAEN;

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
}

int main() {

	for(;;);
}
#endif
