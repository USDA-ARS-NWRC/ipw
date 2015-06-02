################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/basic/primg/getcoords.c \
../src/bin/basic/primg/main.c \
../src/bin/basic/primg/primg.c 

OBJS += \
./src/bin/basic/primg/getcoords.o \
./src/bin/basic/primg/main.o \
./src/bin/basic/primg/primg.o 

C_DEPS += \
./src/bin/basic/primg/getcoords.d \
./src/bin/basic/primg/main.d \
./src/bin/basic/primg/primg.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/basic/primg/%.o: ../src/bin/basic/primg/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


