################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/classify/classeq/classeq.c \
../src/bin/classify/classeq/headers.c \
../src/bin/classify/classeq/hist.c \
../src/bin/classify/classeq/histx.c \
../src/bin/classify/classeq/lutximg.c \
../src/bin/classify/classeq/main.c \
../src/bin/classify/classeq/mean.c \
../src/bin/classify/classeq/median.c \
../src/bin/classify/classeq/newcrh.c 

OBJS += \
./src/bin/classify/classeq/classeq.o \
./src/bin/classify/classeq/headers.o \
./src/bin/classify/classeq/hist.o \
./src/bin/classify/classeq/histx.o \
./src/bin/classify/classeq/lutximg.o \
./src/bin/classify/classeq/main.o \
./src/bin/classify/classeq/mean.o \
./src/bin/classify/classeq/median.o \
./src/bin/classify/classeq/newcrh.o 

C_DEPS += \
./src/bin/classify/classeq/classeq.d \
./src/bin/classify/classeq/headers.d \
./src/bin/classify/classeq/hist.d \
./src/bin/classify/classeq/histx.d \
./src/bin/classify/classeq/lutximg.d \
./src/bin/classify/classeq/main.d \
./src/bin/classify/classeq/mean.d \
./src/bin/classify/classeq/median.d \
./src/bin/classify/classeq/newcrh.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/classify/classeq/%.o: ../src/bin/classify/classeq/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


