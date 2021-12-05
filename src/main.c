#include "stm32f0xx.h"
#include <stdio.h>
			
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
#define MAIN
#if defined(MAIN)

#include "dac.h"
#include "spi.h"
#include "game.h"
#include "timer.h"
#include "dma.h"
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
int currArr = 1;
uint16_t sampSize;
FIL fstart;

//Constants for MAZE
extern cell MAZE[XSIZE][YSIZE];


void DMA1_CH2_3_DMA2_CH1_2_IRQHandler(){
	if(DMA1->ISR & DMA_ISR_TCIF3){
		currArr = Update_Array(currArr, data, data2, &fp, &br, sampSize, fstart);
	}
}

//CONSTANTS FOR LCD
int main() {
	//Storage and initialization for OLED
	FATFS *fs = &fs_storage;
	f_mount(fs, "", 1);

	Init_Play_Devices(data);
	sampSize = play("8bit.wav\0", header, data, data2, &fp, &br);
	fstart = fp;
	int gameStarted = 0;

	LCD_Setup(); //initialize the LCD
	LCD_Clear(BLACK);
	Draw_Start_Screen();

	init_buttons();

	for(;;){
		//DAC audio control


		if(!(GPIOC->IDR & GPIO_IDR_4)){
			//UP
		}


		if(!(GPIOC->IDR & GPIO_IDR_5)){
			//DOWN
		}


		if(!(GPIOC->IDR & GPIO_IDR_6)){
			//LEFT
		}


		if(!(GPIOC->IDR & GPIO_IDR_7)){
			//RIGHT
		}

		if(!(GPIOC->IDR & GPIO_IDR_8)){
			if(!gameStarted){
				//start game
				gameStarted = 1;
				Build_Maze();
				Draw_Timebar();
			} else {
				//game running
			}
		}
    }
}
#endif
