#ifndef __MAZEGEN_H__
#define __MAZEGEN_H__

#include <stdbool.h>


#define UP 0     //-y
#define DOWN 1   //+y
#define LEFT 2   //-x
#define RIGHT 3  //+x
#define OUTFILE "MAZE"
#define WHITE Draw_Cell(x-1, y-1, 0xffff)
#define BLACK Draw_Cell(x-1, y-1, 0x0000)
#define RED   Draw_Cell(x-1, y-1, 0xf800)

#define nodeadend//generate a maze without any dead ends! (consequently, many solutions to maze)
//#define prim    //enable this to generate mazes using prim's algorithm.
#define backtrack//enable this to generate mazes using depth-first search. Don't enable both.
//#define movie   //this option spams bitmaps to illustrate each step of generation.

#define XSIZE 14
#define YSIZE 19

typedef struct cell{
	bool in;  //Is this cell in the maze?
	bool up;  //Does the wall above this cell exist?
	bool left;//Does the wall to the left of this cell exist?
	bool isWall;
	int prevx, prevy; //The coordinates of the previous cell, used for backtracking.
}cell;

int Build_Maze();
void initialize();
void generate();
void savebmp(int xspecial, int yspecial);

#endif
