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

#include "ff.h"
#include "sdcard.h"
#include "mazegen.h"
#include "lcd.h"
#include "draw.h"

//relevant constants for SD Card
#define BUFFSIZE 4000
uint8_t header[44];
uint8_t data[BUFFSIZE];
uint8_t data2[BUFFSIZE];
UINT br; //stores the number of bytes read
FIL fp; //File object to be returned
FRESULT fr; //return code of f_open
char *fileName;
FATFS fs_storage;

//Constants for MAZE
cell MAZE[XSIZE][YSIZE];


//CONSTANTS FOR LCD

int main() {
	//Storage and initialization for OLED
	FATFS *fs = &fs_storage;
	f_mount(fs, "", 1);

	Init_Play_Devices(data);
	uint16_t sampSize = play("16bit.wav\0", header, data, data2, &fp, &br);
	FIL fstart = fp;
	int currArr = 1; //stores which array we're in

	LCD_Setup(); //initialize the LCD
	LCD_Clear(0x0000);

	init_buttons();


	//Build Maze
	Build_Maze(MAZE);
	Draw_Grid(MAZE);


	for(;;){
		//DAC audio control
		if(DMA1->ISR & DMA_ISR_TCIF3){
			currArr = Update_Array(currArr, data, data2, &fp, &br, sampSize, fstart);
		}

		//LCD_DrawFillRectangle(0, 0, 20, 10, RED);


		//Button Detection




    }
}
#endif
