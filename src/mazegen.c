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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "mazegen.h"

cell MAZE[XSIZE][YSIZE];

long numin=1;     //Number of cells in the maze.


//int Build_Maze()
//{
//	srandom((unsigned int)time(NULL)); //seed random number generator with system time
//	initialize(MAZE);      //initialize the maze
//	generate(MAZE);        //generate the maze
//	return 0;
//}

void initialize(){
	//Initialize the maze!
	for(int x=0;x<XSIZE;x++){
		for(int y=0;y<YSIZE;y++){
			//The maze cells on the edges of the maze are "in" to provide padding. Otherwise, all maze cells are not in.
			MAZE[x][y].in   = (x==0||x==XSIZE-1||y==0||y==YSIZE-1)?1:0;
			//All maze cells have all walls existing by default, except the perimeter cells.
			MAZE[x][y].up   = (x==0||x==XSIZE-1||y==0)?0:1;
			MAZE[x][y].left = (x==0||y==0||y==YSIZE-1)?0:1;
		}
	}
	return;
}

void generate(){
	int xcur=1, ycur=1;//start growing from the corner. It could theoretically start growing from anywhere, doesn't matter.
	MAZE[xcur][ycur].in = 1;
	int whichway;
	bool success;
	do{
#ifdef movie
		savebmp(xcur,ycur);
#endif
#ifdef nodeadend
		if( MAZE[xcur][ycur-1].in&&MAZE[xcur][ycur+1].in&&
			   MAZE[xcur-1][ycur].in&&MAZE[xcur+1][ycur].in ){
				   //If at a dead end, randomly destroy a wall to make it not a dead end!
			do{
				success=0;
				whichway=rand()%4;
				switch(whichway){
				case UP:
					if(MAZE[xcur][ycur].up&&ycur!=1){
						success=1;
						MAZE[xcur][ycur].up=0;
					}
					break;
				case DOWN:
					if(MAZE[xcur][ycur+1].up&&ycur!=YSIZE-2){
						success=1;
						MAZE[xcur][ycur+1].up=0;
					}
					break;
				case LEFT:
					if(MAZE[xcur][ycur].left&&xcur!=1){
						success=1;
						MAZE[xcur][ycur].left=0;
					}
					break;
				case RIGHT:
					if(MAZE[xcur+1][ycur].left&&xcur!=XSIZE-2){
						success=1;
						MAZE[xcur+1][ycur].left=0;
					}
					break;
				}
			}while(!success);
		}
#endif
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
#ifdef movie
	savebmp(xcur,ycur);
#endif
	return;
}

void savebmp(int xspecial, int yspecial){
//	//save a bitmap file! the xspecial, yspecial pixel is coloured red.
//	FILE * outfile;
	int extrabytes, paddedsize;
	int x, y, n;
	int width=(XSIZE-1)*2-1;
	int height=(YSIZE-1)*2-1;
//
	extrabytes = (4 - ((width * 3) % 4))%4;
//
//	char filename[200];
//
//	sprintf(filename, "%s_%dx%d_n%ld.jpg", OUTFILE, XSIZE, YSIZE, numin);
	paddedsize = ((width * 3) + extrabytes) * height;
//
	unsigned int headers[13] = {paddedsize + 54, 0, 54, 40, width, height, 0, 0, paddedsize, 0, 0, 0, 0};
//
//	outfile = fopen(filename, "wb");
//	fprintf(outfile, "BM");

//	for (n = 0; n <= 5; n++){
//	   fprintf(outfile, "%c", headers[n] & 0x000000FF);
//	   fprintf(outfile, "%c", (headers[n] & 0x0000FF00) >> 8);
//	   fprintf(outfile, "%c", (headers[n] & 0x00FF0000) >> 16);
//	   fprintf(outfile, "%c", (headers[n] & (unsigned int) 0xFF000000) >> 24);
//	}

//	fprintf(outfile, "%c", 1);fprintf(outfile, "%c", 0);
//	fprintf(outfile, "%c", 24);fprintf(outfile, "%c", 0);

//	for (n = 7; n <= 12; n++){
//	   fprintf(outfile, "%c", headers[n] & 0x000000FF);
//	   fprintf(outfile, "%c", (headers[n] & 0x0000FF00) >> 8);
//	   fprintf(outfile, "%c", (headers[n] & 0x00FF0000) >> 16);
//	   fprintf(outfile, "%c", (headers[n] & (unsigned int) 0xFF000000) >> 24);
//	}

	//Actual writing of data begins here:
	for(y = 0; y <= height - 1; y++){
		for(x = 0; x <= width - 1; x++){
			if(x%2 == 1 && y%2 == 1){
				if(x/2+1 == xspecial && y/2+1 == yspecial) RED;
				else{
					if(MAZE[x/2+1][y/2+1].in) Draw_Cell(x, y, WHITE); else Draw_Cell(x, y, BLACK);
				}
			}else if(x%2 == 0 && y%2 == 0){
				Draw_Cell(x, y, BLACK);
			}else if(x%2 == 0 && y%2 == 1){
				if(MAZE[x/2+1][y/2+1].left) Draw_Cell(x, y, BLACK); else Draw_Cell(x, y, WHITE);
			}else if(x%2 == 1 && y%2 == 0){
				if(MAZE[x/2+1][y/2+1].up) Draw_Cell(x, y, BLACK); else Draw_Cell(x, y, WHITE);
			}
		}
//		if (extrabytes){     // See above - BMP lines must be of lengths divisible by 4.
//			for (n = 1; n <= extrabytes; n++){
//				fprintf(outfile, "%c", 0);
//			}
//		}
	}
//	printf("file printed: %s\n", filename);
//	fclose(outfile);
	return;
}

