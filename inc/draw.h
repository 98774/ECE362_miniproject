#ifndef __DRAW_H__
#define __DRAW_H__

#include "mazegen.h"
#define OFFSETX 10
#define OFFSETY 11
#define SIZE 9


void Draw_Cell(int x, int y, int color);
void Draw_Goal(int x, int y, int color);
void Draw_Game_Over();
void Draw_Grid(cell *MAZE[]);
void Draw_Start_Screen();
void Draw_Completed_Message(int totalMazes);
void Draw_Timebar();
#endif
