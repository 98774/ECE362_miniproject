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

#include <stdio.h>
#include <string.h>

#include "ff.h"
#include "sdcard.h"

#define BUFFSIZE 4000

//sets pb12 to 0 to enable sd card
void enable_sdcard(){
    GPIOB->BSRR |= GPIO_BSRR_BR_12;
}

//sets pb12 to 1 to disable sd card
void disable_sdcard(){
    GPIOB->BSRR |= GPIO_BSRR_BS_12;
}

void init_spi2_slow(){
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;

    //configures for alternate function and pb2 for output
    GPIOB->MODER |= GPIO_MODER_MODER13_1 |
            GPIO_MODER_MODER14_1 |
            GPIO_MODER_MODER15_1;

    //set af0 for pb 3 4 5
    GPIOB->AFR[1] &= ~(GPIO_AFRH_AFR13 |
            GPIO_AFRH_AFR14 |
            GPIO_AFRH_AFR15);


    //set baud rate as low as possible, configure as master
    SPI2->CR1 |= SPI_CR1_BR | SPI_CR1_MSTR;
    SPI2->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;

    //data size goes to 8 by default but ill clear it anyways
    //set  to invalid value so it goes to 8 bits
    SPI2->CR2 &= ~SPI_CR2_DS;
    SPI2->CR2 |= SPI_CR2_FRXTH; //set FIFO reception threshold

    SPI2->CR1 |= SPI_CR1_SPE; //Enable SPI

}

void sdcard_io_high_speed(){
    SPI2->CR1 &= ~SPI_CR1_SPE; //disable clock

    SPI2->CR1 &= ~SPI_CR1_BR; //reset baud rate bits
    SPI2->CR1 |= SPI_CR1_BR_0; //sets divisor to 4 to get 48 / 4 = 12 MHz clock

    SPI2->CR1 |= SPI_CR1_SPE; //reenable clock
}

void init_sdcard_io(){
    init_spi2_slow();
    GPIOB->MODER &= ~GPIO_MODER_MODER12;
    GPIOB->MODER |= GPIO_MODER_MODER12_0;
    disable_sdcard();
}


uint32_t Get_4(uint8_t *byteAddr, char type, int print){
    uint32_t new = 0;

        new |= byteAddr[0];
        new |= byteAddr[1] << 8;
        new |= byteAddr[2] << 16;
        new |= byteAddr[3] << 24;
    if(print){
        if(type == 'n'){
            printf("%lu\n", new);
        } else {
            printf("%c%c%c%c\n", byteAddr[0], byteAddr[1], byteAddr[2], byteAddr[3]);
        }
    }
    return new;
}

uint16_t Get_2(uint8_t *byteAddr, char type, int print){
    uint16_t new = 0;

        new |= byteAddr[0];
        new |= byteAddr[1] << 8;

    if(print){
        if(type == 'n'){
            printf("%lu\n", new);

        } else {
            printf("%c%c\n", byteAddr[0], byteAddr[1]);
        }
    }
    return new;
}

void Print_Header(uint8_t *header)
{
//Header fields
    printf("Header:\n");
    Get_4(&header[0], 'w', 1); //chunk id
    Get_4(&header[4], 'n', 1); //the number of bytes in the file
    Get_4(&header[8], 'w', 1); //riff type id

    printf("\nChunk 1 ID: ");
    Get_4(&header[12], 'w', 1); //chunk1 id

    printf("Chunk 1 data size:     ");
    Get_4(&header[16], 'n', 1);

    printf("Format Tag:            ");
    Get_2(&header[20], 'n', 1);

    printf("Num Channels:          ");
    Get_2(&header[22], 'n', 1);

    printf("Sample Rate:           ");
    Get_4(&header[24], 'n', 1);

    printf("Byte rate:             ");
    Get_4(&header[28], 'n', 1);

    printf("Block Align:           ");
    Get_2(&header[32], 'n', 1);

    printf("Bits per sample:       ");
    Get_2(&header[34], 'n', 1);

    printf("\nChunk 2 ID:            ");
    Get_4(&header[36], 'w', 1);

    printf("Chunk 2 data size:     ");
    Get_4(&header[40], 'n', 1);

}


void Setup_TIM15(int byteRate){
    TIM15->CR1 &= ~TIM_CR1_CEN;
    TIM15->CR1 |= TIM_CR2_MMS_1;
    TIM15->PSC = 0;
    TIM15->ARR = (48000000 / byteRate) - 1; //set to the sampling rate of the file
    TIM15->CR1 |= TIM_CR1_CEN;
    printf("SAMP RATE SET: %lu\n", 48000000 / (TIM15->ARR + 1) / (TIM15->PSC + 1));
}

void Configure_DMA1CH3(uint8_t *data, int size){
    DMA1_Channel3->CCR &= ~DMA_CCR_EN;

    DMA1_Channel3->CMAR = (uint32_t) data;

    if(size == 16){
        DMA1_Channel3->CPAR = (uint32_t) &(DAC->DHR12LD);
        DMA1_Channel3->CNDTR = BUFFSIZE / 2; //two bytes per sample
        DMA1_Channel3->CCR |= DMA_CCR_MSIZE_0; //set memory size to 16
        DMA1_Channel3->CCR |= DMA_CCR_PSIZE_0; //set memory size to 16

    }else{

        DMA1_Channel3->CPAR = (uint32_t) &(DAC->DHR8RD);
        DMA1_Channel3->CNDTR = BUFFSIZE;
        DMA1_Channel3->CCR &= ~DMA_CCR_MSIZE; //set memory size to 8
        DMA1_Channel3->CCR &= ~DMA_CCR_PSIZE; //set peripheral size to 8

    }
    DMA1_Channel3->CCR |= DMA_CCR_EN;
}
void Swap_Bytes(uint16_t *data){
    for(int i = 0; i < BUFFSIZE / 2; i++){
        data[i] = data[i] + 0x8000;
    }
}

void Swap_Array(uint8_t *data){
    DMA1_Channel3->CCR &= ~DMA_CCR_EN;
    DMA1_Channel3->CMAR = (uint32_t) data;
    DMA1_Channel3->CCR |= DMA_CCR_EN;
}

void Init_Play_Devices(uint8_t *data){
   RCC->APB2ENR |= RCC_APB2ENR_TIM15EN; //enable RCC clock to tim 7
   RCC->APB1ENR |= RCC_APB1ENR_DACEN; //enable clock to dac
   RCC->AHBENR |= RCC_AHBENR_DMA1EN; //enable clock to dma
   RCC->AHBENR |= RCC_AHBENR_GPIOAEN; //enable GPIO A
   GPIOA->MODER |= GPIO_MODER_MODER4; //Set PA4 to analog

   //Configure DAC channel 1 to be triggered by TIM15 TRGO
   DAC->CR |= DAC_CR_TSEL1_1 |DAC_CR_TSEL1_0;//Set up triggering on timer 15 0x011
   DAC->CR |= DAC_CR_TEN1; //enable trigger

   //Configure DMA to transfer 512 bytes from data to DAC
   DMA1_Channel3->CCR &= ~DMA_CCR_EN; //disable dma to configure
   DMA1_Channel3->CCR &= ~DMA_CCR_MSIZE;
   DMA1_Channel3->CCR &= ~DMA_CCR_PSIZE;
   DMA1_Channel3->CCR |= DMA_CCR_DIR  | //set mem to peripheral
                         DMA_CCR_MINC | //set memory address increment
                         DMA_CCR_CIRC |
                         DMA_CCR_TCIE;

   DMA1_Channel3->CNDTR = BUFFSIZE;
   DMA1_Channel3->CMAR = (uint32_t) data;
   DMA1_Channel3->CPAR = (uint32_t) &(DAC->DHR8R1);

   //Enable DMA channel3
   DMA1_Channel3->CCR |= DMA_CCR_EN;

   //Enable DAC channel1 and DMA connection
   DAC->CR |= DAC_CR_DMAEN1;
   //DAC->CR |= DAC_CR_DMAUDRIE1; //enable dma underrun innterrupt
   DAC->CR |= DAC_CR_EN1; //enable channel 1

   //Enable devices
   //Step 3: Configure timer
   TIM15->PSC = 0;
   TIM15->CR2 &= ~TIM_CR2_MMS;
   TIM15->CR2 |=  TIM_CR2_MMS_1; //enable update trigger on timer edge
}

uint16_t play(char *fileName,  uint8_t *header, uint8_t *data, uint8_t *data2, FIL *fp, UINT *br){
    //argv is array that contains string filename
    FRESULT fr; //return code of f_open


    fr = f_open(fp, fileName, FA_READ|FA_OPEN_EXISTING);
    if (fr) {
        print_error(fr, fileName);
        return 0;
    }

    //read in header information exit if errror
    fr = f_read(fp, header, 44, br);
    if (fr){
        print_error(fr, fileName);
        return 0;
    }

    //this is where we parse the header
    Print_Header(header);

    uint32_t sampRate = Get_4(&header[24], 'n', 0);
    uint16_t sampSize = Get_2(&header[34], 'n', 0);
    uint32_t waveDataLength = Get_4(&header[40], 'n', 0); //number of bytes to read

    //set up configuration
    Configure_DMA1CH3(data, sampSize);
    Setup_TIM15(sampRate); //set transfer frequency based on samples and start transfer

    return sampSize;
}

int Update_Array(int currArr, uint8_t *data, uint8_t *data2, FIL *fp, UINT *br, uint16_t sampSize, FIL fstart){
    DMA1->IFCR |= DMA_IFCR_CTCIF3;
    if(currArr == 1){
        currArr = 2;
        Swap_Array(data); //write data to dac
        f_read(fp, data2, BUFFSIZE, br);

        //byte order is swapped for use in 16bit sample
        if(sampSize == 16)
            Swap_Bytes(data2);

        if(*br != BUFFSIZE)
           *fp = fstart;
    } else {
        currArr = 1;
        Swap_Array(data2); //write data2 to dac
        f_read(fp, data, BUFFSIZE, br);

        //byte order is swapped for use in 16bit sample
        if(sampSize == 16)
            Swap_Bytes(data);

        if(*br != BUFFSIZE)
           *fp = fstart;
    }

    return currArr;
}
