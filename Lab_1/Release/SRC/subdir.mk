################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SRC/ADC.c \
../SRC/USART.c \
../SRC/main.c \
../SRC/squareWaveGenerator.c \
../SRC/timer.c \
../SRC/timestamp.c 

OBJS += \
./SRC/ADC.o \
./SRC/USART.o \
./SRC/main.o \
./SRC/squareWaveGenerator.o \
./SRC/timer.o \
./SRC/timestamp.o 

C_DEPS += \
./SRC/ADC.d \
./SRC/USART.d \
./SRC/main.d \
./SRC/squareWaveGenerator.d \
./SRC/timer.d \
./SRC/timestamp.d 


# Each subdirectory must supply rules for building sources it contributes
SRC/%.o: ../SRC/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"C:\Users\thagen\workspace\RBELib\include" -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega644p -DF_CPU=18432000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


