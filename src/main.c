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
	--PA4: SPI1_NSS
 	--PA5: SPI_SCK
 	--PA6: SPI1_MISO
 	--PA7: SPI1_MOSI

SPI1:
 	--PB12: SPI2_NSS
 	--PB13: SPI2_SCK
 	--PB14: SPI2_MISO
 	--PB15: SPI2_MISO
*******************************************************************************/
void init_gpio(){

}

int main() {

	for(;;);
}
#endif
