#include "game.h"
#include "STM32f0xx.h"
#include "draw.h"
#include "mazegen.h"
#include "lcd.h"

//#define TRAIL BLACK
int gameRunning = 0;
extern cell WALLS[WALLXSIZE][WALLYSIZE];

void Move(int dir, int playerX, int playerY){
	return;
}

void init_buttons(){

    // Turn on clock for the GPIO pins
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGCOMPEN;

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
    //NVIC->ISER[0] |= 1 << EXTI4_15_IRQn;
}


//void EXTI4_15_IRQHandler(void){
//}

int move_down(int xCurr, int yCurr)
{
	if((yCurr + 1) < WALLYSIZE && !WALLS[xCurr][yCurr + 1].isWall)
	{
		Draw_Cell(xCurr,yCurr, BACKCOLOR);
		yCurr += 1;
		Draw_Cell(xCurr,yCurr,0xf800);

	}

	return yCurr;
}
int move_up(int xCurr, int yCurr)
{
	if( (yCurr) > 0 && !WALLS[xCurr][yCurr - 1].isWall)

	{
		Draw_Cell(xCurr,yCurr, BACKCOLOR);
		yCurr -= 1;
		Draw_Cell(xCurr,yCurr,0xf800);

	}

	return yCurr;
}

int move_right(int xCurr, int yCurr)
{
	if(xCurr + 1 < WALLXSIZE && !WALLS[xCurr + 1][yCurr].isWall)
	{
		Draw_Cell(xCurr,yCurr, BACKCOLOR);
		xCurr += 1;
		Draw_Cell(xCurr,yCurr,0xf800);
	}

	return xCurr;
}

int move_left(int xCurr, int yCurr)
{
	if( (xCurr) > 0 && !WALLS[xCurr - 1][yCurr].isWall)
	{
		Draw_Cell(xCurr,yCurr, BACKCOLOR);
		xCurr -= 1;
		Draw_Cell(xCurr,yCurr,0xf800);

	}

	return xCurr;
}

//sets goalX and goalY to the coords of the goal and draws a goal there
void Set_Goal(int *goalX, int *goalY){
	*goalX = (rand() % (XSIZE - 1)) * 2 - 2;
	if(*goalX < 1)
		*goalX += 2;

	*goalY = (rand() % (YSIZE - 1)) * 2 - 2;
	if(*goalY < 1)
		*goalY += 2;


	Draw_Goal(*goalX, *goalY, GREEN);
}
