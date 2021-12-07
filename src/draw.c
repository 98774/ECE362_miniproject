#include "mazegen.h"
#include "draw.h"
#include "lcd.h"

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


void Draw_Start_Screen()
{
	char *title = "MAZE RUNNER\0";
	char *info = "Navigate to the goal before";
	char *info2 = "the time runs out!";
	LCD_Clear(MAGENTA);
	LCD_DrawString(75, 20, BLACK, WHITE, title, LARGE, 0);
	LCD_DrawString(10, 40, BLACK, RED, info, LARGE, 0);
	LCD_DrawString(50, 56, BLACK, RED, info2, LARGE, 0);
}

void Draw_Game_Over()
{
    char *title = "GAME OVER!\0";
    char *info = "Press RESET to";
    char *info2 = "start over!";
    LCD_Clear(MAGENTA);
    LCD_DrawString(75, 20, BLACK, WHITE, title, LARGE, 0);
    LCD_DrawString(10, 40, BLACK, RED, info, LARGE, 0);
    LCD_DrawString(50, 56, BLACK, RED, info2, LARGE, 0);
}

void Draw_Timebar(){
	LCD_DrawFillRectangle((OFFSETX + SIZE * (XSIZE - 2) * 2),
	        0,
	        (OFFSETX + SIZE * (XSIZE - 2) * 2) + XSIZE,
	        (OFFSETY + SIZE * (YSIZE - 2) * 2) + YSIZE, MAGENTA);
}
