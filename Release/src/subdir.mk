################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/dac.c \
../src/dma.c \
../src/game.c \
../src/main.c \
../src/spi.c \
../src/syscalls.c \
../src/system_stm32f0xx.c \
../src/timer.c 

OBJS += \
./src/dac.o \
./src/dma.o \
./src/game.o \
./src/main.o \
./src/spi.o \
./src/syscalls.o \
./src/system_stm32f0xx.o \
./src/timer.o 

C_DEPS += \
./src/dac.d \
./src/dma.d \
./src/game.d \
./src/main.d \
./src/spi.d \
./src/syscalls.d \
./src/system_stm32f0xx.d \
./src/timer.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F0 -DSTM32F091RCTx -DSTM32F091 -DUSE_STDPERIPH_DRIVER -I"C:/Users/jsnyd/OneDrive - purdue.edu/School/ECE 36200/Project/StdPeriph_Driver/inc" -I"C:/Users/jsnyd/OneDrive - purdue.edu/School/ECE 36200/Project/inc" -I"C:/Users/jsnyd/OneDrive - purdue.edu/School/ECE 36200/Project/CMSIS/device" -I"C:/Users/jsnyd/OneDrive - purdue.edu/School/ECE 36200/Project/CMSIS/core" -O3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


