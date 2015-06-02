################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/classify/declass/declass.c \
../src/bin/classify/declass/headers.c \
../src/bin/classify/declass/main.c \
../src/bin/classify/declass/newlqh.c 

OBJS += \
./src/bin/classify/declass/declass.o \
./src/bin/classify/declass/headers.o \
./src/bin/classify/declass/main.o \
./src/bin/classify/declass/newlqh.o 

C_DEPS += \
./src/bin/classify/declass/declass.d \
./src/bin/classify/declass/headers.d \
./src/bin/classify/declass/main.d \
./src/bin/classify/declass/newlqh.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/classify/declass/%.o: ../src/bin/classify/declass/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


