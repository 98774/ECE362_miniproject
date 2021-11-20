//NOTE: this code is incomplete. It has dependancies from lab 10
//to mess with it, copy the files from the website to your lab 10 and then
//you can run this code as the main file.



#define STEP25
#if defined(STEP25)
#include <stdio.h>

#include "fifo.h"
#include "ff.h"
#include "lcd.h"
#include "tty.h"
#include "commands.h"
#define FIFOSIZE 16
char serfifo[FIFOSIZE];
int seroffset = 0;

int interrupt_getchar(){
    // Wait for a newline to complete the buffer.
    while(fifo_newline(&input_fifo) == 0) {
        asm volatile ("wfi"); //wait for an interrupt
    }
    // Return a character from the line buffer.
    char ch = fifo_remove(&input_fifo);
    return ch;
}

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

	DMA2_Channel2->CMAR = (uint32_t) serfifo; //set location in memory
	DMA2_Channel2->CPAR = (uint32_t) &(USART5->RDR); //set peripheral location
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


void USART3_4_5_6_7_8_IRQHandler() {
    //printf("First Char: %c\n", serfifo[seroffset]);
	while(DMA2_Channel2->CNDTR != sizeof(serfifo) - seroffset){
		if(!fifo_full(&input_fifo))
			insert_echo_char(serfifo[seroffset]);
		seroffset = (seroffset + 1) % sizeof(serfifo);
	}
	//printf("WHILE FINISHED\n");
}

void init_spi1_slow(){
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	//configures for alternate function and pb2 for output
	GPIOB->MODER |=	GPIO_MODER_MODER3_1 |
			GPIO_MODER_MODER4_1 |
			GPIO_MODER_MODER5_1;

	//set af0 for pb 3 4 5
	GPIOB->AFR[0] &= ~(GPIO_AFRL_AFR3 |
			GPIO_AFRL_AFR4 |
			GPIO_AFRL_AFR5);


	//set baud rate as low as possible, configure as master
	SPI1->CR1 |= SPI_CR1_BR | SPI_CR1_MSTR;
	SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;

	//data size goes to 8 by default but ill clear it anyways
	//set  to invalid value so it goes to 8 bits
	SPI1->CR2 &= ~SPI_CR2_DS;
	SPI1->CR2 |= SPI_CR2_FRXTH; //set FIFO reception threshold

	SPI1->CR1 |= SPI_CR1_SPE; //Enable SPI

}

//sets pb2 to 0 to enable sd card
void enable_sdcard(){
	GPIOB->BSRR |= GPIO_BSRR_BR_2;
}

//sets pb2 to 1 to disable sd card
void disable_sdcard(){
	GPIOB->BSRR |= GPIO_BSRR_BS_2;
}

void init_sdcard_io(){
	init_spi1_slow();
	GPIOB->MODER |= GPIO_MODER_MODER2_0;
	disable_sdcard();
}

void sdcard_io_high_speed(){
	SPI1->CR1 &= ~SPI_CR1_SPE; //disable clock

	SPI1->CR1 &= ~SPI_CR1_BR; //reset baud rate bits
	SPI1->CR1 |= SPI_CR1_BR_0; //sets divisor to 4 to get 48 / 4 = 12 MHz clock

	SPI1->CR1 |= SPI_CR1_SPE; //reenable clock
}

void init_lcd_spi(){
	//configure as outputs
	GPIOB->MODER |= GPIO_MODER_MODER8_0 |
			GPIO_MODER_MODER14_0 |
			GPIO_MODER_MODER11_0;

	init_spi1_slow();
	sdcard_io_high_speed();

}

#include "commands.h"

void add(int argc, char *argv[])
{
	int sum = 0;
	for(int i=1; i < argc; i++) {
		sum += strtol(argv[i], 0, 0);
	}
	printf("The sum is %d\n", sum);
}

void mul(int argc, char *argv[])
{
	int prod = 1;
	for(int i=1; i < argc; i++) {
		prod *= strtol(argv[i], 0, 0);
	}
	printf("The product is %d\n", prod);
}

#define BUFFSIZE 50
void play( int argc, char *argv[]){
	//argv is array that contains string filename
	FIL fp; //File object to be returned
	FRESULT fr; //return code of f_open
	fr = f_open(&fp, argv[1], FA_READ|FA_OPEN_EXISTING);
    if (fr) {
        print_error(fr, argv[1]);
        return;
    }
    printf("HELLO?\n");

    char data[BUFFSIZE];
    UINT br = BUFFSIZE; //stores the number of bytes read
    //reads from fi
	DMA1_Channel5->CCR &= ~DMA_CCR_EN; //disable dma to configure
	DMA1_Channel5->CCR &= ~DMA_CCR_MSIZE;
	DMA1_Channel5->CCR &= ~DMA_CCR_PSIZE;
	DMA1_Channel5->CCR |= DMA_CCR_DIR  | //set mem to peripheral
						  DMA_CCR_MINC | //set memory address increment
						  //DMA_CCR_CIRC | //set circular operation
						  //DMA_CCR_PSIZE_0 | //set psize to 16 bits
						  //DMA_CCR_MSIZE_0 |
						  DMA_CCR_TCIE; //Set memory size to 16 bits

	DMA1_Channel5->CNDTR = 1;
	DMA1_Channel5->CMAR = (uint32_t) data;
	DMA1_Channel5->CPAR = (uint32_t) &(DAC->DHR8R1);
	DMA1_Channel5->CCR |= DMA_CCR_EN;

    while(br == BUFFSIZE){
    	fr = f_read(&fp, data, BUFFSIZE, &br);
		if (fr){
			print_error(fr, argv[1]);
			return;
		}
		while(!(DMA1->ISR | DMA_ISR_TCIF5));
	}
	DMA1_Channel5->CCR &= ~DMA_CCR_EN;
	return; //file completely read


}

struct commands_t usercmds[] = {
	 { "add",    add },
	 { "mul",    mul },
	 { "play",   play},
};

int main() {
	//Step 1.1
		RCC->APB2ENR |= RCC_APB2ENR_TIM15EN; //enable RCC clock to tim 7
		RCC->APB1ENR |= RCC_APB1ENR_DACEN; //enable clock to dac
		RCC->AHBENR |= RCC_AHBENR_DMA1EN; //enable clock to dma
		RCC->AHBENR |= RCC_AHBENR_GPIOAEN; //enable GPIO A
		GPIOA->MODER |= GPIO_MODER_MODER4; //Set PA4 to analog

		//Step 3: Configure timer
		TIM15->ARR = 48000000-1; //trigger fre * samples / sec
		TIM15->PSC = 0;
		TIM15->CR2 &= ~TIM_CR2_MMS;
		TIM15->CR2 |=  TIM_CR2_MMS_1; //enable update trigger on timer edge
		TIM15->DIER |= TIM_DIER_UDE; //enable dma requests
		TIM15->CR1 |= TIM_CR1_CEN; //enable timer


		//Step 4: DMA Config


		//DMA1_Channel4->CCR = DMA_RMPCR1_CH4_TIM7_UP;

		//Step 5: DAC Config
		DAC->CR |= DAC_CR_TSEL1_0;
		DAC->CR |= DAC_CR_TSEL1_1; //Set up triggering on timer 15 0x011
		DAC->CR |= DAC_CR_TEN1; //enable trigger
		DAC->CR |= DAC_CR_EN1; //enable channel 1
	init_usart5();
    enable_tty_interrupt();
    setbuf(stdin,0);
    setbuf(stdout,0);
    setbuf(stderr,0);
    command_shell();
}
#endif
