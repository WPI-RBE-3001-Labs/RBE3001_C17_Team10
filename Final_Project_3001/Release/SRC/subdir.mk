################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SRC/ADC.c \
../SRC/DAC.c \
../SRC/PID.c \
../SRC/Periph.c \
../SRC/Ports.c \
../SRC/SPI.c \
../SRC/USART.c \
../SRC/forwardKinematics.c \
../SRC/main.c \
../SRC/motorCurrentSensor.c \
../SRC/motors.c \
../SRC/timer.c 

OBJS += \
./SRC/ADC.o \
./SRC/DAC.o \
./SRC/PID.o \
./SRC/Periph.o \
./SRC/Ports.o \
./SRC/SPI.o \
./SRC/USART.o \
./SRC/forwardKinematics.o \
./SRC/main.o \
./SRC/motorCurrentSensor.o \
./SRC/motors.o \
./SRC/timer.o 

C_DEPS += \
./SRC/ADC.d \
./SRC/DAC.d \
./SRC/PID.d \
./SRC/Periph.d \
./SRC/Ports.d \
./SRC/SPI.d \
./SRC/USART.d \
./SRC/forwardKinematics.d \
./SRC/main.d \
./SRC/motorCurrentSensor.d \
./SRC/motors.d \
./SRC/timer.d 


# Each subdirectory must supply rules for building sources it contributes
SRC/%.o: ../SRC/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"C:\Users\thagen\git\RBE3001_C17_Team10\RBELib\RBELib\include" -Wall -Os -fpack-struct -fshort-enums -ffunction-sections -fdata-sections -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega644p -DF_CPU=18432000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


