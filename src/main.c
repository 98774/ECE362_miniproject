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
extern cell WALLS[WALLXSIZE][WALLYSIZE];
int px = 0;
int py = 0;
extern int gameRunning;
extern int gameOver;
extern int rows;
extern int timSpeed;

void DMA1_CH2_3_DMA2_CH1_2_IRQHandler(){
	if(DMA1->ISR & DMA_ISR_TCIF3){
		currArr = Update_Array(currArr, data, data2, &fp, &br, sampSize, fstart);
	}
}

//CONSTANTS FOR LCD
int main() {
	int firstStart = 1;
	int xCurr = 1;
	int yCurr = 1;
	int goalX = 0;
	int goalY = 0;
	int totalMazes = 0;

	//Storage and initialization for OLED
	FATFS *fs = &fs_storage;
	f_mount(fs, "", 1);

	Init_Play_Devices(data);
	sampSize = play("music.wav\0", header, data, data2, &fp, &br);
	fstart = fp;


	LCD_Setup(); //initialize the LCD
	LCD_Clear(BLACK);
	Draw_Start_Screen();

	init_buttons();

	for(;;){
		//Update timer
		if(gameRunning)
			LCD_DrawFillRectangle((OFFSETX + SIZE * (XSIZE - 2) * 2),
		    	        0,
		    	        (OFFSETX + SIZE * (XSIZE - 2) * 2) + XSIZE,
		    	        rows, GREEN);

		nano_wait(60000000); //get inputs at a reasonable speed

		//UP
		if(!(GPIOC->IDR & GPIO_IDR_4) && gameRunning == 1){
			yCurr = move_up(xCurr, yCurr);
			//char title [1];
			//title[0] = WALLS[xCurr][yCurr].isWall;
			//LCD_DrawString(75, 20, BLACK, BLUE, title, 16, 0);
		}

		//Down
		if(!(GPIOC->IDR & GPIO_IDR_5) && gameRunning == 1){
			yCurr = move_down(xCurr, yCurr);
			//char *title = "THIS IS TEXT";
			//title[0] = (char) WALLS[xCurr][yCurr].isWall;
			//LCD_DrawString(75, 20, BLACK, BLUE, title, 16, 0);
		}

		//LEFT
		if(!(GPIOC->IDR & GPIO_IDR_6) && gameRunning == 1){
			xCurr = move_left(xCurr,yCurr);
			//char title [1];
			//title[0] = WALLS[xCurr][yCurr].isWall;
			//LCD_DrawString(75, 20, BLACK, BLUE, title, 16, 0);
		}

		//Right
		if(!(GPIOC->IDR & GPIO_IDR_7) && gameRunning == 1){
			xCurr = move_right(xCurr, yCurr);
			//char title [1];
			//title[0] = WALLS[xCurr][yCurr].isWall;
			//LCD_DrawString(75, 20, BLACK, BLUE, title, 16, 0);
		}

		//Start
		if(!(GPIOC->IDR & GPIO_IDR_8)){
			if(firstStart){
				srandom((unsigned int) TIM15->CNT); //seed random number generator with system time
				firstStart = 0;
			}
			if(!gameRunning){
				//start game
				gameRunning = 1;
				timSpeed = 500;
				xCurr = 0;
				yCurr = 0;
				totalMazes = 0;
				LCD_Clear(BLACK);
				Build_Maze();
				Draw_Timebar();
				Set_Goal(&goalX, &goalY);
				init_TIM6();
			} else {
				//game running

			}
		}

		if(gameOver){
			gameOver = 0;
			gameRunning = 0;
	    	LCD_Clear(BLACK);
			Draw_Game_Over(totalMazes);
			nano_wait(1000000);
		}

		if(xCurr == goalX && yCurr == goalY){
			gameRunning = 0;
			xCurr = 0;
			yCurr = 0;
			timSpeed -= (timSpeed >= 0 && timSpeed >= 200) ? 50 : 0;
			totalMazes++;
			Draw_Completed_Message(totalMazes);
			LCD_Clear(BLACK);
			Build_Maze();
			Draw_Timebar();
			Set_Goal(&goalX, &goalY);
			init_TIM6();
			gameRunning = 1;
			//LCD_Clear(BLACK);
		}
    }
}
#endif
