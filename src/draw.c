#include "mazegen.h"
#include "draw.h"
#include "lcd.h"



void Draw_Grid(cell *MAZE[]){
	for(int x = 0; x < XSIZE; x++){
		for(int y = 0; y < YSIZE; y++){
			if(MAZE[x][y].in == 0)
				LCD_DrawFillRectangle(10*x, 10*y, 10 * (x + 1), 10 * (y + 1), RED);

		}
	}
}

void Draw_Cell(int x, int y, int color){
	LCD_DrawFillRectangle(SIZE*x+OFFSETX, SIZE*y+OFFSETY, SIZE*(x+1)+OFFSETX, SIZE*(y+1)+OFFSETY, color);
}
