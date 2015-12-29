################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/classify/classify/classify.o \
../src/bin/classify/classify/get_classes.o \
../src/bin/classify/classify/headers.o \
../src/bin/classify/classify/lutximg.o \
../src/bin/classify/classify/main.o 

C_SRCS += \
../src/bin/classify/classify/classify.c \
../src/bin/classify/classify/get_classes.c \
../src/bin/classify/classify/headers.c \
../src/bin/classify/classify/lutximg.c \
../src/bin/classify/classify/main.c 

OBJS += \
./src/bin/classify/classify/classify.o \
./src/bin/classify/classify/get_classes.o \
./src/bin/classify/classify/headers.o \
./src/bin/classify/classify/lutximg.o \
./src/bin/classify/classify/main.o 

C_DEPS += \
./src/bin/classify/classify/classify.d \
./src/bin/classify/classify/get_classes.d \
./src/bin/classify/classify/headers.d \
./src/bin/classify/classify/lutximg.d \
./src/bin/classify/classify/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/classify/classify/%.o: ../src/bin/classify/classify/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


