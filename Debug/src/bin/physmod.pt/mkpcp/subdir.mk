################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../src/bin/physmod.pt/mkpcp/calc_snow.o \
../src/bin/physmod.pt/mkpcp/main.o 

C_SRCS += \
../src/bin/physmod.pt/mkpcp/calc_snow.c \
../src/bin/physmod.pt/mkpcp/main.c 

OBJS += \
./src/bin/physmod.pt/mkpcp/calc_snow.o \
./src/bin/physmod.pt/mkpcp/main.o 

C_DEPS += \
./src/bin/physmod.pt/mkpcp/calc_snow.d \
./src/bin/physmod.pt/mkpcp/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/bin/physmod.pt/mkpcp/%.o: ../src/bin/physmod.pt/mkpcp/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gccgcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


