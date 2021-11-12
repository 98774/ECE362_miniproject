#include "spi.h"
#include "STM32f0xx.h"


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

