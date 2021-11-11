/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f0xx.h"
			

//========================================================
//STEP 2.4
//========================================================

#define STEP24
#if defined(STEP24)
#include <stdio.h>

#include "fifo.h"
#include "tty.h"
#define FIFOSIZE 16
char serfifo[FIFOSIZE];
int seroffset = 0;

int __io_putchar(int c){
	//if a new line is passed, insert a carriage return, then new line
	if(c == '\n')
	{
		while(!(USART5->ISR & USART_ISR_TXE));
		USART5->TDR = '\r';
	}

	while(!(USART5->ISR & USART_ISR_TXE));
	USART5->TDR = c;
	return c;
}

int __io_getchar(void) {
	return interrupt_getchar();
}

void enable_tty_interrupt(){
	USART5->CR1 |= USART_CR1_RXNEIE; //enable interrupt when RX not empty
	USART5->CR3 |= USART_CR3_DMAR; //enable dma on receiver
	NVIC->ISER[0] |= 1 << USART3_8_IRQn; // enable interrupt in table

	//DMA Configuration
	RCC->AHBENR |= RCC_AHBENR_DMA2EN;
	DMA2->RMPCR |= DMA_RMPCR2_CH2_USART5_RX;
	DMA2_Channel2->CCR &= ~DMA_CCR_EN; //disable DMA to configure

	DMA2_Channel2->CMAR = (uint16_t) serfifo; //set location in memory
	DMA2_Channel2->CPAR = (uint16_t) &USART5->RDR; //set peripheral location
	DMA2_Channel2->CNDTR = FIFOSIZE; //set num data
	DMA2_Channel2->CCR &= ~DMA_CCR_DIR; //read from peripheral
	DMA2_Channel2->CCR &= ~(DMA_CCR_TCIE | DMA_CCR_HTIE); //disable interrupts
	DMA2_Channel2->CCR &= ~DMA_CCR_MSIZE; //set memory size to 8 bits
	DMA2_Channel2->CCR &= ~DMA_CCR_PSIZE; //set peripheral size to 8 bits
	DMA2_Channel2->CCR |= DMA_CCR_MINC; //increment memory address
	DMA2_Channel2->CCR &= ~DMA_CCR_PINC; //dont increment peripheral address
	DMA2_Channel2->CCR |= DMA_CCR_CIRC; //set circular mode
	DMA2_Channel2->CCR &= ~DMA_CCR_MEM2MEM; //disable mem2mem

	//reenable DMA
	DMA2_Channel2->CCR |= DMA_CCR_EN;
}

int interrupt_getchar(){
    // Wait for a newline to complete the buffer.
    while(fifo_newline(&input_fifo) == 0) {
    	asm volatile ("wfi"); //wait for an interrupt
    }
    // Return a character from the line buffer.
    char ch = fifo_remove(&input_fifo);
    return ch;
}

void USART3_4_5_6_7_8_IRQHandler() {
	while(DMA2_Channel2->CNDTR != sizeof(serfifo - seroffset)){
		if(!fifo_full(&input_fifo))
			insert_echo_char(serfifo[seroffset]);
		seroffset = (seroffset + 1) % sizeof(serfifo);
	}
}

int main() {

	for(;;);
}
#endif
