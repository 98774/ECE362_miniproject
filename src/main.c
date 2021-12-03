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
#include "dma.h"

/*******************************************************************************
Author: Jonathon Snyder
Date: 11/11/2021
Description: initializes the gpio pins to their respective functions

Pins:
SPI1:
	--PA15: SPI1_NSS
 	--PA5: SPI_SCK
 	--//PB4: SPI1_MISO
 	--PA7: SPI1_MOSI

SPI2:
 	--PB12: SPI2_NSS
 	--PB13: SPI2_SCK
 	--PB14: SPI2_MISO
 	--PB15: SPI2_MOSI

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

#define SAMPLES 30


int main() {
	//Storage and initialization for OLED
	uint16_t oled_msg[34]; //array used for DMA on the OLED
	uint16_t dac_sample_array[SAMPLES];
	/*
	//setup SPI for the OLED
	setup_spi1();
	spi_init_oled(oled_msg);

	//setup DMA for the OLED
    setup_dma_ch3();
    enable_dma_ch3();
    */


	//Initialization for OLED
    setup_spi1();
    spi_init_oled();
    //setup_dma_ch3();
    //enable_dma_ch3();
    spi_display1("Hello again,");
    spi_display2("Test display");

    init_buttons();
    //Initializetion for DAC
    init_tim7_dac();
    setup_dma_dac(dac_sample_array, SAMPLES);
    init_dac(dac_sample_array);

    setfreq(500);

	for(;;){
		//Plays major scale by setting frequency

		setfreq(pow(2, (-12.0/12))*440);
		nano_wait(1000000000);

		setfreq(pow(2, (-10.0/12))*440);
		nano_wait(1000000000);

		setfreq(pow(2, (-8.0/12))*440);
		nano_wait(1000000000);

		setfreq(pow(2, (-7.0/12))*440);
		nano_wait(1000000000);

		setfreq(pow(2, (-5.0/12))*440);
		nano_wait(1000000000);

		setfreq(pow(2, (-3.0/12))*440);
		nano_wait(1000000000);

		setfreq(pow(2, (-1.0/12))*440);
		nano_wait(1000000000);
	}
}
#endif

/*JONATHON'S CODE FOR DAC PLAYBACK
Need to incorporate this into main

#include "stm32f0xx.h"

#define STEP25
#if defined(STEP25)
#include <stdio.h>
#include "ff.h"
#include "sdcard.h"

//relevant constants for sdcard wav file
#define BUFFSIZE 4000
uint8_t header[44];
uint8_t data[BUFFSIZE];
uint8_t data2[BUFFSIZE];
UINT br; //stores the number of bytes read
FIL fp; //File object to be returned
FRESULT fr; //return code of f_open
char *fileName;
FATFS fs_storage;
int main() {


    FATFS *fs = &fs_storage;
    f_mount(fs, "", 1);

    Init_Play_Devices(data);
    uint16_t sampSize = play("16bit.wav\0", header, data, data2, &fp, &br);
    FIL fstart = fp;
    int currArr = 1; //stores which array we're in

    for(;;){
        if(DMA1->ISR & DMA_ISR_TCIF3){
            currArr = Update_Array(currArr, data, data2, &fp, &br, sampSize, fstart);
        }
    }


}

*/

