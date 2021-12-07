//Purpy Pupple's amazing maze generator.
//Released under the CC-BY-SA 3.0 License and the GFDL
/*************************************
Found on:
https://en.wikipedia.org/wiki/User:Dllu/Maze
*************************************/
/************************************
Modified by Manas Tanneeru
Modified by Jonathon Snyder
************************************/
#include "stm32f0xx.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "mazegen.h"
#include "lcd.h"
#include "draw.h"


cell MAZE[XSIZE][YSIZE];
cell WALLS[WALLXSIZE][WALLYSIZE];
long numin=1;     //Number of cells in the maze.


int Build_Maze()
{
	numin = 1;
	initialize();      //initialize the maze
	generate();        //generate the maze
	savebmp(1,1);
	return 0;
}

void initialize(){
	//Initialize the maze!
	for(int x=0;x<XSIZE;x++){
		for(int y=0;y<YSIZE;y++){
			//The maze cells on the edges of the maze are "in" to provide padding. Otherwise, all maze cells are not in.
			MAZE[x][y].in   = (x==0||x==XSIZE-1||y==0||y==YSIZE-1)?1:0;
			//All maze cells have all walls existing by default, except the perimeter cells.
			MAZE[x][y].up   = (x==0||x==XSIZE-1||y==0)?0:1;
			MAZE[x][y].left = (x==0||y==0||y==YSIZE-1)?0:1;
			MAZE[x][y].prevx = 0;
			MAZE[x][y].prevy = 0;
		}
	}
	return;
}

void generate(){
	int xcur=10, ycur=10;//start growing from the corner. It could theoretically start growing from anywhere, doesn't matter.
	MAZE[xcur][ycur].in = 1;
	int whichway;
	bool success;
	do{

#ifdef backtrack
		while( MAZE[xcur][ycur-1].in&&MAZE[xcur][ycur+1].in&&
			   MAZE[xcur-1][ycur].in&&MAZE[xcur+1][ycur].in ){
				   //If all the neighbourhood cells are in, backtrack.
				int xcur2=MAZE[xcur][ycur].prevx;
				ycur=MAZE[xcur][ycur].prevy;
				xcur=xcur2;
		}
#endif
#ifdef prim
		do{
			//randomly find a cell that's in the maze
			xcur=rand()%(XSIZE-2)+1;
			ycur=rand()%(YSIZE-2)+1;
		}while(!MAZE[xcur][ycur].in ||
			(MAZE[xcur][ycur-1].in&&MAZE[xcur][ycur+1].in&&
			MAZE[xcur-1][ycur].in&&MAZE[xcur+1][ycur].in)); //edited.
#endif
		do{
			//Randomly grow the maze if possible.
			success=0;
			whichway=rand()%4;
			switch(whichway){
			case UP:
				if(!MAZE[xcur][ycur-1].in){
					success=1;
					MAZE[xcur][ycur].up=0;
					MAZE[xcur][ycur-1].prevx=xcur;
					MAZE[xcur][ycur-1].prevy=ycur;
					ycur--;
				}
				break;
			case DOWN:
				if(!MAZE[xcur][ycur+1].in){
					success=1;
					MAZE[xcur][ycur+1].up=0;
					MAZE[xcur][ycur+1].prevx=xcur;
					MAZE[xcur][ycur+1].prevy=ycur;
					ycur++;
				}
				break;
			case LEFT:
				if(!MAZE[xcur-1][ycur].in){
					success=1;
					MAZE[xcur][ycur].left=0;
					MAZE[xcur-1][ycur].prevx=xcur;
					MAZE[xcur-1][ycur].prevy=ycur;
					xcur--;
				}
				break;
			case RIGHT:
				if(!MAZE[xcur+1][ycur].in){
					success=1;
					MAZE[xcur+1][ycur].left=0;
					MAZE[xcur+1][ycur].prevx=xcur;
					MAZE[xcur+1][ycur].prevy=ycur;
					xcur++;
				}
				break;
			}
		}while(!success);
		MAZE[xcur][ycur].in=1;
		numin++; //Every iteration of this loop, one maze cell is added to the maze.
	}while(numin<(XSIZE-2)*(YSIZE-2));

	return;
}

void savebmp(int xspecial, int yspecial){

	int x, y, n;
	int width=(XSIZE-1)*2-1;
	int height=(YSIZE-1)*2-1;

	//Actual writing of data begins here:
	for(y = 0; y <= height - 1; y++){
		for(x = 0; x <= width - 1; x++){
			nano_wait(500000);
			if(x%2 == 1 && y%2 == 1){
				if(x > 0 && y > 0)
					WALLS[x - 1][y - 1].isWall = 0; //all cells on even (odd, odd) = not wall
				if(x/2+1 == xspecial && y/2+1 == yspecial) {
					Draw_Cell(x-1, y-1, RED);
					//WALLS[x][y].isWall = 0;

				} else {
					//if(MAZE[x/2+1][y/2+1].in){
					Draw_Cell(x-1, y-1, BACKCOLOR);

//					} else {
//						Draw_Cell(x-1, y-1, WHITE); //black
//						WALLS[x][y].isWall = 0;
//					}
				}
			}else if(x%2 == 0 && y%2 == 0){
				//All walls
				Draw_Cell(x-1, y-1, WALLCOLOR); //Black

				if(x > 0 && y > 0)
					WALLS[x-1][y-1].isWall = 1;

			}else if(x%2 == 0 && y%2 == 1){
				//might be wall
				if(MAZE[x/2+1][y/2+1].left){
					Draw_Cell(x-1, y-1, WALLCOLOR); //black
					if(x > 0 && y > 0)
						WALLS[x-1][y-1].isWall = 1;
				}
				else {
					Draw_Cell(x-1, y-1, BACKCOLOR);
					if(x > 0 && y > 0)
						WALLS[x-1][y-1].isWall = 0;
				}

			}else if(x%2 == 1 && y%2 == 0){
				//might be wall
				if(MAZE[x/2+1][y/2+1].up){
					Draw_Cell(x-1, y-1, WALLCOLOR); //black
					if(x > 0 && y > 0)
						WALLS[x-1][y-1].isWall = 1;
				} else{
					Draw_Cell(x-1, y-1, BACKCOLOR);
					if(x > 0 && y > 0)
						WALLS[x-1][y-1].isWall = 0;
				}
			}
		}
	}

    return;
}

