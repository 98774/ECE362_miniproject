#ifndef __MAZEGEN_H__
#define __MAZEGEN_H__

#include <stdbool.h>

//maze dimensions
#define XSIZE 24
#define YSIZE 32

typedef struct cell{
	bool in;  //Is this cell in the maze?
	bool up;  //Does the wall above this cell exist?
	bool left;//Does the wall to the left of this cell exist?
	bool isWall;
	int prevx, prevy; //The coordinates of the previous cell, used for backtracking.
}cell;

int Build_Maze(cell *MAZE[]);
void initialize(cell *MAZE[]);
void generate(cell *MAZE[]);
void savebmp(cell *MAZE[], int xspecial, int yspecial);

#endif
