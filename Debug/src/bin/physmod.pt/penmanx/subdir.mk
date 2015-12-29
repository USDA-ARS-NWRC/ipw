################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/physmod.pt/penmanx/main.o \
../src/bin/physmod.pt/penmanx/pm.o 

C_SRCS += \
../src/bin/physmod.pt/penmanx/main.c \
../src/bin/physmod.pt/penmanx/pm.c 

OBJS += \
./src/bin/physmod.pt/penmanx/main.o \
./src/bin/physmod.pt/penmanx/pm.o 

C_DEPS += \
./src/bin/physmod.pt/penmanx/main.d \
./src/bin/physmod.pt/penmanx/pm.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/physmod.pt/penmanx/%.o: ../src/bin/physmod.pt/penmanx/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


