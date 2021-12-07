#ifndef __GAME_H_
#define __GAME_H_
void Move(int dir, int playerX, int playerY);
void init_buttons();
void Start_Game();
void EXTI4_15_IRQHandler(void);
int move_up(int xCurr, int yCurr);
int move_down(int xCurr, int yCurr);
int move_left(int xCurr, int yCurr);
int move_right(int xCurr, int yCurr);
void Set_Goal(int *goalX, int *goalY);

#endif
