#include "dma.h"
#include "STM32f0xx.h"

//===========================================================================
//DMA memory map for OLED display
//===========================================================================
void setup_dma_ch3(uint16_t oled_msg[]) {
	DMA1_Channel3->CCR &= ~DMA_CCR_EN;
	DMA1_Channel3->CPAR = (uint32_t) &SPI1->DR;
	DMA1_Channel3->CMAR = (uint16_t) oled_msg;
	DMA1_Channel3->CNDTR = 34;
	DMA1_Channel3->CCR |= DMA_CCR_DIR;
	DMA1_Channel3->CCR |= DMA_CCR_MINC;
	DMA1_Channel3->CCR &= ~DMA_CCR_MSIZE; //clear mem size
	DMA1_Channel3->CCR |= 0x01 << 10; //set msize to 16 (bits 10 and 11 to 01)
	DMA1_Channel3->CCR &= ~DMA_CCR_PSIZE; //clear peripheral size
	DMA1_Channel3->CCR |= DMA_CCR_PSIZE_1; //set psize to 32
	DMA1_Channel3->CCR |= DMA_CCR_CIRC; //set circular operation
}

void enable_dma_ch3(void) {
	DMA1_Channel3->CCR |= DMA_CCR_EN;
}
