#include "spi.h"
#include "STM32f0xx.h"


/*******************************************************************************
Author: Jonathon Snyder
Date: 11/11/2021
Description: waits for n nanoseconds
*******************************************************************************/
void nano_wait(unsigned int n) {
    asm(    "        mov r0,%0\n"
            "repeat: sub r0,#83\n"
            "        bgt repeat\n" : : "r"(n) : "r0", "cc");
}



/*******************************************************************************
Author: Agrim Bharat
Date: 11/11/2021
Description: Initialize SPI2 (For reference, need to make changes to the configuration for our use case)
Pins : PB12 (NSS), PB13 (SCK), PB14(MISO), PB15(MOSI) for SPI2
Pins : PA15 (NSS), PB1 (SCK), PB2(MISO), PB3(MOSI) for SPI1

*******************************************************************************/

void init_spi2(void) {
    GPIOB -> MODER &= ~0xcf000000;
    GPIOB -> MODER |= 0x8a000000;

    GPIOB -> AFR[1] &= ~0XF0FF0000;


    RCC -> APB1ENR |= RCC_APB1ENR_SPI2EN;
    SPI2 -> CR1 &= ~SPI_CR1_SPE;
    SPI2 -> CR1 |= SPI_CR1_BR_2 | SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_MSTR;
    SPI2 -> CR2 |= SPI_CR2_DS_3 | SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0 | SPI_CR2_SSOE | SPI_CR2_NSSP;
    SPI2 -> CR2 |= SPI_CR2_TXDMAEN;
    SPI2 -> CR1 |= SPI_CR1_SPE;

}


//==============================================================================
//OLED SPI FUNCTIONS
//==============================================================================

/*******************************************************************************
Author: Jonathon Snyder
Date: 11/11/2021
Description: initializes SPI1 for use with the OLED peripheral
*******************************************************************************/
void setup_spi1() {
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; //enable in rcc

	//set pins to 10 in MODER
	GPIOA->MODER &= ~(GPIO_MODER_MODER5 | GPIO_MODER_MODER7 | GPIO_MODER_MODER15); //set top to 00xx0000
	GPIOA->MODER |= GPIO_MODER_MODER5_1 | GPIO_MODER_MODER7_1 | GPIO_MODER_MODER15_1; //set 15, 5,6,7 to alternate functions
	GPIOA->AFR[1] &= ~GPIO_AFRH_AFR15; //clear 15
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFR5 | GPIO_AFRL_AFR7); //clear 5,6,7
	//AF0 is desired so leave bits cleared

	SPI1->CR1 &= ~(SPI_CR1_SPE); //clear enable bit

	//SPI1->CR1 |= SPI_CR1_BIDIMODE | SPI_CR1_BIDIOE; //set bidirectional output
	SPI1->CR1 = SPI_CR1_BR; //set BR to 7; Potentially causing the flickering
	//configure datasize to 10, ss output enable, nss pulse
	SPI1->CR2 = SPI_CR2_DS_3 | SPI_CR2_DS_0 | SPI_CR2_SSOE | SPI_CR2_NSSP;
	SPI1->CR1 |= SPI_CR1_MSTR;

	//SPI1->CR2 |= SPI_CR2_TXDMAEN; //generate DMA request when TX is empty

	SPI1->CR1 |= SPI_CR1_SPE; //enable spi1
}


/*******************************************************************************
Author: Jonathon Snyder
Date: 11/11/2021
Description: writes a 10 bit comand to the OLED
*******************************************************************************/
void spi_cmd(unsigned int data) {
	while(!(SPI1->SR & SPI_SR_TXE)); //wait until TXE bit is set
	SPI1->DR = data;
}


/*******************************************************************************
Author: Jonathon Snyder
Date: 11/11/2021
Description: writes an 8 bit value orred with 0x200
*******************************************************************************/
void spi_data(unsigned int data) {
	while(!(SPI1->SR & SPI_SR_TXE)); //wait until TXE bit is set
	SPI1->DR = (0x200 | data);

}

/*******************************************************************************
Author: Jonathon Snyder
Date: 11/11/2021
Description: runs the OLED initialization sequence
*******************************************************************************/
void spi_init_oled() {
	nano_wait(1000000); //wait 1ms for powerup
	spi_cmd(0x38); //setup for 8 bit operation
	spi_cmd(0x08); //turn display off
	spi_cmd(0x01); //clear display
	nano_wait(2000000); //wait 2 ms for display clear

	spi_cmd(0x06); //set display to scroll
	spi_cmd(0x02); //move cursor to home position
	spi_cmd(0x0c); //turn display on

	/*
	//initialize oled_message
	for(int i = 0; i < 34; i++)
		oled_msg[i] = 0x200 | ' '; //ASCII space

	//update command entries
	oled_msg[0] = 0x0002;
	oled_msg[17] = 0x00c0;
	*/
}

//displays the first row on the screen
void spi_display1(const char *string) {
	spi_cmd(0x02); //moves the cursor to the home positon
	int i = 0; //index of string
	while(string[i]){
		spi_data(string[i]); //write the string
		i++;
	}
}

void spi_display2(const char *string) {
	spi_cmd(0xc0); //moves the cursor to the lower row
	int i = 0; //index of string
	while(string[i]){
		spi_data(string[i]); //write the string
		i++;
	}
}
