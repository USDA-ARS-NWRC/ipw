################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/pixcalc/imgsum/headers.c \
../src/bin/pixcalc/imgsum/imgsum.c \
../src/bin/pixcalc/imgsum/main.c 

OBJS += \
./src/bin/pixcalc/imgsum/headers.o \
./src/bin/pixcalc/imgsum/imgsum.o \
./src/bin/pixcalc/imgsum/main.o 

C_DEPS += \
./src/bin/pixcalc/imgsum/headers.d \
./src/bin/pixcalc/imgsum/imgsum.d \
./src/bin/pixcalc/imgsum/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/pixcalc/imgsum/%.o: ../src/bin/pixcalc/imgsum/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


