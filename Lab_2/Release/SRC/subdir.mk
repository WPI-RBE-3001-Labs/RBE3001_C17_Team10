################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SRC/ADC.c \
../SRC/DAC.c \
../SRC/PID.c \
../SRC/Ports.c \
../SRC/SPI.c \
../SRC/USART.c \
../SRC/main.c 

OBJS += \
./SRC/ADC.o \
./SRC/DAC.o \
./SRC/PID.o \
./SRC/Ports.o \
./SRC/SPI.o \
./SRC/USART.o \
./SRC/main.o 

C_DEPS += \
./SRC/ADC.d \
./SRC/DAC.d \
./SRC/PID.d \
./SRC/Ports.d \
./SRC/SPI.d \
./SRC/USART.d \
./SRC/main.d 


# Each subdirectory must supply rules for building sources it contributes
SRC/%.o: ../SRC/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"R:\3001\rbelib-17.1.0\RBELib\include" -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega644p -DF_CPU=18432000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


