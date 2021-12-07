#include "mazegen.h"
#include "draw.h"
#include "lcd.h"
#include "spi.h"
#include <stdio.h>
#define SMALL 12
#define LARGE 16


void Draw_Grid(cell *MAZE[]){
	for(int x = 0; x < XSIZE; x++){
		for(int y = 0; y < YSIZE; y++){
			if(MAZE[x][y].in == 0)
				LCD_DrawFillRectangle(10*x, 10*y, 10 * (x + 1), 10 * (y + 1), RED);

		}
	}
}

void Draw_Cell(int x, int y, int color){
	LCD_DrawFillRectangle(SIZE*x+OFFSETX, SIZE*y+OFFSETY, SIZE*(x+1)+OFFSETX - 1, SIZE*(y+1)+OFFSETY - 1, color);
}

void Draw_Goal(int x, int y, int color){
	LCD_DrawFillRectangle(SIZE*x+OFFSETX , SIZE*y+OFFSETY, SIZE*(x+1)+OFFSETX - 1, SIZE*(y+1)+OFFSETY - 1, color);
}

void Draw_Completed_Message(int totalMazes){
	char numMazes[5];
	sprintf(numMazes,"%d", totalMazes);
	char *mazesNot1 = "Score:   ";


	LCD_Clear(WHITE);
	LCD_DrawString(80, 150, BLACK, WHITE, mazesNot1, LARGE, 0);
	LCD_DrawString(140, 150, RED, WHITE, numMazes, LARGE, 0);
	nano_wait(1000000000);
}

void Draw_Start_Screen()
{
	char *title = "MAZE RUNNER\0";
	char *info = "Navigate to the goal before";
	char *info2 = "the time runs out!";
	LCD_Clear(WHITE);
	LCD_DrawString(75, 20, BLACK, WHITE, title, LARGE, 0);
	LCD_DrawString(10, 40, BLACK, WHITE, info, LARGE, 0);
	LCD_DrawString(50, 56, BLACK, WHITE, info2, LARGE, 0);
}

void Draw_Game_Over(int totalMazes)
{
	char numMazes[5];
	sprintf(numMazes,"%d", totalMazes);
    char *title = "GAME OVER!\0";
    char *mazesNot1 = "You completed    mazes!";
    char *mazes1 = "You completed 1 maze!";
    char *info = "Press       to";
    char *reset = "RESET";
    char *info2 = "start over!";

    LCD_Clear(BLACK);
    LCD_DrawString(75, 20, RED, BLACK, title, LARGE, 1);
    if(totalMazes == 1)
    	LCD_DrawString(25, 40, WHITE, BLACK, mazes1, LARGE, 1);
    else {
    	LCD_DrawString(25, 40, WHITE, WHITE, mazesNot1, LARGE, 1);
    	LCD_DrawString(140, 40, RED, WHITE, numMazes, LARGE, 1);
    }
	LCD_DrawString(55, 60, WHITE, RED, info, LARGE, 1);
    LCD_DrawString(105, 60, RED, RED, reset, LARGE, 1);
    LCD_DrawString(70, 76, WHITE, RED, info2, LARGE, 1);
}

void Draw_Timebar(){
	LCD_DrawFillRectangle((OFFSETX + SIZE * (XSIZE - 2) * 2),
	        0,
	        (OFFSETX + SIZE * (XSIZE - 2) * 2) + XSIZE,
	        (OFFSETY + SIZE * (YSIZE - 2) * 2) + YSIZE, MAGENTA);
}
