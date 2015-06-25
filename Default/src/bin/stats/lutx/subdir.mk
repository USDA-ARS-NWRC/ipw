################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/bin/stats/lutx/lutx.c \
../src/bin/stats/lutx/lutximg.c \
../src/bin/stats/lutx/main.c 

OBJS += \
./src/bin/stats/lutx/lutx.o \
./src/bin/stats/lutx/lutximg.o \
./src/bin/stats/lutx/main.o 

C_DEPS += \
./src/bin/stats/lutx/lutx.d \
./src/bin/stats/lutx/lutximg.d \
./src/bin/stats/lutx/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/stats/lutx/%.o: ../src/bin/stats/lutx/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


