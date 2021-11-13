#include "STM32f0xx.h"
	//Setup DMA for SPI2 (OLED)
void setup_dma_ch3(uint16_t oled_msg[]);
void enable_dma_ch3(void);

void setup_dma_dac(uint16_t array[], int count);
