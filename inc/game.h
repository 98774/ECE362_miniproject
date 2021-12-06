#ifndef __GAME_H_
#define __GAME_H_
void Move(int dir, int playerX, int playerY);
void init_buttons();
void Start_Game();
void EXTI4_15_IRQHandler(void);

#endif
