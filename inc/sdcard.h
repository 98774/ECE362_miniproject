#ifndef __SDCARD_H__
#define __SDCARD_H__

#include "stm32f0xx.h"

//commands
//void print_error(FRESULT fr, const char *msg);
//void mount(FATFS *fs_storage);

//sdcard functions
void enable_sdcard();
void disable_sdcard();
void init_spi2_slow();
void sdcard_io_high_speed();
void init_sdcard_io();

//helper functions
uint32_t Get_4(uint8_t *byteAddr, char type, int print);
uint16_t Get_2(uint8_t *byteAddr, char type, int print);
void Print_Header(uint8_t *header);

//control functions
void Setup_TIM15(int byteRate);
void Configure_DMA1CH3(uint8_t *data, int size);
void Swap_Bytes(uint16_t *data);
void Swap_Array(uint8_t *data);
void Init_Play_Devices();
uint16_t play(char *fileName,  uint8_t *header, uint8_t *data, uint8_t *data2, FIL *fp, UINT *br);
int Update_Array(int currArr, uint8_t *data, uint8_t *data2, FIL *fp, UINT *br, uint16_t sampSize, FIL fstart);


#endif
