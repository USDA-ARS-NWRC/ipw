################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/lq/requant/lqhprint.c \
../src/bin/lq/requant/lqhremap.c \
../src/bin/lq/requant/main.c \
../src/bin/lq/requant/requant.c 

OBJS += \
./src/bin/lq/requant/lqhprint.o \
./src/bin/lq/requant/lqhremap.o \
./src/bin/lq/requant/main.o \
./src/bin/lq/requant/requant.o 

C_DEPS += \
./src/bin/lq/requant/lqhprint.d \
./src/bin/lq/requant/lqhremap.d \
./src/bin/lq/requant/main.d \
./src/bin/lq/requant/requant.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/lq/requant/%.o: ../src/bin/lq/requant/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


