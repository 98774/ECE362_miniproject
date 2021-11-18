#include "STM32f0xx.h"

void nano_wait(unsigned int n);

//SPI functions for LED Matrix
void init_spi2(void);

//SPI functions for OLED
void setup_spi1();
void spi_cmd(unsigned int data);
void spi_data(unsigned int data);
void spi_init_oled();
void spi_display1(const char *string);
void spi_display2(const char *string);
